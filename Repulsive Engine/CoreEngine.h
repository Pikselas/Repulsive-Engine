#pragma once

#include<D3DCompiler.h>
#pragma comment(lib,"D3DCompiler.lib")

#include"ImageSprite.h"
#include"CustomWindow.h"
#include"MemoryRenderer.h"
#include"WindowRenderer.h"

#include "Engine.h"

class CoreEngine : public Engine
{
private:
	template<typename ObjectT>
	using ObjectManager = Microsoft::WRL::ComPtr<ObjectT>;
private:
	struct VertexType
	{
		float x, y;
		float u, v;
	};
private:
	ObjectManager<ID3D11Device>				graphics_device;
	ObjectManager<ID3D11DeviceContext>		device_context;
private:
	ObjectManager<ID3D11InputLayout>		input_layout;
private:
	ObjectManager<ID3D11VertexShader>		vertex_shader;
	ObjectManager<ID3D11PixelShader>		pixel_shader;
private:
	ObjectManager<ID3D11Buffer>				vertex_shader_transform_buffer;
	ObjectManager<ID3D11Buffer>				vertex_shader_surface_size_buffer;
	ObjectManager<ID3D11Buffer>				index_buffer;
public:
	CoreEngine();
private:
	ObjectManager<ID3D11SamplerState>		SAMPLER_STATE;
public:
	void SetComponent(const DirectX::XMMATRIX transformation) override;
	void SetComponent(ID3D11ShaderResourceView* texture_view) override;
	void SetComponent(ID3D11Buffer* vertices) override;
public:
	ImageSprite CreateSprite(const Image& image);
	MemoryRenderer CreateRenderer(Image& image);
	WindowRenderer CreateRenderer(CustomWindow& window);
public:
	void SetRenderer(const RenderDevice& render_device);
public:
	void Draw() override;
public:
	void ClearFrame(const RenderDevice& render_device);
};