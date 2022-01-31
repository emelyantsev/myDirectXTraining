#include "LitColumnsApp.h"


#if defined(SUBSYSTEM_WINDOWS)
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd) {
#else
    int main() {
#endif

    LitColumnsApp app;

    try {
        
        if ( !app.Initialize() ) {

            std::cerr << "Failed to initialize" << std::endl;
            return EXIT_FAILURE;
        } 
        else {

            std::cerr << "Initialized OK" << std::endl;
        }

        return app.Run() ;
    }
    catch ( DxException& e )
    {
        
        ::MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return EXIT_FAILURE;
    }

}