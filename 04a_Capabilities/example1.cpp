// Enumerate adapters and display modes

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include <wrl.h>
#include <dxgi1_4.h>


#pragma comment(lib, "dxgi.lib")

void LogAdapterOutputs(IDXGIAdapter* adapter) ;
void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format) ;

int main() {


    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;

    HRESULT result;

    result = ::CreateDXGIFactory1( __uuidof(**( &mdxgiFactory) ) , IID_PPV_ARGS_Helper( &mdxgiFactory ) ) ;

    if (result != S_OK ) {

        return EXIT_FAILURE;
    }


    UINT i = 0;
    IDXGIAdapter* adapter = nullptr;

    std::vector<IDXGIAdapter*> adapterList;
    
    while ( mdxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND) {

        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description;
        text += L"***" ; 

        std::wcout << text << L" " << desc.DedicatedVideoMemory << std::endl;

        adapterList.push_back(adapter);
        
        ++i;
    }

    for(size_t i = 0; i < adapterList.size(); ++i)
    {
        LogAdapterOutputs(adapterList[i]) ;

        if (adapterList[i] != nullptr) {
            adapterList[i]->Release();
        }
    }


    return EXIT_SUCCESS;
}


void LogAdapterOutputs(IDXGIAdapter* adapter)
{
    UINT i = 0;
    IDXGIOutput* output = nullptr;

    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    while(adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);
        
        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        
        RECT desktopCoords = desc.DesktopCoordinates ;

        std::wcout << text << " " << desktopCoords.bottom << " " << desktopCoords.left << " " 
            << desktopCoords.top << " " << desktopCoords.right <<  std::endl;

        // LogOutputDisplayModes(output, mBackBufferFormat);

        if (output != nullptr) {
            output->Release();
        }

        ++i;
    }
}

void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
    UINT count = 0;
    UINT flags = 0;

    output->GetDisplayModeList(format, flags, &count, nullptr);

    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format, flags, &count, &modeList[0]);

    for(auto& x : modeList)
    {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text =
            L"Width = " + std::to_wstring(x.Width) + L" " +
            L"Height = " + std::to_wstring(x.Height) + L" " +
            L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
            L"\n";

        std::wcout << text << std::endl;
    }
}