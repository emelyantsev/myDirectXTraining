
#include <cstdlib>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <iostream>

using namespace DirectX; // for operator +

std::ostream& XM_CALLCONV operator << (std::ostream& os, DirectX::FXMVECTOR v)
{
    DirectX::XMFLOAT3 dest;
    DirectX::XMStoreFloat3(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
    return os;
}


int main() {

    DirectX::XMVECTOR u = DirectX::XMVectorSet(8.0f, 4.0f, 5.0f, 0.0f);
    DirectX::XMVECTOR v = DirectX::XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
    

    DirectX::XMVECTOR a = u + v ;    

    std::cout << a << std::endl;
    

    DirectX::XMVECTOR b = u - v ;        
    std::cout << b << std::endl;


    DirectX::XMVECTOR c = 10.0f * u;

    std::cout << c << std::endl;


    auto L = DirectX::XMVector3Length( u ) ;

    std::cout << L << std::endl;
    std::cout << DirectX::XMVectorGetX( L ) << std::endl;



    auto an = DirectX::XMVector3Normalize( u ) ;

    std::cout << an << std::endl;


    auto lan = DirectX::XMVector3Length( an ) ;

    std::cout << lan << std::endl;
    std::cout << DirectX::XMVectorGetX( lan ) << std::endl;



    auto uvdot = DirectX::XMVector3Dot( u, v) ;

    std::cout << uvdot << std::endl;
    std::cout << DirectX::XMVectorGetX( uvdot ) << std::endl;



    DirectX::XMVECTOR e1 = DirectX::XMVector3Cross( u, v) ;
    DirectX::XMVECTOR e2 = DirectX::XMVector3Cross( v, u) ;

    std::cout << e1 << " " << e2 << std::endl;

    auto z = DirectX::XMVectorZero();

    std::cout.setf( std::ios_base::boolalpha) ;


    std::cout << DirectX::XMVector3Equal( e1 + e2, z ) << " " << DirectX::XMVector3NotEqual( e1 + e2, z ) << std::endl ; 
    
    
    DirectX::XMVECTOR n = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    
    DirectX::XMVECTOR projW;
    DirectX::XMVECTOR perpW;
    
    DirectX::XMVector3ComponentsFromNormal(&projW, &perpW, u, n);


    std::cout << projW << " " << perpW << std::endl;

    
    DirectX::XMVECTOR angleVec = DirectX::XMVector3AngleBetweenVectors( projW, perpW ) ;

    std::cout << angleVec << std::endl;

    float angleRadians = DirectX::XMVectorGetX( angleVec )  ;

    std::cout << angleRadians << std::endl;

    float angleDegrees = DirectX::XMConvertToDegrees( angleRadians ) ;
    std::cout << angleDegrees << std::endl;


    return EXIT_SUCCESS;
}