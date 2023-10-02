#pragma once
#include<wrl.h>
#include<d3d11.h>
#include<DirectXMath.h>
#include<D3DCompiler.h>
#include<filesystem>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#include"CustomWindow.h"

#include"Image.h"

class CoreEngine
{
private:
	template<typename ObjectT>
	using ObjectManager = Microsoft::WRL::ComPtr<ObjectT>;
private:
	ObjectManager<ID3D11Device>				graphics_device;
	ObjectManager<ID3D11DeviceContext>		device_context;
	ObjectManager<IDXGISwapChain>			swap_chain;
	ObjectManager<ID3D11RenderTargetView>	render_target_view;
private:
	ObjectManager<ID3D11InputLayout>		input_layout;
private:
	ObjectManager<ID3D11VertexShader>		vertex_shader;
	ObjectManager<ID3D11PixelShader>		pixel_shader;
private:
	ObjectManager<ID3D11Buffer>				transformation_buffer;
public:
	CoreEngine(CustomWindow& window);
	~CoreEngine() = default;
public:
	ID3D11Device*	GetGraphicsDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
private:
	ObjectManager<ID3D11SamplerState>		SAMPLER_STATE;
public:
	void SetTransformation(const DirectX::XMMATRIX transformation);
public:
	void ClearFrame();
	void RenderFrame();
};