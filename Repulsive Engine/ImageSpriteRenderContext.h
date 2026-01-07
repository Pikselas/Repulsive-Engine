#pragma once
#include "ImageSprite.h"
#include "CoreEngine.h"
#include "RenderActionContext.h"

namespace RenderAction
{
	class ImageSpriteRenderContext : public RenderContext
	{
	private:
		Shader::Config::ImageSpriteShader shd_config;
		Resource::ConstantBuffer tex_coord_buff;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
	public:
		ImageSpriteRenderContext(ID3D11Device* device, const std::filesystem::path& p)
			:
			RenderContext(device),
			shd_config(device, p),
			tex_coord_buff(device, sizeof(float) * 4)
		{
			constexpr unsigned int Indices[] =
			{
				2 , 0 ,1,
				2 , 1 ,3,
			};

			D3D11_BUFFER_DESC ibd = { 0 };
			ibd.ByteWidth = sizeof(unsigned int) * 6;
			ibd.Usage = D3D11_USAGE_DEFAULT;
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0u;
			ibd.MiscFlags = 0u;
			ibd.StructureByteStride = sizeof(unsigned int);

			D3D11_SUBRESOURCE_DATA isubd = { 0 };
			isubd.pSysMem = Indices;

			// create index buffer
			device->CreateBuffer(&ibd, &isubd, &index_buffer);
		}
	public:
		void Apply(ID3D11DeviceContext* context) const override
		{
			std::vector<Resource::ConstantBuffer> arr = { transformation_buff , surface_size_buffer , tex_coord_buff };
			Resource::BindConstantBuffers(context, std::span<Resource::ConstantBuffer>(arr), shd_config.getShader<Shader::VertexShader>()->GetConstantBufferBinder());
			shd_config.BindToContext(context);

			context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
		}

		void UnApply(ID3D11DeviceContext* context) const override
		{
			ID3D11Buffer* null_buf[] = { nullptr , nullptr , nullptr };
			context->VSSetConstantBuffers(0u, 3u, null_buf);
			ID3D11ShaderResourceView* null_srv[] = { nullptr };
			context->PSSetShaderResources(0u, 1u, null_srv);

			context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0u);

			shd_config.UnBindFromContext(context);
		}

		void SetTextureResource(ID3D11DeviceContext* context, ID3D11ShaderResourceView* srv, std::pair<float, float> coord , std::pair<float, float> size) const
		{
			context->PSSetShaderResources(0u, 1u, &srv);
			const std::pair<float, float> c_data[] = { coord , size };
			D3D11_MAPPED_SUBRESOURCE ms;
			context->Map(tex_coord_buff.GetBuffer().Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
			std::memcpy(ms.pData, c_data, sizeof(c_data));
			context->Unmap(tex_coord_buff.GetBuffer().Get(), 0u);
		}

		void SetComponent(ID3D11DeviceContext* context , void* cmp) override
		{
			auto rcd = static_cast<ImageSprite::RenderContextData*>(cmp);
			SetTransformation(context, rcd->transformation);
			SetTextureResource(context, rcd->texture_view, rcd->texture_coord, rcd->texture_size);
		}
	};
}

namespace EngineAdapter
{
	class ImageSpriteRenderingAdapter : public RenderCommandEngine
	{
		CoreEngine& engine;
		RenderAction::ImageSpriteRenderContext& context;
	public:
		ImageSpriteRenderingAdapter(CoreEngine& engine, RenderAction::ImageSpriteRenderContext& cntxt)
			:
			engine(engine),
			context(cntxt)
		{
		}
	public:
		void Draw() override
		{
			engine.Draw(6);
		}
	public:
		void ClearFrame(const RenderDevice& render_device)
		{
			engine.ClearFrame(render_device);
		}
	public:
		void SetTransformation(const DirectX::XMMATRIX& transformation)
		{
			context.SetTransformation(engine.GetDeviceContext(), transformation);
		}

		void SetContextData(void* data) override
		{
			auto rcd = static_cast<ImageSprite::RenderContextData*>(data);
			engine.SetVertexBuffer(rcd->vertex_buffer, sizeof(ImageSprite::VertexType));
			context.SetComponent(engine.GetDeviceContext(), data);
		}
	public:
		void SetStencilBuffer(StencilBuffer& buffer) override
		{
			engine.SetStencilBuffer(buffer);
		}
		void ClearStencilBuffer(StencilBuffer& buffer) override
		{
			engine.ClearStencilBuffer(buffer);
		}
	public:
		void RemoveStencilBuffer() override
		{
			engine.RemoveStencilBuffer();
		}
		void EndStencilClipping(unsigned int ref_value) override
		{
			engine.EndStencilClipping(ref_value);
		}
		void BeginStencilClipping(unsigned int ref_value) override
		{
			engine.BeginStencilClipping(ref_value);
		}
	};
}