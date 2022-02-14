// Console application for vector addition using compute shader on GPU

#include "VecAddCSApp.h"

int main() {

    VecAddCSApp app;

    try {
        
        if ( !app.Initialize() ) {

            std::cerr << "Failed to initialize" << std::endl;
            return EXIT_FAILURE;
        } 

        app.DoComputeWork();

        return EXIT_SUCCESS ;
    }
    catch ( DxException& e )
    {
        std::wcerr << e.ToString() << std::endl;
        return EXIT_FAILURE;
    }
    
}