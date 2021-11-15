//
//  First_app.hpp
//  Vulkan_Engin
//
//  Created by Anna Diani on 12/11/2021.
//

#ifndef First_app_hpp
#define First_app_hpp

#include <stdio.h>

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"

#include "First_app.hpp"
namespace lve{

class FirstApp {
    
    
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    void run();
private:
    LveWindow lveWindow{WIDTH, HEIGHT, "Hi BITCH"};
    LveDevice lveDevice{lveWindow};
    LvePipeline lvePipline {
        lveDevice,
        "/Users/anna/Documents/Vulkan_Engin/Vulkan_Engin/Shaders/simple_shader.vert.spv",
        "/Users/anna/Documents/Vulkan_Engin/Vulkan_Engin/Shaders/simple_shader.frag.spv",
        LvePipeline::defaultPipelineConfigIndo(WIDTH, HEIGHT)
        
    
    };
};
}

#endif /* First_app_hpp */
