#pragma once
#include "ImageSprite.h"
#include "CoreEngine.h"
#include "RenderActionContext.h"

namespace RenderAction
{
	class ImageSpriteRenderContext : public RenderContext
	{
	public:
		Shader::Config::ImageSpriteShader shd_config;
		Resource::ConstantBuffer tex_coord_buff;
	public:
		ImageSpriteRenderContext(ID3D11Device* device, const std::filesystem::path& p)
			:
			RenderContext(device),
			shd_config(device, p),
			tex_coord_buff(device, sizeof(float) * 4)
		{
		}
	public:
		void Apply(ID3D11DeviceContext* context) const override
		{
			std::vector<Resource::ConstantBuffer> arr = { transformation_buff , surface_size_buffer , tex_coord_buff };
			Resource::BindConstantBuffers(context, std::span<Resource::ConstantBuffer>(arr), shd_config.getShader<Shader::VertexShader>()->GetConstantBufferBinder());
			shd_config.BindToContext(context);
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
			auto rcd = static_cast<RenderContextData*>(cmp);

			struct VertexType
			{
				float x, y;
			};

			constexpr unsigned int offset = 0u;
			constexpr unsigned int stride = sizeof(VertexType);
			context->IASetVertexBuffers(0u, 1u, &rcd->vertex_buffer, &stride, &offset);

			SetTransformation(context, rcd->transformation);
			//auto trv = static_cast<ImageSprite::TextureResourceView*>(cmp);
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
			engine.Draw();
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

		void SetVertexBuffer(ID3D11Buffer* vertices) override
		{
			engine.SetVertexBuffer(vertices, sizeof(CoreEngine::VertexType));
		}

		void SetContextData(void* data) override
		{
			context.SetComponent(engine.GetDeviceContext(), data);
		}

		/*void SetUserComponent(void* cmp)
		{
			auto trv = static_cast<ImageSprite::TextureResourceView*>(cmp);
			context.SetTextureResource(engine.GetDeviceContext(), trv->resource, trv->coord, trv->size);
		}*/
	};
}