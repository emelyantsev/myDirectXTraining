#pragma once

#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <comdef.h>

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>


#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include "d3dx12.h"
#include "DDSTextureLoader.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")