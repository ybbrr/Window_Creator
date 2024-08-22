///
/// @file window_properties.hpp
/// @author Yasin BASAR
/// @brief Defined the WindowProperties class for creating and filling windows with given buffers.
/// @version 1.0.0
/// @date 22/08/2014
/// @copyright Copyright (c) 2024
///

#ifndef WINDOW_PROPERTIES_HPP
#define WINDOW_PROPERTIES_HPP

////////////////////////////////////////////////////////////////////////////////
// Project Includes
////////////////////////////////////////////////////////////////////////////////

#include "i_window_properties.hpp"

////////////////////////////////////////////////////////////////////////////////
// Third Party Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Forward and Typedef Declarations
////////////////////////////////////////////////////////////////////////////////

struct GLFWwindow;

typedef int GLint;
typedef unsigned int GLuint;

namespace YB
{
    class WindowProperties final : public IWindowProperties
    {
    public:
    ////////////////////////////////////////////////////////////////////////////
    // Special Members
    ////////////////////////////////////////////////////////////////////////////

        WindowProperties() noexcept = delete; ///< Deleted default constructor
        WindowProperties(WindowProperties &&) noexcept = default; ///< Default move constructor
        WindowProperties &operator=(WindowProperties &&) noexcept = default; ///< Default move assignment operator
        WindowProperties(const WindowProperties &) noexcept = default; ///< Default copy constructor
        WindowProperties &operator=(WindowProperties const &) noexcept = default; ///< Default copy assignment operator

    ////////////////////////////////////////////////////////////////////////////
    // Public Members
    ////////////////////////////////////////////////////////////////////////////

        /// @brief It creates GLFW Context.
        /// @param window_name[in] Window Name
        /// @param window_width[in] Window Width
        /// @param window_height[in] Window Height
        /// @param cato_screen_frame_rate[in] Set false this parameter for unlimited framerate,
        /// otherwise window's framerate will be capped to the screen's framerate.
        WindowProperties(const char* window_name,
                         int window_width,
                         int window_height,
                         bool cato_screen_frame_rate);

        /// @brief Destructs window creator object.
        ~WindowProperties() noexcept override;

        /// @brief It runs window image rendering
        /// @param data_ptr[in] Image buffer pointer. Only 4 channel images acceptable.
        /// @param frame_width[in] Image Width
        /// @param frame_height[in] Image Height
        /// @param exit_status[out] Use it to end your render loop
        void image_render(uint8_t* data_ptr,
                          int frame_width,
                          int frame_height,
                          volatile bool& exit_status) noexcept override;

    ////////////////////////////////////////////////////////////////////////////
    // Private Members
    ////////////////////////////////////////////////////////////////////////////
    private:

        /// @brief It sets shader properties.
        static void set_window_properties() noexcept;

        /// @brief It complies the shader codes if there is no error.
        ///        If there is an error print it.
        /// @param type[in] Shader Type (Vertex or Fragment)
        /// @param source[in] Shader code as string
        /// @return uint32_t - Shader ID
        static GLuint shader_compile(uint32_t type,
                              const char* source);

        /// @brief It creates GLFW Window.
        /// @return Shaders attached Program object's ID
        static GLuint shader_create() noexcept;

        /// @brief Keyboard callback function for handling key events.
        /// @param window[in] The GLFW window pointer.
        /// @param key[in] The key code.
        /// @param scancode[in] The system-specific scancode.
        /// @param action[in] The action (e.g., press, release).
        /// @param mode[in] The modifier key state.
        static void keyboard_callback(GLFWwindow* window,
                                      int key,
                                      int scancode,
                                      int action,
                                      int mode) noexcept;

        /// @brief Instance-specific window resize callback.
        ///        Prevents windows from affecting each other when resizing.
        void resize_window(int width, int height) const noexcept;

        /// @brief Window resize callback function to update the viewport.
        /// @param window[in] The GLFW window pointer.
        /// @param width[in] The new width of the window.
        /// @param height[in] The new height of the window.
        static void window_resize_callback(GLFWwindow* window,
                                           int width,
                                           int height) noexcept;

#ifdef _WIN32
        /// @brief Checks if Dark Mode enabled in Windows Operating System
        /// @return returns true if Dark Mode enabled, otherwise returns false
        static bool is_dark_mode_enabled() noexcept;
#endif

        static const char* m_vertex_shader; ///< Holds the vertex shader code as string.
        static const char* m_fragment_shader; ///< Holds the fragment shader code as string.
        static GLuint m_VAO; ///< Vertex Array Buffer of OpenGL operations.
        static GLuint m_VBO; ///< Vertex Buffer Object of OpenGL operations.
        static GLuint m_IBO; ///< Index Buffer Object of OpenGL operations.
        static GLuint m_shader_program; ///< Shader program attach id of OpenGL operations.
        static GLuint m_vertex_shader_id; ///< Vertex shader attach id of OpenGL operations.
        static GLuint m_fragment_shader_id; ///< Fragment shader attach id of OpenGL operations.
        static GLuint m_texture_output; ///< Texture attach id of OpenGL operations.
        static GLint m_location; ///< Location id that match locations ids in the shader code.
        static bool m_is_common_window_resources_cleared; ///< A variable to check when there are multiple windows to not re-clear static resources.
        GLFWwindow* m_window; ///< Window context object.

    ////////////////////////////////////////////////////////////////////////////
    // Protected Members
    ////////////////////////////////////////////////////////////////////////////
    protected:

        // Data

    };
} // YB

#endif // WINDOW_PROPERTIES_HPP

/* end_of_file */
