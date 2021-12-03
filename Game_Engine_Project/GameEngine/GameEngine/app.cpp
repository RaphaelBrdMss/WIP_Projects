//
//  app.cpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#include "app.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <cstdlib>
#include <iostream>
namespace vulkbrad{

struct SimplePushConstantData{
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};
App::App(){
    loadModels();
    createPipeline_Layout();
    recreateSwapChain();
    createCommandBuffers();
}
App::~App(){
    vkDestroyPipelineLayout(bradEngineDevice.device(), pipelineLayout, nullptr);
}

void App::run(){
    

    while(!vulkWindow.shouldClose()){
        glfwPollEvents();
        drawFrame();
    }
    
    vkDeviceWaitIdle(bradEngineDevice.device());
}

void App::loadModels(){
    std::vector<BradModel::Vertex> vertices{
        {{0.0f,-.50f},{1.0f,.0f,.0f}},
        {{.50f,.50f},{.0f,1.0f,.0f}},
        {{-.50f,.50f},{.0f,.0f,1.0f}}
    };
    
    bradModel = std::make_unique<BradModel>(bradEngineDevice, vertices);
}

void App::createPipeline_Layout(){
    
    VkPushConstantRange pushConstatnRange{};
    
    pushConstatnRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstatnRange.offset = 0;
    pushConstatnRange.size = sizeof(SimplePushConstantData);
    
    
    VkPipelineLayoutCreateInfo pipelineLayout_Info{};
    pipelineLayout_Info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayout_Info.setLayoutCount = 0;
    pipelineLayout_Info.pSetLayouts = nullptr;
    pipelineLayout_Info.pushConstantRangeCount=1;
    pipelineLayout_Info.pPushConstantRanges=&pushConstatnRange;
    if(vkCreatePipelineLayout(bradEngineDevice.device(), &pipelineLayout_Info, nullptr, &pipelineLayout) !=  VK_SUCCESS){
        throw  std::runtime_error("fail to create pipeline layout");
        
    }
    
    
}
void App::createPipline(){
    
    assert(bradEngineSwapChain != nullptr&&"cant create pipline befor swap chain");
    assert(pipelineLayout != nullptr && "cant create pipline befor pipeline layout");
    Pipeline_configInfo pipelineConfig{};
    V_Pipeline::defaultPiplineConfigInfo(pipelineConfig );
    pipelineConfig.renderPass = bradEngineSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<V_Pipeline>(bradEngineDevice, "shaders/Vertex_shader.vert.spv", "shaders/Frag_shader.frag.spv" , pipelineConfig);
    
    
}

void App::recreateSwapChain(){
    auto extent = vulkWindow.getScaleWindow();
    
    while (extent.width ==0 || extent.height == 0){
        extent = vulkWindow.getScaleWindow();
    }
    
    vkDeviceWaitIdle(bradEngineDevice.device());
    if(bradEngineSwapChain == nullptr){
    bradEngineSwapChain = std::make_unique<BradEngineSwapChain>(bradEngineDevice,extent);
    }else{
        bradEngineSwapChain = std::make_unique<BradEngineSwapChain>(bradEngineDevice,extent,std::move(bradEngineSwapChain));
    }
    if(bradEngineSwapChain->imageCount() != commandBuffers.size()){
        freeCommandBuffer();
        createCommandBuffers();
    }
    createPipline();
    
}

void App::createCommandBuffers(){
    commandBuffers.resize(bradEngineSwapChain->imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = bradEngineDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    if(vkAllocateCommandBuffers(bradEngineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("fail to create the command buffer");
    }

}

void App::freeCommandBuffer(){
    vkFreeCommandBuffers(bradEngineDevice.device(), bradEngineDevice.getCommandPool(),
                         static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    
    commandBuffers.clear();
}

void App::recordCommandBuffer(int imageIndex){
    
    static int frame= 0;
    
    frame = (frame+1) %150;
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
        throw  std::runtime_error("failed to begin recording commad buffer");
        
    }
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = bradEngineSwapChain->getRenderPass();
    renderPassInfo.framebuffer = bradEngineSwapChain->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = bradEngineSwapChain->getSwapChainExtent();
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f,0.01f,0.01f,0.1f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    
    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(bradEngineSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(bradEngineSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0,0}, bradEngineSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);
    
    pipeline->bind(commandBuffers[imageIndex]);
    bradModel->bind(commandBuffers[imageIndex]);
    
    for(int j =0; j<4;j++){
        SimplePushConstantData push{};
        push.offset = {-0.5f+frame *0.02, -0.4f + j * 0.25f};
        push.color = {0.0f,0.0f,0.2f+0.2f*j};
        
        vkCmdPushConstants(commandBuffers[imageIndex],
                           pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0, sizeof(SimplePushConstantData), &push);
        bradModel->draw(commandBuffers[imageIndex]);
    }
    
    
    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS ){
        throw std::runtime_error("failed to record commad buffer");
    }
}

void App::drawFrame(){
    uint32_t image_Index;
    auto result = bradEngineSwapChain->acquireNextImage(&image_Index); // bradcom : fetches the index of the frame wich should render, automaticly handle the gpu and cpu synchro (double or tripple buffering)
    
    if(result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return;
    }
    if(result !=VK_SUCCESS && result!= VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("failed to acquire swap chain image");
        
    }
    
    recordCommandBuffer(image_Index);
    result = bradEngineSwapChain->submitCommandBuffers(&commandBuffers[image_Index], &image_Index); // bradcom : submit the provided command buffer to our device graphics queue while handling th cpu and gpu syncro
    if( result == VK_ERROR_OUT_OF_DATE_KHR || result ==  VK_SUBOPTIMAL_KHR || vulkWindow.isWindowResized()){
        vulkWindow.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if(result !=VK_SUCCESS){
        throw std::runtime_error("failed to present swap chain image");
        
    }
    
}
}
 
