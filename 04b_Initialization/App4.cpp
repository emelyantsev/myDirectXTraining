#include "InitD3dApp.h"

int main() {

    InitD3DApp app;

    try {
        
        if (!app.Initialize() ) {

            std::cerr << "Failed to initialize" << std::endl;
            return EXIT_FAILURE;
        } 
        else {

            std::cerr << "Initialized OK" << std::endl;
        }

        app.Run() ;
    }
    catch (DxException& e)
    {
        ::MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}