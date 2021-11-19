//
//  pipeline.cpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#include "V_pipeline.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cassert>


namespace vulkbrad {

V_Pipeline::V_Pipeline(class BradEngineDevice& device ,
                     const std::string& vertPath,
                     const std::string& fragPath,
                     const Pipeline_configInfo &configInfo) : BradEngineDevice{device}  {
    initGraphicsPipline(vertPath, fragPath, configInfo);
}


V_Pipeline::~V_Pipeline(){
    vkDestroyShaderModule(BradEngineDevice.device(), vertexShaderModule, nullptr); 
    vkDestroyShaderModule(BradEngineDevice.device(), fragmentShaderModule, nullptr);
    vkDestroyPipeline(BradEngineDevice.device(), graphicsPipe, nullptr);
}

std::vector<char> V_Pipeline::fileReader(const std::string& filepath){
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};
    
    if(!file.is_open()){
        throw std::runtime_error("fail to open the file " +filepath);
    }
    
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
    
}

void V_Pipeline::initGraphicsPipline (const std::string& vertPath, const std::string& fragPath,const Pipeline_configInfo &configInfo){
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "no pipelinelayout provided in configinfo");  
    assert(configInfo.renderPass != VK_NULL_HANDLE && "no pipelinelayout provided in configinfo");
    auto vertexCode = fileReader(vertPath);
    auto fragmentCode = fileReader(fragPath);
    
    creatShaderModule(vertexCode, &vertexShaderModule);
    creatShaderModule(fragmentCode, &fragmentShaderModule);
    
    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragmentShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;
    
    
    VkPipelineVertexInputStateCreateInfo vertex_InputInfo{};
    vertex_InputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_InputInfo.vertexBindingDescriptionCount =0;
    vertex_InputInfo.vertexAttributeDescriptionCount =0;
    vertex_InputInfo.pVertexAttributeDescriptions= nullptr;
    vertex_InputInfo.pVertexBindingDescriptions  = nullptr;
    

    
    
    VkGraphicsPipelineCreateInfo pipeline_Info{};
    pipeline_Info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_Info.stageCount = 2;
    pipeline_Info.pStages= shaderStages;
    pipeline_Info.pVertexInputState = &vertex_InputInfo;
    pipeline_Info.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipeline_Info.pViewportState = &configInfo.viewportInfo;
    pipeline_Info.pRasterizationState = &configInfo.rasterizationInfo;
    pipeline_Info.pMultisampleState = &configInfo.multisampleInfo;
    pipeline_Info.pColorBlendState = &configInfo.colorBlendInfo;
    pipeline_Info.pDepthStencilState = &configInfo.depthStencilInfo;
    pipeline_Info.pDynamicState =nullptr;
    
    pipeline_Info.layout= configInfo.pipelineLayout;
    pipeline_Info.renderPass = configInfo.renderPass;
    pipeline_Info.subpass = configInfo.subpass;
    
    
    pipeline_Info.basePipelineIndex = -1;
    pipeline_Info.basePipelineHandle = VK_NULL_HANDLE;
    
    if(vkCreateGraphicsPipelines(BradEngineDevice.device(), VK_NULL_HANDLE, 1, &pipeline_Info, nullptr, &graphicsPipe) != VK_SUCCESS){
        throw std::runtime_error("failed to create graphics pipe");
    }
    
    
    
    
    
    
    
    
}
void V_Pipeline::creatShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    if(vkCreateShaderModule( BradEngineDevice.device(),  &createInfo, nullptr, shaderModule) != VK_SUCCESS){
        throw std::runtime_error("failed to create shader module"); 
    }
    
}

void V_Pipeline::bind(VkCommandBuffer commandBuffer){
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipe);
}
//for more details youtube : Fixed function Pipeline Stages - Vulkan Game Engine Tutorial
void V_Pipeline::defaultPiplineConfigInfo(Pipeline_configInfo& configInfo,uint32_t width, uint32_t height ){
    
    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
    //view port describes our transformation betwin our pipeline output and our target image
    
    configInfo.viewport.x = 0.0f;
    
    configInfo.viewport.y = 0.0f;
    configInfo.viewport.width = static_cast<float>(width);
    configInfo.viewport.height = static_cast<float>(height);
    configInfo.viewport.minDepth = 0.0f;
    configInfo.viewport.maxDepth = 1.0f;
   
    configInfo.scissor.offset = {0, 0};
    configInfo.scissor.extent = {width, height};
    
 
    
    configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    configInfo.viewportInfo.viewportCount = 1;
    configInfo.viewportInfo.pViewports = &configInfo.viewport;
    configInfo.viewportInfo.scissorCount = 1;
    configInfo.viewportInfo.pScissors = &configInfo.scissor;
    
    //rasterization breaksup our geometry into fragment for each pixels or triangle overlaps
    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // order to travel all over our vertices and to know if we are looking on the front or back side of our triangle
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;
   
    
    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;
    configInfo.multisampleInfo.pSampleMask = nullptr;
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;
    
    
    configInfo.colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
   
    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional
    
    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {};  // Optional
    configInfo.depthStencilInfo.back = {};   // Optional
    
    

}
}
