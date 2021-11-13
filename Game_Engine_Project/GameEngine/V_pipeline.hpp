//
//  pipeline.hpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 12/11/2021.
//

#ifndef pipeline_hpp
#define pipeline_hpp

#include <stdio.h>
#pragma once
#include <string>

namespace Vwin {
class V_Pipline{
    public :
    V_Pipline(const std::string& vertPath, const std::string& fragPath);
    
    private :
    static std::vector<char> fileReader(const std::string& filepath);
    
    void initGraphicsPipline (const std::string& vertPath, const std::string& fragPath);
};
}
#endif /* pipeline_hpp */
