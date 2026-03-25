#include "App.h"
#include <iostream>
#include <exception>

int main() 
{
try 
    {
        std::cout << "Starting AxonEngine..." << std::endl;
        Axn::App app;
        app.Run();
    }
    catch (const std::exception& e) 
    {
        std::cerr << "\n========================================" << std::endl;
        std::cerr << "[FATAL ERROR]: " << e.what() << std::endl;
        std::cerr << "========================================\n" << std::endl;
        
        #ifdef _WIN32
        std::cout << "Press any key to exit..." << std::endl;
        system("pause"); 
        #endif
        
        return -1;
    }

    return 0;
}