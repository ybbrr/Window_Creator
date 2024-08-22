///
/// @file example_main.cpp
/// @author Yasin BASAR
/// @brief Example usage of Window Handler
/// @version 1.0.0
/// @date 20/07/2014
/// @copyright Copyright (c) 2024
/// 

#include <iostream>
#include <window.hpp>

int main()
{
    int window_width=400;
    int window_height=400;

    std::unique_ptr<YB::Window> window{
        new YB::Window("test window", window_width, window_height, true)
    };

    std::unique_ptr<YB::Window> window2{
        new YB::Window("test window 2", 1.5f * window_width, 1.5f * window_height, true)
    };

    std::unique_ptr<YB::Window> window3{
        new YB::Window("test window 3", 1.8f * window_width, 1.8f * window_height, true)
    };

    int frame_width = 1600;
    int frame_height = 900;
    int channel = 4;
    int len= frame_width * frame_height * channel;

    uint8_t* white = new uint8_t[len];
    memset(white, 255, len);

    uint8_t* gray = new uint8_t[len];
    memset(gray, 128, len);

    uint8_t* dark = new uint8_t[len];
    memset(dark, 34, len);

    volatile bool exit_status = false;

    // Press ESC to close the window
    while (!exit_status)
    {
        window->render(white, frame_width, frame_height, exit_status);
        window2->render(gray, frame_width, frame_height, exit_status);
        window3->render(dark, frame_width, frame_height, exit_status);
    }

    return 0;
}

/* End Of File */
