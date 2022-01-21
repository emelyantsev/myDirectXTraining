#include "MathHelper.h"

#include <float.h>
#include <cmath>

float MathHelper::RandF() {

    return (float) (rand() ) / (float) RAND_MAX;
}

// Returns random float in [a, b).
float MathHelper::RandF(float a, float b) {
    
    return a + RandF()*(b-a);
}

int MathHelper::Rand(int a, int b) {

    return a + rand() % ((b - a) + 1);
}


float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;
 
	// Quadrant I or IV
	if (x >= 0.0f) 
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if(theta < 0.0f)
			theta += 2.0f*Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else {      
	
    	theta = atanf(y/x) + Pi; // in [0, 2*pi).
    }

	return theta;
}

DirectX::XMVECTOR MathHelper::SphericalToCartesian(float radius, float theta, float phi)
{
    return DirectX::XMVectorSet(
        radius*sinf(phi)*cosf(theta),
        radius*cosf(phi),
        radius*sinf(phi)*sinf(theta),
        1.0f);
}

DirectX::XMMATRIX MathHelper::InverseTranspose(DirectX::CXMMATRIX M)
{
    // Inverse-transpose is just applied to normals.  So zero out 
    // translation row so that it doesn't get into our inverse-transpose
    // calculation--we don't want the inverse-transpose of the translation.
    DirectX::XMMATRIX A = M;
    A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
    return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
}

DirectX::XMFLOAT4X4 MathHelper::Identity4x4()
{
    DirectX::XMFLOAT4X4 I(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    return I;
}


DirectX::XMVECTOR MathHelper::RandUnitVec3()
{
	DirectX::XMVECTOR One  = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		DirectX::XMVECTOR v = DirectX::XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if( DirectX::XMVector3Greater( DirectX::XMVector3LengthSq(v), One) )
			continue;

		return DirectX::XMVector3Normalize(v);
	}
}

DirectX::XMVECTOR MathHelper::RandHemisphereUnitVec3(DirectX::XMVECTOR n)
{
	DirectX::XMVECTOR One  = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while (true)
	{
		// Generate random point in the cube [-1,1]^3.
		DirectX::XMVECTOR v = DirectX::XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.
		
		if( DirectX::XMVector3Greater( DirectX::XMVector3LengthSq(v), One) )
			continue;

		// Ignore points in the bottom hemisphere.
		if( DirectX::XMVector3Less( DirectX::XMVector3Dot(n, v), Zero ) )
			continue;

		return DirectX::XMVector3Normalize(v);
	}
}