//
// Created by win111 on 2024/5/30.
//

#ifndef VULKANABSTRACTIONLAYER_WINDOWS_H
#define VULKANABSTRACTIONLAYER_WINDOWS_H


#include "pch.h"

struct WindowsSetting{
    unsigned int hight;
    unsigned int width;

    std::string title;

};

class Windows {



private:
    GLFWwindow* ptr_window = {};
    unsigned int hight = 720;
    unsigned int width = 1080;
    std::string  title = {"default"};

    void initGlfw();

public:
    Windows(const WindowsSetting& setting);
    Windows();
    ~Windows();

    GLFWwindow * getWindowHandle()const{
        return ptr_window;
    }

    void loop();

};


#endif //VULKANABSTRACTIONLAYER_WINDOWS_H
