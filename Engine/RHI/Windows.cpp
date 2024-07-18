//
// Created by win111 on 2024/5/30.
//

#include "Windows.h"

Windows::Windows(const WindowsSetting &setting) {


    initGlfw();
    width = setting.width;
    hight = setting.hight;

    title = setting.title;

    ptr_window = glfwCreateWindow(width,hight,title.c_str(), nullptr, nullptr);



}

Windows::~Windows() {
    glfwTerminate();
    glfwDestroyWindow(ptr_window);
}

Windows::Windows() {
    initGlfw();
    ptr_window = glfwCreateWindow(width,hight,title.c_str(), nullptr, nullptr);

}

void Windows::initGlfw() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    LOG_INFO("GLFW initialized for Vulkan");

}

void Windows::loop() {
    while(!glfwWindowShouldClose(ptr_window))
    {
        glfwPollEvents();
        glfwSwapBuffers(ptr_window);
    }
}



