///
/// @file window_properties.cpp
/// @author Yasin BASAR
/// @brief Implements the `Window` class for managing a GLFW window and OpenGL context.
/// @version 1.0.0
/// @date 22/08/2024
/// @copyright Copyright (c) 2024
///

////////////////////////////////////////////////////////////////////////////////
// Project Includes
////////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <iostream>
#include <stdexcept>
#include "window_properties.hpp"

#ifdef _WIN32
#include <windows.h>
#include <uxtheme.h>
#include <dwmapi.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// Third Party Includes
////////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace YB
{
    GLuint WindowProperties::m_VAO{};
    GLuint WindowProperties::m_VBO{};
    GLuint WindowProperties::m_IBO{};
    GLuint WindowProperties::m_shader_program{};
    GLuint WindowProperties::m_vertex_shader_id{};
    GLuint WindowProperties::m_fragment_shader_id{};
    GLuint WindowProperties::m_texture_output{};
    GLint WindowProperties::m_location{};
    bool WindowProperties::m_is_common_window_resources_cleared{false};

    const char* WindowProperties::m_vertex_shader = "\n"
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(position.xy, 0.0, 1.0);\n"
        "   TexCoord = aTexCoord;\n"
        "}\n"
        "\n";

    const char* WindowProperties::m_fragment_shader = "\n"
        "#version 330 core\n"
        "layout (location = 0) out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D t_Texture;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(t_Texture, TexCoord);\n"
        "}\n"
        "\n";

    ///
    /// @brief Checks for OpenGL errors and logs them.
    ///
    /// @param file The file name where the error check is performed.
    /// @param line The line number where the error check is performed.
    /// @return GLenum The last recorded OpenGL error code.
    ///
    GLenum glCheckError_(const char* file, int line)
    {
        GLenum error_code;
        while ((error_code = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (error_code)
            {
                case GL_INVALID_ENUM:
                    error = "INVALID_ENUM";
                break;
                case GL_INVALID_VALUE:
                    error = "INVALID_VALUE";
                break;
                case GL_INVALID_OPERATION:
                    error = "INVALID_OPERATION";
                break;
                case GL_STACK_OVERFLOW:
                    error = "STACK_OVERFLOW";
                break;
                case GL_STACK_UNDERFLOW:
                    error = "STACK_UNDERFLOW";
                break;
                case GL_OUT_OF_MEMORY:
                    error = "OUT_OF_MEMORY";
                break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
                default:
                    break;
            }
            std::cout << error << " - error code: " << error_code << " | " << file << " (" << line << ")" << "\n";
        }
        return error_code;
    }
#define glCheckError() glCheckError_(__FILE__, __LINE__)

////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////

    WindowProperties::WindowProperties(const char *window_name,
                                       int window_width,
                                       int window_height,
                                       bool cap_to_screen_frame_rate) :
        m_window{nullptr}
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Could not start GLFW3");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        this->m_window = glfwCreateWindow(window_width,
                                          window_height,
                                          window_name,
                                          nullptr,
                                          nullptr);

        if (!this->m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Could not create GLFW3 Window");
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(this->m_window);

        if (cap_to_screen_frame_rate)
        {
            glfwSwapInterval(1); // set 1 to limit fps to the screen's framerate
        }
        else
        {
            glfwSwapInterval(0); // set 0 for un-limited fps
        }

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            throw std::runtime_error("Could not initialize GLAD");
        }

        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(this->m_window, keyboard_callback);
        glfwSetWindowSizeCallback(this->m_window, window_resize_callback);

#ifdef _WIN32
        if (is_dark_mode_enabled())
        {
            // Get the native GLFW window handle
            HWND hwnd = glfwGetWin32Window(this->m_window);

            // You can also use DwmSetWindowAttribute for more detailed settings
            BOOL dark_mode = TRUE;
            (void)DwmSetWindowAttribute(hwnd,
                                        DWMWA_USE_IMMERSIVE_DARK_MODE,
                                        &dark_mode,
                                        sizeof(dark_mode));
        }
#endif

        glfwShowWindow(this->m_window);

        set_window_properties();
    }

    WindowProperties::~WindowProperties()
    {
        // Ensure the context is current
        glfwMakeContextCurrent(this->m_window);

        if (!m_is_common_window_resources_cleared)
        {
            if (m_VAO != 0)
            {
                glDeleteVertexArrays(1, &m_VAO);
            }
            if (m_VBO != 0)
            {
                glDeleteBuffers(1, &m_VBO);
            }
            if (m_IBO != 0)
            {
                glDeleteBuffers(1, &m_IBO);
            }

            if (m_shader_program != 0)
            {
                if (m_vertex_shader_id != 0)
                {
                    glDetachShader(m_shader_program, m_vertex_shader_id);
                }
                if (m_fragment_shader_id != 0)
                {
                    glDetachShader(m_shader_program, m_fragment_shader_id);
                }
                glDeleteProgram(m_shader_program);
            }

            m_is_common_window_resources_cleared = true;
        }

        glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    void WindowProperties::image_show(uint8_t * data_ptr,
                                      int frame_width,
                                      int frame_height,
                                      volatile bool & exit_status) noexcept
    {

        glfwMakeContextCurrent(this->m_window);

        glCheckError();

        /* Loop until the user closes the window */
        if (!glfwWindowShouldClose(this->m_window))
        {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(m_shader_program);

            glActiveTexture(GL_TEXTURE0);
            glUniform1i(m_location, 0);
            glBindTexture(GL_TEXTURE_2D, m_texture_output);

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA8,
                         frame_width,
                         frame_height,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         data_ptr);

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);

            glfwSwapBuffers(this->m_window);

            glCheckError();
        }
        else
        {
            exit_status = true;
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Private Functions
////////////////////////////////////////////////////////////////////////////////

    void WindowProperties::set_window_properties() noexcept
    {
        const float positions[16] =
        {
            //X    //Y   //U   //V 
            -1.0f, -1.0f, 0.0f, 0.0f, // 0
            1.0f, -1.0f, 1.0f, 0.0f, // 1
            1.0f,  1.0f, 1.0f, 1.0f, // 2
            -1.0f,  1.0f, 0.0f, 1.0f  // 3
        };

        const uint32_t indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), positions, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<const void*>(2 * sizeof(float)));
        
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

        m_shader_program = shader_create();
        glUseProgram(m_shader_program);

        glGenTextures(1, &m_texture_output);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture_output);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, m_texture_output);

        m_location = glGetUniformLocation(m_shader_program, "t_Texture");

        if (-1 == m_location)
        {
            printf("Uniform location could not find (texture).\n");
        }

        glUniform1i(m_location, 0);

        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint WindowProperties::shader_compile(uint32_t type,
                                            const char *source)
    {
        uint32_t id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (GL_FALSE == result)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = new char[length];
            glGetShaderInfoLog(id, length, &length, message);

            printf("\nFailed to compile Shader!\n%s\n", message);
            glDeleteShader(id);
            delete[] message;
            
            throw std::runtime_error("Failed to compile Shader!");
        }

        return id;
    }

    GLuint WindowProperties::shader_create() noexcept
    {
        GLuint program = glCreateProgram();

        m_vertex_shader_id = shader_compile(GL_VERTEX_SHADER, m_vertex_shader);
        m_fragment_shader_id = shader_compile(GL_FRAGMENT_SHADER, m_fragment_shader);

        glAttachShader(program, m_vertex_shader_id);
        glAttachShader(program, m_fragment_shader_id);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(m_vertex_shader_id);
        glDeleteShader(m_fragment_shader_id);

        return program;
    }

    void WindowProperties::keyboard_callback(GLFWwindow* window,
                                             int key,
                                             int scancode,
                                             int action,
                                             int mode) noexcept
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    void WindowProperties::resize_window(int width, int height) const noexcept
    {
        glViewport(0, 0, width, height);
    }

    void WindowProperties::window_resize_callback(GLFWwindow* window,
                                                  int width,
                                                  int height) noexcept
    {
        // Retrieve the instance from the user pointer
        WindowProperties* instance
            = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

        // Call the member function if the instance exists
        if (instance)
        {
            // Make the context for the window current
            glfwMakeContextCurrent(window);

            // Call the member function
            instance->resize_window(width, height);
        }
    }

#ifdef _WIN32
    bool WindowProperties::is_dark_mode_enabled() noexcept
    {
        HKEY h_Key;

        const char* registry_path
            = R"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)";

        LONG result = RegOpenKeyExA(HKEY_CURRENT_USER,
                                    registry_path,
                                    0,
                                    KEY_READ,
                                    &h_Key);

        DWORD dark_mode = 0;

        if (result == ERROR_SUCCESS)
        {
            DWORD data_size = sizeof(dark_mode);
            result = RegQueryValueExA(h_Key,
                                      "AppsUseLightTheme",
                                      nullptr,
                                      nullptr,
                                      reinterpret_cast<LPBYTE>(&dark_mode),
                                      &data_size);
        }

        RegCloseKey(h_Key);

        return result == ERROR_SUCCESS && dark_mode == 0; // 0 means dark mode, 1 means light mode
    }
#endif

////////////////////////////////////////////////////////////////////////////////
// Protected Functions
////////////////////////////////////////////////////////////////////////////////

} // YB

/* End of File */
