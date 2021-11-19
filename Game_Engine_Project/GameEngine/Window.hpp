//
//  HeaderWin.h
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#ifndef HeaderWin_h
#define HeaderWin_h
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
 

namespace vulkbrad {
class VulkWindow{
    
    public :
    VulkWindow(int windowWidth, int windowHeight, std::string windowName);
    ~VulkWindow();
    
    VulkWindow(const VulkWindow &) = delete;
    VulkWindow &operator = (const VulkWindow &) =delete; 
    
    bool shouldClose(){ return glfwWindowShouldClose(window); }
    VkExtent2D getScaleWindow() { return {static_cast<uint32_t>(WW), static_cast<uint32_t>(WH)};}
    
    void createWinSurface(VkInstance instance, VkSurfaceKHR *surface);
    private :
    GLFWwindow *window;
    void initWindow();
    const int WW;
    const int WH;
    std::string name;
    
};
}



#endif /* HeaderWin_h */
