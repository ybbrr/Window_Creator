///
/// @file example_main.cpp
/// @author Yasin BASAR
/// @brief Example usage of Window Handler
/// @version 1.0.0
/// @date 22/08/2024
/// @copyright Copyright (c) 2024
/// 

#include <window_creator.hpp>

int main()
{
    int window_width=200;
    int window_height=200;

    std::unique_ptr<YB::WindowCreator> window{
        new YB::WindowCreator("test window", 2 * window_width, 2 * window_height, true)
    };

    std::unique_ptr<YB::WindowCreator> window2{
        new YB::WindowCreator("test window 2", 3 * window_width, 3 * window_height, true)
    };

    std::unique_ptr<YB::WindowCreator> window3{
        new YB::WindowCreator("test window 3", 4 * window_width, 4 * window_height, true)
    };

    int frame_width = 1600;
    int frame_height = 900;
    int channel = 4;
    int len= frame_width * frame_height * channel;

    uint8_t* red_image = new uint8_t[len];
    for (int idx = 0; idx < len; idx+=4)
    {
        red_image[idx] = 255;
        red_image[idx+1] = 0;
        red_image[idx+2] = 0;
        red_image[idx+3] = 255;
    }

    uint8_t* green_image = new uint8_t[len];
    for (int idx = 0; idx < len; idx+=4)
    {
        green_image[idx] = 0;
        green_image[idx+1] = 255;
        green_image[idx+2] = 0;
        green_image[idx+3] = 255;
    }

    uint8_t* blue_image = new uint8_t[len];
    for (int idx = 0; idx < len; idx+=4)
    {
        blue_image[idx] = 0;
        blue_image[idx+1] = 0;
        blue_image[idx+2] = 255;
        blue_image[idx+3] = 255;
    }

    volatile bool exit_status = false;

    // Press ESC to close the window
    while (!exit_status)
    {
        window->image_show(red_image, frame_width, frame_height, exit_status);
        window2->image_show(green_image, frame_width, frame_height, exit_status);
        window3->image_show(blue_image, frame_width, frame_height, exit_status);
    }

    delete[] red_image;
    delete[] green_image;
    delete[] blue_image;

    return 0;
}

/* End Of File */
