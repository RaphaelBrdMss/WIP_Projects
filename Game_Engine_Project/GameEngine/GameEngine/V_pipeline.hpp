//
//  pipeline.hpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#ifndef pipeline_hpp
#define pipeline_hpp
#include "BradEngineDevice.hpp"
#include <stdio.h>
//#include "app.hpp"
#pragma once
#include <string>

namespace vulkbrad {

struct Pipeline_configInfo{
    Pipeline_configInfo(const Pipeline_configInfo&)=delete;
    Pipeline_configInfo operator=(const Pipeline_configInfo&) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicsStateEnable;
    VkPipelineDynamicStateCreateInfo dynamicsSrtateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class V_Pipeline{
    public :
    V_Pipeline( BradEngineDevice &device ,
              const std::string& vertPath,
              const std::string& fragPath,
              const Pipeline_configInfo &configInfo);
    
    ~V_Pipeline();
    
    V_Pipeline(const V_Pipeline&) = delete;
    V_Pipeline& operator = (const V_Pipeline&) = delete;
    
    void bind(VkCommandBuffer commandBuffer);
    
    static void defaultPiplineConfigInfo(Pipeline_configInfo& configInfo);
    
    private :
    static std::vector<char> fileReader(const std::string& filepath);
    
    void initGraphicsPipline (const std::string& vertPath,
                              const std::string& fragPath,
                              const Pipeline_configInfo &configInfo);
    
    void creatShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    
    BradEngineDevice& BradEngineDevice;
    VkPipeline graphicsPipe;
    VkShaderModule vertexShaderModule;
    VkShaderModule fragmentShaderModule;
};
}
#endif /* pipeline_hpp */
