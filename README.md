# Window_Creator
I wrote a library to easily create a high functional, flexible OpenGL Windows.<br />

You can create as many windows you want.<br />
You can fill your windows with the image buffer you want to see on the screen.<br />
You can interact with your windows separately.<br />

All windows are auto resizable.<br />

And it is cross-platform for Windows and Linux.

## How to add Third Parties to Project Workspace Folder
Download the third parties from pre-releases here: https://github.com/ybbrr/Window_Creator/releases/tag/third_parties <br />
And extract the content of `__third-parties__.zip` to Workspace Folder of the project.

## How to compile Example Executable or Library

### Windows

To compile example executable --> Run `build_example.bat`

To compile static or shared library --> Run `build_lib.bat`

### Linux

To compile example executable --> Run `sh build_example.sh`

To compile static or shared library --> Run `sh build_lib.sh`

## Usage

```c++
#include <window_creator.hpp>

int main()
{
    int window_width=200;
    int window_height=200;

    std::unique_ptr<YB::WindowCreator> window1{
        new YB::WindowCreator("test window 1", 2 * window_width, 2 * window_height, true)
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
        window1->image_show(red_image, frame_width, frame_height, exit_status);
        window2->image_show(green_image, frame_width, frame_height, exit_status);
        window3->image_show(blue_image, frame_width, frame_height, exit_status);
    }

    delete[] red_image;
    delete[] green_image;
    delete[] blue_image;

    return 0;
}
```

## Screenshots

### Linux Screenshot

<a href="https://i.ibb.co/7gNddLj/Screenshot-from-2024-08-22-23-54-05.png"><img src="https://i.ibb.co/7gNddLj/Screenshot-from-2024-08-22-23-54-05.png" alt="Window_Creator_Screenshot_01" border="0"></a>

### Windows Screenshot

<a href="https://i.ibb.co/Nt1DByM/Screenshot-4.png"><img src="https://i.ibb.co/Nt1DByM/Screenshot-4.png" alt="Window_Creator_Screenshot_02" border="0"></a>

[comment]: #end_of_file
