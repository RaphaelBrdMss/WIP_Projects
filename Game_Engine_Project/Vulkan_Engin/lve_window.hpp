//
//  lve_window.hpp
//  Vulkan_Engin
//
//  Created by Anna Diani on 12/11/2021.
//
#pragma once

#ifndef lve_window_hpp
#define lve_window_hpp

#include <stdio.h>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace lve {

class LveWindow {
    
public:
    LveWindow(int w, int h, std::string name);
    ~LveWindow();
    
    LveWindow(const LveWindow &) = delete;
    LveWindow &operator=(const LveWindow &) = delete;
    
    bool shouldClose(){
        return glfwWindowShouldClose(window);
    }
    
    void creatWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    
    
    private:
    
        void initWindow();
        
        const int width;
        const int height;
    
        std::string windowName;
    
        GLFWwindow *window;
};
}

#endif /* lve_window_hpp */
