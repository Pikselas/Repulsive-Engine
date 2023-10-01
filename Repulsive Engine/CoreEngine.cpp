#include "CoreEngine.h"

CoreEngine::CoreEngine(CustomWindow& window)
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

	D3DReadFileToBlob(pixel_shader_path.c_str(), &shader_buffer);
	graphics_device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &pixel_shader);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

}

CoreEngine::ObjectManager<ID3D11Device> CoreEngine::GetGraphicsDevice() const
{
	return graphics_device;
}