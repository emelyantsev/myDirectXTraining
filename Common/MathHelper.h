#pragma once

#include "common.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <cstdint>

namespace MathHelper {

	// Returns random float in [0, 1).
	float RandF() ;

	// Returns random float in [a, b).
	float RandF(float a, float b) ;

    int Rand(int a, int b) ;

	template<typename T>
	T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	 
	template<typename T>
	T Lerp(const T& a, const T& b, float t)
	{
		return a + (b-a)*t;
	}

	template<typename T>
	T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x); 
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	float AngleFromXY(float x, float y);

	DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi) ;

    DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M) ;

    DirectX::XMFLOAT4X4 Identity4x4() ;

    DirectX::XMVECTOR RandUnitVec3() ;
    DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n) ;

	const float Infinity = FLT_MAX;
    const float Pi = 3.1415926535f;

};

