//
//  lve_window.cpp
//  Vulkan_Engin
//
//  Created by Anna Diani on 12/11/2021.
//

#include "lve_window.hpp"

namespace lve{
LveWindow::LveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}{
    initWindow();
}

LveWindow::~LveWindow(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void LveWindow::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
void LveWindow::creatWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
        throw std::runtime_error("Failed to create window surface");
    }
}
} //namespace lve
