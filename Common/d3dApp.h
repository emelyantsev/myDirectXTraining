#pragma once

#include "common.h"
#include "d3dUtils.h"
#include "GameTimer.h"

class D3DApp {

    public:

        D3DApp();

        D3DApp(const D3DApp& rhs) = delete;
        D3DApp& operator=(const D3DApp& rhs) = delete;

        virtual ~D3DApp();

    public:

        HWND MainWnd() const;
	    float AspectRatio() const;

        bool Get4xMsaaState() const;
        void Set4xMsaaState(bool value);

        virtual bool Initialize();
        int Run();

    protected:

        virtual void Update() = 0;
        virtual void Draw() = 0;

        virtual void OnResize(); 

        virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	    virtual void OnMouseUp(WPARAM btnState, int x, int y)  {}
	    virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

        virtual void OnChangeFullScreenMode();


    protected:

        bool InitMainWindow();
	    bool InitDirect3D();

        static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) ;
        LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) ;


        virtual void CreateRtvAndDsvDescriptorHeaps();
        void CreateCommandObjects();
        void CreateSwapChain();

        void FlushCommandQueue();

        ID3D12Resource* CurrentBackBuffer() const;
	    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

        void CalculateFrameStats();

    protected:

        HINSTANCE mhAppInst = nullptr;
        HWND mhMainWnd = nullptr;

        bool mAppPaused = false; 
        bool mMinimized = false;  
        bool mMaximized = false;  
        bool mResizing = false;   
        bool mFullscreenState = false;

        bool m4xMsaaState = false;
        UINT m4xMsaaQuality = 0; 

        GameTimer mTimer;

        Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
        Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
        Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

        Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
        UINT64 mCurrentFence = 0;
        
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

        static const int SwapChainBufferCount = 2;
        int mCurrBackBuffer = 0;
        Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
        Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

        D3D12_VIEWPORT mScreenViewport; 
        D3D12_RECT mScissorRect;

        UINT mRtvDescriptorSize = 0;
        UINT mDsvDescriptorSize = 0;
        UINT mCbvSrvUavDescriptorSize = 0;

        std::wstring mMainWndCaption = L"d3d App";

        D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
        DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        int mClientWidth = 800;
	    int mClientHeight = 600;

        int cxClient, cyClient;

        bool isFullScreen = false;
        int prevStyle, prevExtStyle;

        RECT rect;
        std::string dimStr;
}; 