#include "Engine.h"
int main() 
{
    Engine engine;
    if(!engine.initialize())
        return 0;
    
    return engine.run();
}

