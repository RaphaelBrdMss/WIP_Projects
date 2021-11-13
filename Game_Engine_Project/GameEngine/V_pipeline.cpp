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


namespace Vwin {

V_Pipline::V_Pipline(const std::string& vertPath, const std::string& fragPath){
    initGraphicsPipline(vertPath, fragPath);
}


std::vector<char> V_Pipline::fileReader(const std::string& filepath){
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

void V_Pipline::initGraphicsPipline (const std::string& vertPath, const std::string& fragPath){
    
    auto vertexCode = fileReader(vertPath);
    auto fragmentCode = fileReader(fragPath);
    
    std::cout << "taille du vertex shader: "<< vertexCode.size() <<'\n';
    std::cout << "taille du Fragment shader: "<< fragmentCode.size() <<'\n';
    
}
}
