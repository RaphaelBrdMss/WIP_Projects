//
//  app.cpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#include "app.hpp"
#include <stdexcept>
#include <array>

namespace vulkbrad{

App::App(){
    createPipeline_Layout();
    createPipline();
    createCommandBuffers();
}
App::~App(){
    vkDestroyPipelineLayout(BradEngineDevice.device(), pipelineLayout, nullptr);
}

void App::run(){
    while(!vulkWindow.shouldClose()){
        glfwPollEvents();
        drawFrame();
    }
    
    vkDeviceWaitIdle(BradEngineDevice.device());
}

void App::createPipeline_Layout(){
    VkPipelineLayoutCreateInfo pipelineLayout_Info{};
    pipelineLayout_Info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayout_Info.setLayoutCount = 0;
    pipelineLayout_Info.pSetLayouts = nullptr;
    pipelineLayout_Info.pushConstantRangeCount=0;
    pipelineLayout_Info.pPushConstantRanges=nullptr;
    if(vkCreatePipelineLayout(BradEngineDevice.device(), &pipelineLayout_Info, nullptr, &pipelineLayout) !=  VK_SUCCESS){
        throw  std::runtime_error("fail to create pipeline layout");
        
    }
    
    
}
void App::createPipline(){
    Pipeline_configInfo pipelineConfig{};
    V_Pipeline::defaultPiplineConfigInfo(pipelineConfig , BradEngineSwapChain.width(),BradEngineSwapChain.height());
    pipelineConfig.renderPass = BradEngineSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<V_Pipeline>(BradEngineDevice, "shaders/Vertex_shader.vert.spv", "shaders/Frag_shader.frag.spv" , pipelineConfig);
    
    
}

void App::createCommandBuffers(){
    commandBuffers.resize(BradEngineSwapChain.imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = BradEngineDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    if(vkAllocateCommandBuffers(BradEngineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("fail to create the command buffer");
    }
    
    for(int i = 0 ; i<commandBuffers.size(); i++){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
            throw  std::runtime_error("failed to begin recording commad buffer");
            
        }
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = BradEngineSwapChain.getRenderPass();
        renderPassInfo.framebuffer = BradEngineSwapChain.getFrameBuffer(i);
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = BradEngineSwapChain.getSwapChainExtent();
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f,0.1f,0.1f,1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        
        pipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0); // Bradcom : draw 3 vertices in one instances.
        
        vkCmdEndRenderPass(commandBuffers[i]);
        if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS ){
            throw std::runtime_error("failed to record commad buffer");
        }
        
        
        
        
    }
        
    
}
void App::drawFrame(){
    uint32_t image_Index;
    auto result = BradEngineSwapChain.acquireNextImage(&image_Index); // bradcom : fetches the index of the frame wich should render, automaticly handle the gpu and cpu synchro (double or tripple buffering)
    if(result !=VK_SUCCESS && result!= VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("failed to acquire swap chain image");
        
    }
    result = BradEngineSwapChain.submitCommandBuffers(&commandBuffers[image_Index], &image_Index); // bradcom : submit the provided command buffer to our device graphics queue while handling th cpu and gpu syncro
    
    if(result !=VK_SUCCESS){
        throw std::runtime_error("failed to present swap chain image");
        
    }
    
}
}
 
