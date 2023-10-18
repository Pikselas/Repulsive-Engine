#pragma once
#include"RenderDevice.h"
#include"Image.h"

class MemoryRenderer : public RenderDevice
{
	friend class CoreEngine;
private:
	Image& surface;
	ID3D11DeviceContext* context;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> memory_data;
private:
	MemoryRenderer(ID3D11Device* device,ID3D11DeviceContext* context, Image& surface)
		:
		surface(surface),
		context(context),
		RenderDevice(surface.GetWidth() , surface.GetHeight())
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = surface.GetWidth();
		desc.Height = surface.GetHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		auto h = device->CreateTexture2D(&desc, nullptr, &memory_data);

		if(FAILED(h))
			MessageBox(nullptr, "Failed to create memory data", "Error", MB_OK);

		CreateTarget(device, memory_data.Get());
	}
public:
	void RenderFrame() override
	{
		//ID3D11Device* device;
		//ID3D11DeviceContext* device_context;
		D3D11_MAPPED_SUBRESOURCE mapped_resource = {};
		
		//memory_data->GetDevice(&device);
		//device->GetImmediateContext(&device_context);

		auto h = context->Map(memory_data.Get(), 0, D3D11_MAP_READ , 0, &mapped_resource);

		if (FAILED(h))
		{
			MessageBox(nullptr, "Failed to map memory data", "Error", MB_OK);
		}

		std::memcpy(surface.Raw(), mapped_resource.pData ,sizeof(ColorType) * surface.GetWidth() * surface.GetHeight());
		
		context->Unmap(memory_data.Get(), 0);
	}
};