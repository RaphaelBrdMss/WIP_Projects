//
//  app.cpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#include "app.hpp"

namespace Vwin{
void App::run(){
    while(!vulkWindow.shouldClose()){
        glfwPollEvents();
    }
    
    
    
    
}
}
