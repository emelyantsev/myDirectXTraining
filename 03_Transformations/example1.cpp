#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
#include <cstdlib>

std::ostream& XM_CALLCONV operator << (std::ostream& os, DirectX::FXMVECTOR v)
{
    DirectX::XMFLOAT3 dest;
    DirectX::XMStoreFloat3(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
    return os;
}

namespace DX = DirectX;

std::ostream& XM_CALLCONV operator << (std::ostream& os, DirectX::FXMMATRIX m)
{
    for (int i = 0; i < 4; ++i)
    {
        os << DirectX::XMVectorGetX(m.r[i]) << "\t";
        os << DirectX::XMVectorGetY(m.r[i]) << "\t";
        os << DirectX::XMVectorGetZ(m.r[i]) << "\t";
        os << DirectX::XMVectorGetW(m.r[i]);
        os << std::endl;
    }
    return os;
}

int main() {


    DX::XMVECTOR axis = DX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);


    auto M = DX::XMMatrixRotationAxis( axis , DX::XM_PIDIV2 ) ;


    DX::XMVECTOR p = DX::XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f);


    DX::XMVECTOR rp = DX::XMVector2TransformCoord( p, M )  ;


    std::cout << p << std::endl;

    std::cout << rp << std::endl;
    


    

    return EXIT_SUCCESS;
}