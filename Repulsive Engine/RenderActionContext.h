#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "ImageSprite.h"

namespace RenderAction
{
	// Manages a list of configured components
	// that will be used for performing render operation
	class RenderContext
	{
	public:
		Resource::ConstantBuffer transformation_buff;
		Resource::ConstantBuffer surface_size_buffer;
	public:
		RenderContext(ID3D11Device* device)
			:
			transformation_buff(device, sizeof(DirectX::XMMATRIX)),
			surface_size_buffer(device, sizeof(float) * 2 < 16 ? 16 : sizeof(float) * 2)
		{}
	public:
		void SetTransformation(ID3D11DeviceContext* context, const DirectX::XMMATRIX& matrix) const
		{
			D3D11_MAPPED_SUBRESOURCE ms;
			context->Map(transformation_buff.GetBuffer().Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
			std::memcpy(ms.pData, &matrix, sizeof(DirectX::XMMATRIX));
			context->Unmap(transformation_buff.GetBuffer().Get(), 0u);
		}
		void SetSurfaceSize(ID3D11DeviceContext* context, unsigned int width, unsigned int height) const
		{
			const float surface_size[] = { static_cast<float>(width) / 2 , static_cast<float>(height) / 2 };

			// update the surface size in vertex shader
			D3D11_MAPPED_SUBRESOURCE ms;
			context->Map(surface_size_buffer.GetBuffer().Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
			std::memcpy(ms.pData, surface_size, sizeof(float) * std::size(surface_size));
			context->Unmap(surface_size_buffer.GetBuffer().Get(), 0u);
		}
		virtual void SetComponent(ID3D11DeviceContext*, void*) {}
	public:
		virtual void Apply(ID3D11DeviceContext* context) const = 0;
		virtual void UnApply(ID3D11DeviceContext* context) const = 0;
	};
}