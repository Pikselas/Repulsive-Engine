#pragma once
#include<d3d11.h>
#include<wrl.h>

#include<span>
class Sprite
{
private:
	struct VertexType
	{
		float x, y;
		float u, v;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
public:
	struct Points
	{
		int left, right, top, bottom;
	};
	Sprite(ID3D11Device* graphics_device , Points points , float winWidth , float winHeight)
	{

		const float right = (winWidth / winHeight) * (points.right / winWidth);
		const float bottom = (winWidth / winHeight) * (points.bottom / winHeight);

		const float left = (winWidth / winHeight) * (points.left / winWidth);
		const float top = (winWidth / winHeight) * (points.top / winHeight);

		const float x_ = 0.5f * (right - left);
		const float y_ = 0.5f * (bottom - top);

		//constexpr float x_ = 0.5f * (800.0f / 600.0f) * (400.0f / 800.0f);
		//constexpr float y_ = 0.5f * (800.0f / 600.0f) * (400.0f / 600.0f);

		const VertexType Vertices[] = 
		{
			{ -x_ , y_ , 0.0f , 0.0f},
			{ x_ , y_  , 1.0f , 0.0f},
			{-x_ , -y_  , 0.0f , 1.0f},
			{ x_ , -y_  , 1.0f , 1.0f},
		};
		constexpr unsigned int Indices[] = 
		{
			2 , 0 ,1,
			2 , 1 ,3,
		};

		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexType) * 4;										// total array size
		bd.Usage = D3D11_USAGE_DEFAULT;												// how buffer data will be used (read/write protections for GPU/CPU)
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// What type of buffer would it be
		bd.CPUAccessFlags = 0u;														// we don't want any cpu access for now so setting it to 0 for now
		bd.MiscFlags = 0u;															// misscellinious flag for buffer configuration (we don't want it now either)
		bd.StructureByteStride = sizeof(VertexType);								// Size of every vertex in the array 

		//holds the data pointer that will be used in vertex buffer

		D3D11_SUBRESOURCE_DATA subd = { 0 };
		subd.pSysMem = Vertices;											// pointer to array so that it can copy all the array data to the buffer

		graphics_device->CreateBuffer(&bd, &subd, &vertex_buffer);

		D3D11_BUFFER_DESC ibd = { 0 };
		ibd.ByteWidth = sizeof(unsigned int) * 6;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(unsigned int);

		D3D11_SUBRESOURCE_DATA isubd = { 0 };
		isubd.pSysMem = Indices;

		graphics_device->CreateBuffer(&ibd, &isubd, &index_buffer);
	}
	~Sprite() = default;
public:
	virtual void Draw(ID3D11DeviceContext* device_context)
	{
		constexpr unsigned int stride = sizeof(VertexType);
		constexpr unsigned int offset = 0u;
		device_context->IASetVertexBuffers(0u, 1u, vertex_buffer.GetAddressOf(), &stride, &offset);
		device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
		device_context->DrawIndexed(6u, 0u, 0u);
	}
};