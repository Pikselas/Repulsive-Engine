#pragma once
#include<d3d11.h>
#include<wrl.h>

class RenderDevice
{
	friend class CoreEngine;
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
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