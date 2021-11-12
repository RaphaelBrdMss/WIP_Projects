//
//  app.hpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#ifndef app_hpp
#define app_hpp

#include <stdio.h>
#include "Window.hpp"
#include "V_pipeline.hpp"
namespace Vwin{
class App{
    
    public :
    int w = 800;
    int h = 600;
    
    void run();
    
    private :
    VulkWindow vulkWindow{w,h,"hey bitch that's vulkan"};
    V_Pipline pipeline{"shaders/Vertex_shader.vert.spv","shaders/Frag_shader.frag.spv"};
};
}

#endif /* app_hpp */
