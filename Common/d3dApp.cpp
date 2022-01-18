#include  "d3dApp.h"

D3DApp::D3DApp() {}

D3DApp::~D3DApp() {


    if( md3dDevice != nullptr ) {
	
    	FlushCommandQueue();
    }
}


bool D3DApp::Initialize() {

    if( !InitMainWindow() ) {
	
    	return false;
    }

    if( !InitDirect3D() ) {
	
    	return false;
    }

    OnResize();

    return true;
}


int D3DApp::Run() {

    MSG msg = { 0 };
 
	while (msg.message != WM_QUIT) 
	{
		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ) {

            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}		
		else {	

			Update();	
            Draw();
        }
    }

	return (int) msg.wParam;
}


LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch( msg )     {


        case WM_ACTIVATE:

            if( LOWORD(wParam) == WA_INACTIVE )
            {
                mAppPaused = true;
            }
            else
            {
                mAppPaused = false;
                
            }
            return 0;

        case WM_SIZE:
            
            mClientWidth  = LOWORD(lParam);
            mClientHeight = HIWORD(lParam);

            if( md3dDevice )
            {
                if( wParam == SIZE_MINIMIZED )
                {
                    mAppPaused = true;
                    mMinimized = true;
                    mMaximized = false;
                }
                else if( wParam == SIZE_MAXIMIZED )
                {
                    mAppPaused = false;
                    mMinimized = false;
                    mMaximized = true;
                    OnResize();
                }
                else if( wParam == SIZE_RESTORED )
                {
                             
                    if( mMinimized )
                    {
                        mAppPaused = false;
                        mMinimized = false;
                        OnResize();
                    }
                 
                    else if( mMaximized )
                    {
                        mAppPaused = false;
                        mMaximized = false;
                        OnResize();
                    }
                    else if( mResizing )
                    {
                    }
                    else 
                    {
                        OnResize();
                    }
                }
            }
            return 0;

        case WM_ENTERSIZEMOVE:
            mAppPaused = true;
            mResizing  = true;
            return 0;

        case WM_EXITSIZEMOVE:
            mAppPaused = false;
            mResizing  = false;
            OnResize();
            return 0;

        case WM_DESTROY:

            PostQuitMessage( 0 );
            return 0;

        default:
            return DefWindowProc( hwnd, msg, wParam, lParam );
    }
    return TRUE;

}

LRESULT CALLBACK D3DApp::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
    D3DApp* pThis;

    if ( msg == WM_NCCREATE ) {

        CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
        pThis = (D3DApp*) pCreate->lpCreateParams;

        SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
    }   
    else {

        pThis = (D3DApp*) GetWindowLongPtr( hwnd, GWLP_USERDATA );
    }

    if ( pThis ) {

        return pThis->MsgProc(hwnd, msg, wParam, lParam );
    } 
    else {

        return DefWindowProc( hwnd, msg, wParam, lParam );
    }

}

bool D3DApp::InitMainWindow() {

    WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = D3DApp::MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhAppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) ::GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"MainWnd";


	if( !::RegisterClass(&wc) )
	{
		::MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	RECT R = { 0, 0, mClientWidth, mClientHeight };
    ::AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = ::CreateWindow(L"MainWnd", mMainWndCaption.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, this); 
	
    if( !mhMainWnd ) 	{

		::MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	} 
    
	::ShowWindow(mhMainWnd, SW_SHOW);
	::UpdateWindow(mhMainWnd);

	return true;
}

bool D3DApp::InitDirect3D() {

    // Create Factory

    HRESULT result = ::CreateDXGIFactory1( __uuidof(**( &mdxgiFactory) ) , IID_PPV_ARGS_Helper( &mdxgiFactory ) ) ;

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }


    // Create Device

    HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS( &md3dDevice ) 
    );

    if ( FAILED(hardwareResult) ) {

        exit( EXIT_FAILURE );
    }

    // Create Fence

    result = md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence)) ;

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }

    // Get Descriptors size

    mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Check multisampling support

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;


    result = md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)) ;

    m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");


    CreateCommandObjects();
    CreateSwapChain();
    CreateRtvAndDsvDescriptorHeaps();

    return true;
}



void D3DApp::CreateRtvAndDsvDescriptorHeaps() {

    HRESULT result;

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
    result = md3dDevice->CreateDescriptorHeap(
        &rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf()));

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
    result = md3dDevice->CreateDescriptorHeap(
        &dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf()));

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }
}



void D3DApp::OnResize()
{

    std::cout << "On resize" << std::endl;

	assert(md3dDevice);
	assert(mSwapChain);
    assert(mDirectCmdListAlloc);

	FlushCommandQueue();

    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	
	for (int i = 0; i < SwapChainBufferCount; ++i)
		mSwapChainBuffer[i].Reset();

    mDepthStencilBuffer.Reset();
	
	
    ThrowIfFailed(mSwapChain->ResizeBuffers(
		SwapChainBufferCount, 
		mClientWidth, mClientHeight, 
		mBackBufferFormat, 
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	mCurrBackBuffer = 0;
 
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT i = 0; i < SwapChainBufferCount; i++) 	{

		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;

 
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

    depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = mDepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;

    ThrowIfFailed(md3dDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

    
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
    md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());


	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	

    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };

    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	
	FlushCommandQueue();

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast<float>(mClientWidth);
	mScreenViewport.Height   = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

    mScissorRect = { 0, 0, mClientWidth, mClientHeight };
}


void D3DApp::CreateCommandObjects() {

    HRESULT result;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	result = md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)) ;

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }

	result = md3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf()) ) ;

    
    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }

	result = md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(), 
		nullptr,                   
		IID_PPV_ARGS(mCommandList.GetAddressOf()));

	
    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }

	mCommandList->Close();

}

void D3DApp::CreateSwapChain() {

    HRESULT result;

    mSwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = mClientWidth;
    sd.BufferDesc.Height = mClientHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = mBackBufferFormat;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SwapChainBufferCount;
    sd.OutputWindow = mhMainWnd;
    sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	
    result = mdxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd, 
		mSwapChain.GetAddressOf()) ;

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }

}

void D3DApp::FlushCommandQueue() {

    HRESULT result ;

    mCurrentFence++;

    result = mCommandQueue->Signal(mFence.Get(), mCurrentFence);

    if (result != S_OK ) {

        exit( EXIT_FAILURE );
    }
	
    if(mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

        
        result = mFence->SetEventOnCompletion(mCurrentFence, eventHandle);

        if (result != S_OK ) {

            exit( EXIT_FAILURE );
        }

		WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
	}

}


ID3D12Resource* D3DApp::CurrentBackBuffer()const
{
	return mSwapChainBuffer[mCurrBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView()const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView()const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}