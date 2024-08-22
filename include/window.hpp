///
/// @file window.hpp
/// @author Yasin BASAR
/// @brief OpenGL Window context creator.
/// @version 1.0.0
/// @date 20/07/2014
/// @copyright Copyright (c) 2024
///

#ifndef WINDOW_HANDLER_HPP
#define WINDOW_HANDLER_HPP

#ifdef _WIN32
    #if defined(STATIC_EXPORT)
        #define DLL_SPECS
    #elif defined(SHARED_EXPORT)
        #define DLL_SPECS __declspec(dllexport)
    #else
        #define DLL_SPECS __declspec(dllimport)
    #endif
#else
    #define DLL_SPECS
#endif

////////////////////////////////////////////////////////////////////////////////
// Project Includes
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Third Party Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Forward and Typedef Declarations
////////////////////////////////////////////////////////////////////////////////

struct GLFWwindow;

namespace YB
{
    /// @class Window
    /// @brief
    class DLL_SPECS Window
    {
    public:
    ////////////////////////////////////////////////////////////////////////////
    // Special Members
    ////////////////////////////////////////////////////////////////////////////

        Window() noexcept = delete; ///< Deleted default constructor
        Window(Window &&) noexcept = delete; ///< Default move constructor
        Window &operator=(Window &&) noexcept = delete; ///< Default move assignment operator
        Window(const Window &) noexcept = delete; ///< Default copy constructor
        Window &operator=(Window &) noexcept = delete; ///< Default copy assignment operator

    ////////////////////////////////////////////////////////////////////////////
    // Public Members
    ////////////////////////////////////////////////////////////////////////////

        /// @brief It creates GLFW Context.
        /// @param window_name[in] Window Name
        /// @param window_width[in] Window Width
        /// @param window_height[in] Window Height
        /// @param cap_to_screen_frame_rate[in] Set false this parameter for unlimited framerate,
        /// otherwise window's framerate will be capped to the screen's framerate.
        Window(const char* window_name,
                      int window_width,
                      int window_height,
                      bool cap_to_screen_frame_rate);

        /// @brief Destructs window creator object.
        ~Window() noexcept;

	    /// @brief It runs window render cycle
	    /// @param data_ptr[in] Image buffer pointer. Only 4 channel images acceptable.
	    /// @param frame_width[in] Image Width
	    /// @param frame_height[in] Image Height
	    /// @param exit_status[out] Use it to end your render loop
	    void render(uint8_t * data_ptr,
	                int frame_width,
	                int frame_height,
	                volatile bool& exit_status) const noexcept;

        /// @brief It returns Window Context.
        /// @return GLFWwindow* - Window Context pointer
        GLFWwindow* get_window_context() const noexcept;

    ////////////////////////////////////////////////////////////////////////////
    // Private Members
    ////////////////////////////////////////////////////////////////////////////
    private:

        class Impl; ///< Implementation class
        std::unique_ptr<Impl> p_impl; ///< Pointer to implementation class

    ////////////////////////////////////////////////////////////////////////////
    // Protected Members
    ////////////////////////////////////////////////////////////////////////////
    protected:

        // Data

    };
} // YB

#endif // WINDOW_HANDLER_HPP

/* End of Fie */
