
#include "app.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
using namespace vulkbrad;
int main(){
    vulkbrad::App testapp{};
    try{
        testapp.run();
    }catch (const std::exception &e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
    
}


