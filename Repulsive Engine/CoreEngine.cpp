#include "CoreEngine.h"

CoreEngine::CoreEngine(CustomWindow& window) 
	:

half_window_width((float)window.GetWidth() / 2) , half_window_height((float)window.GetHeight() / 2)
{
	ObjectManager<ID3D11Resource> BackBuffer;
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;												// look at the window and use it's size
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0;								// pick the default refresh rates
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferCount = 1;														// one back buffer -> one back and one front (double buffering)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;						// this is the color format (BGRA) 
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// not specifying any scaling because we want the renedred frame same as window size
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// how buffer scaning will be done for copying all to video memory
	sd.Flags = 0;															// not setting any flags
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// use the buffer for render target
	sd.OutputWindow = window.window_handle;									// set the window handle
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								// discard the effects for swapping frames
	sd.Windowed = TRUE;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	// create the device, swap chain and device context
	if (auto hrcode = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swap_chain, &graphics_device, nullptr, &device_context); FAILED(hrcode))
	{
		throw hrcode;
	}

	// get the back buffer of the swap chain and use as a render target view
	swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBuffer);
	graphics_device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &render_target_view);
	device_context->OMSetRenderTargets(1u, render_target_view.GetAddressOf(), nullptr);

	// create the viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window.GetWidth();		//screen height
	viewport.Height = window.GetHeight();	// screen width
	viewport.MaxDepth = 1;					// maximum depth for z axis
	viewport.MinDepth = 0;					// minimum depth for z axis
	device_context->RSSetViewports(1u, &viewport);

	// set the primitive topology
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, 100);
	std::filesystem::path path = buffer;
	auto program_dir = path.parent_path();

	auto vertex_shader_path = program_dir / "VertexShader.cso";
	auto pixel_shader_path = program_dir / "PixelShader.cso";

	ObjectManager<ID3DBlob> shader_buffer;
	D3DReadFileToBlob(vertex_shader_path.c_str(), &shader_buffer);
	graphics_device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &vertex_shader);
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] = 
	{
		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,sizeof(float) * 2,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	graphics_device->CreateInputLayout(ied, 2, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &input_layout);
	device_context->IASetInputLayout(input_layout.Get());

	D3DReadFileToBlob(pixel_shader_path.c_str(), &shader_buffer);
	graphics_device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &pixel_shader);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VertexShaderBufferT);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	graphics_device->CreateBuffer(&bd, nullptr, &vertex_shader_buffer);

	device_context->VSSetConstantBuffers(0u, 1u, vertex_shader_buffer.GetAddressOf());

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							// uses the alpha channel of the source pixel as the blend factor,
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;					// uses the inverse of the alpha channel of the source pixel as the blend factor
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								// adds the source and destination blend factors
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;							// it is fully opaque
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;						// it is fully transparent
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		// enables all the color channels for writing

	ObjectManager<ID3D11BlendState> blendState;
	graphics_device->CreateBlendState(&blendDesc, &blendState);
	device_context->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

}

ID3D11Device* CoreEngine::GetGraphicsDevice() const
{
	return graphics_device.Get();
}

void CoreEngine::SetComponent(ID3D11Buffer* indices)
{
	device_context->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0u);
}

void CoreEngine::SetComponent(const DirectX::XMMATRIX transformation)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	device_context->Map(vertex_shader_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
	VertexShaderBufferT buffer_data{ half_window_width , half_window_height , transformation };
	std::memcpy(ms.pData, &buffer_data, sizeof(VertexShaderBufferT));
	device_context->Unmap(vertex_shader_buffer.Get(), 0u);
}

void CoreEngine::SetComponent(ID3D11Buffer* vertices, unsigned int stride)
{
	constexpr unsigned int offset = 0u;
	device_context->IASetVertexBuffers(0u, 1u, &vertices, &stride, &offset);
}

void CoreEngine::SetComponent(ID3D11ShaderResourceView* texture_view)
{
	device_context->PSSetShaderResources(0u, 1u, &texture_view);
}

void CoreEngine::Draw(unsigned int size)
{
	device_context->DrawIndexed(size, 0u, 0u);
}

void CoreEngine::ClearFrame()
{
	constexpr float color[] = { 0.0f,0.0f,0.0f,1.0f };
	device_context->ClearRenderTargetView(render_target_view.Get(), color);
}

void CoreEngine::RenderFrame()
{
	swap_chain->Present(1u, 0u);
}
