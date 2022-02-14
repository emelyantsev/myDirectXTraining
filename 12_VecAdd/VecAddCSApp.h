#pragma once

#include "UploadBuffer.h"
#include "d3dUtils.h"
#include "common.h"


struct Data
{
	DirectX::XMFLOAT3 v1;
	DirectX::XMFLOAT2 v2;
};


class VecAddCSApp {

public:

    VecAddCSApp();

    VecAddCSApp(const VecAddCSApp& rhs) = delete;
    VecAddCSApp& operator=(const VecAddCSApp& rhs) = delete;

    ~VecAddCSApp();

public:

	bool Initialize();
	bool InitializeDirect3D();
	
	void DoComputeWork();

	void CreateCommandObjects();
	void FlushCommandQueue();

	void BuildBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSOs();	

private:

    UINT mCbvSrvDescriptorSize = 0;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
 
	const int NumDataElements = 50;

	Microsoft::WRL::ComPtr<ID3D12Resource> mInputBufferA = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mInputUploadBufferA = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mInputBufferB = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mInputUploadBufferB = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mOutputBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mReadBackBuffer = nullptr;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;
	
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;
};