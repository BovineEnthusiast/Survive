#include "Engine.h"
#include <iostream>
int main() 
{
    Engine engine;
    if(!engine.initialize())
    {
        std::cout << "failed to init";
        return 0;
    }
    return engine.run();
}

