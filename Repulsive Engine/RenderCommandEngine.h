#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include<DirectXColors.h>
#include"RenderDevice.h"
#include<memory>
#include"StencilBuffer.h"

#pragma comment(lib,"d3d11.lib")

class RenderCommandEngine
{
	template<typename ObjectT>
	using ObjectManager = Microsoft::WRL::ComPtr<ObjectT>;
public:
	virtual void SetContextData(void* data) = 0;
public:
	virtual void SetStencilBuffer(StencilBuffer& buffer) = 0;
	virtual void ClearStencilBuffer(StencilBuffer& buffer) = 0;
public:
	virtual void RemoveStencilBuffer() = 0;
	virtual void EndStencilClipping(unsigned int ref_value) = 0;
	virtual void BeginStencilClipping(unsigned int ref_value) = 0;
public:
	virtual void Draw() = 0;
public:
};