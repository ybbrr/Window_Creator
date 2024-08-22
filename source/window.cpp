///
/// @file window.cpp
/// @author Yasin BASAR
/// @brief OpenGL Window context creator.
/// @version 1.0.0
/// @date 20/07/2014
/// @copyright Copyright (c) 2024
/// 

////////////////////////////////////////////////////////////////////////////////
// Project Includes
////////////////////////////////////////////////////////////////////////////////

#include "window_properties.hpp"
#include "window.hpp"

////////////////////////////////////////////////////////////////////////////////
// Third Party Includes
////////////////////////////////////////////////////////////////////////////////

namespace YB
{
    class Window::Impl
    {
    public:
    ////////////////////////////////////////////////////////////////////////////
    // Special Members
    ////////////////////////////////////////////////////////////////////////////

        ~Impl() noexcept = default;
        Impl(Impl &&) noexcept = default;
        Impl &operator=(Impl &&) noexcept = default;
        Impl(const Impl &) noexcept = delete;
        Impl &operator=(Impl const &) noexcept = delete;

    ////////////////////////////////////////////////////////////////////////////
    // Public Members
    ////////////////////////////////////////////////////////////////////////////

        Impl(const char *window_name,
             int window_width,
             int window_height,
             bool cap_to_screen_frame_rate)
        {
            this->m_window_properties
                = std::make_unique<WindowProperties>(window_name,
                                                     window_width,
                                                     window_height,
                                                     cap_to_screen_frame_rate);
        }

        void render(uint8_t * data_ptr,
                    int frame_width,
                    int frame_height,
                    volatile bool& t_exit_status) const noexcept
        {
            this->m_window_properties->image_render(data_ptr,
                                                    frame_width,
                                                    frame_height,
                                                    t_exit_status);
        }

    ////////////////////////////////////////////////////////////////////////////
    // Private Members
    ////////////////////////////////////////////////////////////////////////////
    private:

        std::unique_ptr<IWindowProperties> m_window_properties;
    };

////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////

    Window::Window(const char *window_name,
                   int window_width,
                   int window_height,
                   bool cap_to_screen_frame_rate)
    {
        this->p_impl = std::make_unique<Impl>(window_name,
                                              window_width,
                                              window_height,
                                              cap_to_screen_frame_rate);
    }

    Window::~Window() noexcept = default;

    void Window::render(uint8_t *data_ptr,
                        int frame_width,
                        int frame_height,
                        volatile bool &exit_status) const noexcept
    {
        this->p_impl->render(data_ptr,
                             frame_width,
                             frame_height,
                             exit_status);
    }

////////////////////////////////////////////////////////////////////////////////
// Private Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Protected Functions
////////////////////////////////////////////////////////////////////////////////

} // YB

/* End of File */
