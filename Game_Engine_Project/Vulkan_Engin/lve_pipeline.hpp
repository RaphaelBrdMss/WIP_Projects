//
//  lve_pipeline.hpp
//  Vulkan_Engin
//
//  Created by Anna Diani on 15/11/2021.
//

#ifndef lve_pipeline_hpp
#define lve_pipeline_hpp

#include "lve_device.hpp"

#include <stdio.h>
#include <string>
#include <vector>

#pragma once

namespace lve {
struct PipelineConfigInfo{};
class LvePipeline {
    public :
    LvePipeline(LveDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
    
    ~LvePipeline() {}
    
    LvePipeline(const LvePipeline&) = delete;
    void operator=(const LvePipeline&) = delete;
    
    static PipelineConfigInfo defaultPipelineConfigIndo(uint32_t width, uint32_t height);
    
private:
    static std::vector<char> readFile(const std::string& filepath);
    
    void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath,const PipelineConfigInfo& configInfo );
    
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    
    LveDevice& lveDevice;
    VkPipeline graphicePipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
                };
}

#endif /* lve_pipeline_hpp */
