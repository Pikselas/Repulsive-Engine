#pragma once

#include<D3DCompiler.h>
#pragma comment(lib,"D3DCompiler.lib")

#include"CustomWindow.h"
#include"MemoryRenderer.h"
#include"WindowRenderer.h"
#include"AnimatedSprite.h"

#include "ResourceEngine.h"
#include "RenderActionContext.h"


class CoreEngine : public ResourceEngine
{
private:
	template<typename ObjectT>
	using ObjectManager = Microsoft::WRL::ComPtr<ObjectT>;
private:
	RenderAction::RenderContext* render_context = nullptr;
private:
	ObjectManager<ID3D11DeviceContext>		device_context;
private:
	ObjectManager<ID3D11Buffer>				index_buffer;
public:
	CoreEngine();
private:
	ObjectManager<ID3D11SamplerState>		SAMPLER_STATE;
	ObjectManager<ID3D11RasterizerState>	raster_state;
private:
	ObjectManager<ID3D11DepthStencilState> STENCIL_PASS_STATE;
	ObjectManager<ID3D11DepthStencilState> STENCIL_CLIP_STATE;
public:
	void SetIndexBuffer(ID3D11Buffer* indices);
	void SetVertexBuffer(ID3D11Buffer* vertices , unsigned int stride);
public:
	void SetRenderContext(RenderAction::RenderContext& context);
public:
	void SetStencilBuffer(StencilBuffer& buffer) ;
	void ClearStencilBuffer(StencilBuffer& buffer);
public:
	void RemoveStencilBuffer();
	void EndStencilClipping(unsigned int ref_value);
	void BeginStencilClipping(unsigned int ref_value);
public:
	StencilBuffer CreateStencilBuffer(unsigned int width, unsigned int height) override;
public:
	Texture CreateTexture(const Image& image) override;
public:
	ImageSprite CreateSprite(const Image& image) override;
	ImageSprite CreateSprite(Texture texture, unsigned int width , unsigned int height) override;
	AnimatedSprite CreateSprite(const std::vector<Image>& frames , std::chrono::milliseconds duration , std::optional<unsigned int> repeat_count = std::nullopt);
public:
	MemoryRenderer CreateRenderer(Image& image);
	WindowRenderer CreateRenderer(CustomWindow& window);
public:
	void Draw(unsigned int index_count);
public:
	ID3D11DeviceContext* GetDeviceContext() const
	{
		return device_context.Get();
	}
	ID3D11Device* GetDevice() const
	{
		return graphics_device.Get();
	}

	void ClearFrame(const RenderDevice& render_device)
	{
		device_context->ClearRenderTargetView(render_device.GetTarget(), DirectX::Colors::White);
	}

	void SetContextData(void* data)
	{
		if (render_context)
			render_context->SetComponent(device_context.Get(), data);
	}

	void SetRenderDevice(const RenderDevice& render_device)
	{
		// create and set the viewport
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = render_device.GetWidth();
		viewport.Height = render_device.GetHeight();
		viewport.MaxDepth = 1;	// maximum depth for z axis
		viewport.MinDepth = 0;	// minimum depth for z axis

		device_context->RSSetViewports(1u, &viewport);

		if (render_context)
		{
			render_context->SetSurfaceSize(device_context.Get(), render_device.GetWidth() , render_device.GetHeight());
		}

		auto render_target_view = render_device.GetTarget();
		device_context->OMSetRenderTargets(1u, &render_target_view, nullptr);
	}
};