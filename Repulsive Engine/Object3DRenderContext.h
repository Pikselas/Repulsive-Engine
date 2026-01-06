#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Object3D.h"
#include "CoreEngine.h"
#include "RenderActionContext.h"

namespace RenderAction
{
	class Object3DRenderContext : public RenderContext
	{
	private:
		Shader::Config::Object3DShader shd_config;
	private:
		Camera camera;
	public:
		Object3DRenderContext(ID3D11Device* device, const std::filesystem::path& p)
			:
			RenderContext(device),
			shd_config(device, p)
		{
			camera.SetZ(10.0f);
		}
	public:
		void Apply(ID3D11DeviceContext* context) const override
		{
			std::vector<Resource::ConstantBuffer> arr = { transformation_buff , surface_size_buffer};
			Resource::BindConstantBuffers(context, std::span<Resource::ConstantBuffer>(arr), shd_config.getShader<Shader::VertexShader>()->GetConstantBufferBinder());
			shd_config.BindToContext(context);
		}

		void SetComponent(ID3D11DeviceContext* context, void* cmp) override
		{
			auto rcd = static_cast<Object3D::RenderContextData*>(cmp);

			auto mtrx = DirectX::XMMatrixTranspose
			(
				rcd->transformation * camera.GetTransformation() *
				DirectX::XMMatrixPerspectiveLH(1.0f, 600.0f / 800.0f, 1.0f, 100.0f)
			);

			//SetTransformation(context, rcd->transformation);
			SetTransformation(context, mtrx);
			
		}
	};
}

namespace EngineAdapter
{
	class Object3DRenderingAdapter : public RenderCommandEngine
	{
	private:
		size_t index_count = 0;
	private:
		CoreEngine& engine;
		RenderAction::Object3DRenderContext& context;
	public:
		Object3DRenderingAdapter(CoreEngine& engine, RenderAction::Object3DRenderContext& cntxt)
			:
			engine(engine),
			context(cntxt)
		{
		}
	public:
		void Draw() override
		{
			engine.Draw(index_count);
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
			auto rcd = static_cast<Object3D::RenderContextData*>(data);
			engine.SetVertexBuffer(rcd->vertex_buffer, sizeof(Object3D::VertexType));
			engine.SetIndexBuffer(rcd->index_buffer);
			index_count = rcd->index_count;
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