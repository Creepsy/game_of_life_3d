#include "../libs/glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>

#include "render_window.h"


int main() {    
    render_window window{800, 600, "Game of Life 3D"};

    while(!window.should_close()) {
        window.clear(0.5f, 0.2f, 0.35f, 1.0f);

        //rendering goes here

        window.update();
    }

    return 0;
}