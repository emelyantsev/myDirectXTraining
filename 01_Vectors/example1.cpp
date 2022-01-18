
#include <cstdlib>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <iostream>


std::ostream& XM_CALLCONV operator << (std::ostream& os, DirectX::FXMVECTOR v)
{
    DirectX::XMFLOAT3 dest;
    DirectX::XMStoreFloat3(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
    return os;
}

namespace dx = DirectX;

int main() {


    DirectX::XMVECTOR v = DirectX::XMVectorSet(8.0f, 4.0f, 5.0f, 0.0f);
    std::cout << v << std::endl;


    auto z = dx::XMVectorZero();

    std::cout << z << std::endl;



    DirectX::XMVECTOR eye = dx::XMVectorSplatOne() ;

    std::cout << eye << std::endl;


    DirectX::XMVECTOR replicated = dx::XMVectorReplicate( -17.3f) ;

    std::cout << replicated << std::endl;


    dx::XMVECTOR w = dx::XMVectorSplatZ( v )  ;

    std::cout << w << std::endl;

    return EXIT_SUCCESS;
}