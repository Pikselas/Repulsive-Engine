#pragma once
#include <vector>
#include <span>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <functional>
#include "RenderCommandEngine.h"

class Object3D
{
public:
	struct VertexType
	{
		float x, y, z;
		unsigned char r, g, b, a;
	};

	struct RenderContextData
	{
		size_t index_count;
		ID3D11Buffer* index_buffer;
		ID3D11Buffer* vertex_buffer;
		DirectX::XMMATRIX transformation;
	};
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
protected:
	float FixedPointRotationX = 0.0f;
	float FixedPointRotationY = 0.0f;
	float FixedPointRotationZ = 0.0f;
protected:
	float PositionalRotateX = 0.0f;
	float PositionalRotateY = 0.0f;
	float PositionalRotateZ = 0.0f;
protected:
	float m_PositionX = 0.0f;
	float m_PositionY = 0.0f;
	float m_PositionZ = 0.0f;
protected:
	DirectX::XMMATRIX transformation = DirectX::XMMatrixIdentity();
protected:
	size_t m_IndexCount = 0;
protected:
	auto GetVBuff() const noexcept { return m_VertexBuffer; }
	auto GetIBuff() const noexcept { return m_IndexBuffer; }
	auto GetIndexCount() const noexcept { return m_IndexCount; }
public:
	auto GetTransformMatrix() const noexcept
	{
		return (DirectX::XMMatrixRotationRollPitchYaw(FixedPointRotationX, FixedPointRotationY, FixedPointRotationZ) *
			DirectX::XMMatrixTranslation(m_PositionX, m_PositionY, m_PositionZ) * DirectX::XMMatrixRotationRollPitchYaw(PositionalRotateX, PositionalRotateY, PositionalRotateZ)) * transformation;
	}
public:
	void Set(ID3D11Device* device , std::span<const VertexType> vertices, std::span<const unsigned int> indices)
	{
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexType) * vertices.size();						// total array size
		bd.Usage = D3D11_USAGE_DEFAULT;												// how buffer data will be used (read/write protections for GPU/CPU)
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// What type of buffer would it be
		bd.CPUAccessFlags = 0u;														// we don't want any cpu access for now so setting it to 0 for now
		bd.MiscFlags = 0u;															// misscellinious flag for buffer configuration (we don't want it now either)
		bd.StructureByteStride = sizeof(VertexType);								// Size of every vertex in the array 

		//holds the data pointer that will be used in vertex buffer

		D3D11_SUBRESOURCE_DATA subd = { 0 };
		subd.pSysMem = vertices.data();											// pointer to array so that it can copy all the array data to the buffer

		/*CallOnDevice(canvas, &ID3D11Device::CreateBuffer, &bd, &subd, &m_VertexBuffer);*/
		device->CreateBuffer(&bd, &subd, &m_VertexBuffer);

		D3D11_BUFFER_DESC ibd = { 0 };
		ibd.ByteWidth = sizeof(decltype(indices)::element_type) * indices.size();
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(decltype(indices)::element_type);

		D3D11_SUBRESOURCE_DATA isubd = { 0 };
		isubd.pSysMem = indices.data();

		//CallOnDevice(canvas, &ID3D11Device::CreateBuffer, &ibd, &isubd, &m_IndexBuffer);
		device->CreateBuffer(&ibd, &isubd, &m_IndexBuffer);

		m_IndexCount = indices.size();
	}
public:
	void RotateFixedPoint(const float x, const float y, const float z) noexcept
	{
		FixedPointRotationX = x;
		FixedPointRotationY = y;
		FixedPointRotationZ = z;
	}
	void RotatePositional(const float x, const float y, const float z)
	{
		PositionalRotateX = x;
		PositionalRotateY = y;
		PositionalRotateZ = z;
	}
	void RotateFixedPoint(const short x, const short y, const short z) noexcept
	{
		FixedPointRotationX = x * DirectX::XM_PI / 180;
		FixedPointRotationY = y * DirectX::XM_PI / 180;
		FixedPointRotationZ = z * DirectX::XM_PI / 180;
	}
	void RotatePositional(const short x, const short y, const short z)
	{
		PositionalRotateX = x * DirectX::XM_PI / 180;
		PositionalRotateY = y * DirectX::XM_PI / 180;
		PositionalRotateZ = z * DirectX::XM_PI / 180;
	}
	void SetPosition(const float x, const float y, const float z) noexcept
	{
		m_PositionX = x;
		m_PositionY = y;
		m_PositionZ = z;
	}
	void SetPosition(DirectX::XMVECTOR pos) noexcept
	{
		m_PositionX = DirectX::XMVectorGetX(pos);
		m_PositionY = DirectX::XMVectorGetY(pos);
		m_PositionZ = DirectX::XMVectorGetZ(pos);
	}
	void SetTransformation(DirectX::XMMATRIX mat)
	{
		transformation = mat;
	}
	DirectX::XMVECTOR GetPosition() const noexcept
	{
		return DirectX::XMVectorSet(m_PositionX, m_PositionY, m_PositionZ, 0.0f);
	}
public:
	void Draw(RenderCommandEngine& engine) const
	{
		RenderContextData context_data
		{
			.index_count = m_IndexCount,
			.index_buffer = m_IndexBuffer.Get(),
			.vertex_buffer = m_VertexBuffer.Get(),
			.transformation = GetTransformMatrix()
		};

		engine.SetContextData(&context_data);
		engine.Draw();
	}
};

class Cube : public Object3D
{
public:
	static constexpr VertexType Vertices[] = {
		//front
		{-1.0f , 1.0f , -1.0f , 100 , 200 , 150},
		{ 1.0f , 1.0f , -1.0f , 200 , 100 , 50},
		{-1.0f , -1.0f , -1.0f , 50 , 60 , 40},
		{ 1.0f , -1.0f , -1.0f , 100 , 120 , 130},

		//left
		{-1.0f ,1.0f , 1.0f , 200 , 0 , 10 },
		{-1.0f ,1.0f , -1.0f , 80 , 100 , 25},
		{-1.0f ,-1.0f , 1.0f , 220 , 190 , 34 },
		{-1.0f ,-1.0f , -1.0f , 100 , 123 , 78},

		//top
		{-1.0f ,1.0f , 1.0f , 99 , 98 , 97},
		{ 1.0f ,1.0f , 1.0f , 213 , 183 , 65},
		{-1.0f ,1.0f , -1.0f ,123 , 87 , 90 },
		{ 1.0f ,1.0f , -1.0f , 14 , 78 , 36 },

		//back
		{-1.0f , 1.0f , 1.0f , 123 , 67 , 100},
		{ 1.0f , 1.0f , 1.0f , 220 , 78 , 13},
		{-1.0f , -1.0f , 1.0f , 90 , 54 , 67},
		{ 1.0f , -1.0f , 1.0f , 132 , 78 , 21},

		//right
		{1.0f ,1.0f , 1.0f , 34 , 78 , 90 },
		{1.0f ,1.0f , -1.0f , 123 , 89 , 36 },
		{1.0f ,-1.0f , 1.0f , 24 , 63 , 217 },
		{1.0f ,-1.0f , -1.0f , 24 , 80 , 90 },

		//bottom
		{-1.0f ,-1.0f , -1.0f , 24 , 99 , 213},
		{ 1.0f ,-1.0f , -1.0f , 47 , 76 , 56 },
		{-1.0f ,-1.0f , 1.0f , 90 , 12 , 64 },
		{ 1.0f ,-1.0f , 1.0f , 124 , 76 , 34}
	};
	static constexpr unsigned int Indices[] = {
		2 , 0 ,1,
		2 , 1 ,3,

		6 , 4, 5,
		5 , 7 ,6,

		10 , 8 , 9,
		9 , 11, 10,

		13 , 12 , 14,
		14 , 15 , 13,

		18 , 17 , 16,
		19 , 17 , 18,

		20 , 21 , 22,
		23 , 22 , 21
	};
public:
	Cube(ID3D11Device* device)
	{
		auto v = std::span<const VertexType>(Vertices);
		auto i = std::span(Indices);
		Set(device, v, i);
	}
};