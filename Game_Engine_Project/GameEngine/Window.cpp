//
//  Window.cpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#include "Window.hpp"

namespace Vwin{

VulkWindow::VulkWindow(int windowWidth, int windowHeight, std::string windoName) : WW{windowWidth}, WH{windowHeight}, name{windoName}{
    
    initWindow();
}

VulkWindow::~VulkWindow(){
    glfwDestroyWindow(window);
    glfwTerminate();
}
void VulkWindow::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(WW, WH, name.c_str(), nullptr, nullptr);
}

}
