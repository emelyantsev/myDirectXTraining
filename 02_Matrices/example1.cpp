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


    DX::XMMATRIX A(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 4.0f, 0.0f,
        1.0f, 2.0f, 3.0f, 1.0f);

    std::cout << A << std::endl;


    DX::XMMATRIX B = DX::XMMatrixIdentity();

    std::cout << B << std::endl;


    DX::XMMATRIX C = A * B;
    std::cout << C << std::endl;


    DX::XMMATRIX D = DX::XMMatrixTranspose(A);

    std::cout << D << std::endl;


    DX::XMVECTOR det = DX::XMMatrixDeterminant(A);

    std::cout << det << std::endl;

    std::cout << DX::XMVectorGetX(det) << std::endl;


    DX::XMVECTOR detA = DX::XMMatrixDeterminant(A);

    DX::XMMATRIX E = DX::XMMatrixInverse(&detA, A);

    std::cout << E << std::endl;

    std::cout << detA << std::endl;


    DX::XMVECTOR detE = DX::XMMatrixDeterminant(E);

    std::cout << detE << std::endl;

    std::cout << DX::XMVectorGetX(detE) << std::endl;


    std::cout << std::boolalpha << ( DX::XMVectorGetX(det) == ( 1 / DX::XMVectorGetX(detE) ) ) << std::endl;


    std::cout << A * E << std::endl;


    std::cout << DX::XMMatrixIsIdentity( A*E ) << std::endl; 


    std::cout << 2.0f * A << std::endl;

    return EXIT_SUCCESS;
}