//
//  BradModel.hpp
//  GameEngine
//
//  Created by Raphael Braud-Mussi on 20/11/2021.
//

#ifndef BradModel_hpp
#define BradModel_hpp

#include <stdio.h>
#include "BradEngineDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>



namespace vulkbrad {
class BradModel{ //  Bradcom : able to take vertex data created by or right in a file on the cpu and the allocate the memory and copy the data over to our device cpu so it can be rendered efficientle
    
    public :
    struct Vertex{
        glm::vec2 position;
        
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributDescriptions();
    };
    BradModel(BradEngineDevice &device, const std::vector<Vertex> &vertices);
    ~BradModel();
    BradModel(const BradModel &) = delete;
    BradModel &operator = (const BradModel &) =delete;
    
    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);
    
    
    private :
    
    void createVertexBuffer(const std::vector<Vertex> &vertices);
    
    BradEngineDevice &BradDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;
    
    
    
};
}
#endif /* BradModel_hpp */
