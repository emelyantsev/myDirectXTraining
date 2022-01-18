#pragma once

#include "d3dApp.h"

class InitD3DApp : public D3DApp
{

public:
	InitD3DApp();
	~InitD3DApp();

private:
    
    virtual void Update() override;
    virtual void Draw() override;

};