#pragma once
#include<d3d11.h>
#include<wrl.h>

class RenderDevice
{
	friend class CoreEngine;
private:
	const unsigned int width;
	const unsigned int height;
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
protected:
	RenderDevice(unsigned int width, unsigned int height)
		:
		width(width),
		height(height)
	{}
private:
	ID3D11RenderTargetView* GetTarget() const
	{
		return render_target_view.Get();
	}
protected:
	void CreateTarget(ID3D11Device* device, ID3D11Resource* surface)
	{
		device->CreateRenderTargetView(surface, nullptr, &render_target_view);
	}
};