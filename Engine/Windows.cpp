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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

}

void Windows::loop() {
    while(!glfwWindowShouldClose(ptr_window))
    {
        glfwPollEvents();
        glfwSwapBuffers(ptr_window);
    }
}



