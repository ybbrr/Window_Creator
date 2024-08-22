///
/// @file example_main.cpp
/// @author Yasin BASAR
/// @brief Example usage of Window Handler
/// @version 1.0.0
/// @date 20/07/2014
/// @copyright Copyright (c) 2024
/// 

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

    uint8_t* red = new uint8_t[len];
    for (int idx = 0; idx < len; idx+=4)
    {
        red[idx] = 255;
        red[idx+1] = 0;
        red[idx+2] = 0;
        red[idx+3] = 255;
    }

    uint8_t* green = new uint8_t[len];
    for (int idx = 0; idx < len; idx+=4)
    {
        green[idx] = 0;
        green[idx+1] = 255;
        green[idx+2] = 0;
        green[idx+3] = 255;
    }

    uint8_t* blue = new uint8_t[len];
    for (int idx = 0; idx < len; idx+=4)
    {
        blue[idx] = 0;
        blue[idx+1] = 0;
        blue[idx+2] = 255;
        blue[idx+3] = 255;
    }

    volatile bool exit_status = false;

    // Press ESC to close the window
    while (!exit_status)
    {
        window->render(red, frame_width, frame_height, exit_status);
        window2->render(green, frame_width, frame_height, exit_status);
        window3->render(blue, frame_width, frame_height, exit_status);
    }

    delete[] red;
    delete[] green;
    delete[] blue;

    return 0;
}

/* End Of File */
