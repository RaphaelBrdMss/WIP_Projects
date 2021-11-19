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
#include "BradEngineDevice.hpp"
#include "BradEngineSwapChain.hpp"
#include <vector>


#include <memory>

namespace vulkbrad{
class App{
    
    public :
    static constexpr int w = 800;
    static constexpr int h = 600;
    
    App();
    ~App();
    App(const App &) = delete;
    App &operator = (const App &) =delete;

    
    void run();
    
    private :
    
    void createPipeline_Layout();
    void createPipline();
    void createCommandBuffers();
    void drawFrame();
    
    
    VulkWindow vulkWindow{w,h,"hey that's vulkan"};
    BradEngineDevice BradEngineDevice{vulkWindow};
    BradEngineSwapChain BradEngineSwapChain{BradEngineDevice, vulkWindow.getScaleWindow()};
    std::unique_ptr<V_Pipeline> pipeline; // BradCom : smart pointer simulate a pointer with addition automatic memory management we are no longer responsable for new and delete
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    
     

};
}

#endif /* app_hpp */
