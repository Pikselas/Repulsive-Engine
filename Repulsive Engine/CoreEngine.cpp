#include "CoreEngine.h"

CoreEngine::CoreEngine()
{
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &graphics_device, nullptr, &device_context);

	// set the primitive topology
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							// uses the alpha channel of the source pixel as the blend factor,
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;					// uses the inverse of the alpha channel of the source pixel as the blend factor
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								// adds the source and destination blend factors
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;							// it is fully opaque
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		// enables all the color channels for writing

	ObjectManager<ID3D11BlendState> blendState;
	graphics_device->CreateBlendState(&blendDesc, &blendState);
	device_context->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

	//constexpr unsigned int Indices[] =
	//{
	//	2 , 0 ,1,
	//	2 , 1 ,3,
	//};

	//D3D11_BUFFER_DESC ibd = { 0 };
	//ibd.ByteWidth = sizeof(unsigned int) * 6;
	//ibd.Usage = D3D11_USAGE_DEFAULT;
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//ibd.CPUAccessFlags = 0u;
	//ibd.MiscFlags = 0u;
	//ibd.StructureByteStride = sizeof(unsigned int);

	//D3D11_SUBRESOURCE_DATA isubd = { 0 };
	//isubd.pSysMem = Indices;

	//// create index buffer
	//graphics_device->CreateBuffer(&ibd, &isubd, &index_buffer);
	//device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

	D3D11_RASTERIZER_DESC rasterDesc = {};
	
	//rasterDesc.FillMode = D3D11_FILL_SOLID;
	//rasterDesc.CullMode = D3D11_CULL_NONE; // Disable culling
	//rasterDesc.FrontCounterClockwise = false;
	//rasterDesc.DepthClipEnable = true;

	//graphics_device->CreateRasterizerState(&rasterDesc, &raster_state);
	//device_context->RSSetState(raster_state.Get());

	D3D11_DEPTH_STENCIL_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));
	stencilDesc.DepthEnable = FALSE;
	stencilDesc.StencilEnable = TRUE;
	stencilDesc.StencilReadMask = 0xFF;
	stencilDesc.StencilWriteMask = 0xFF;

	stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	stencilDesc.BackFace = stencilDesc.FrontFace;

	graphics_device->CreateDepthStencilState(&stencilDesc, &STENCIL_PASS_STATE);

	D3D11_DEPTH_STENCIL_DESC maskedStencilDesc;
	ZeroMemory(&maskedStencilDesc, sizeof(maskedStencilDesc));
	maskedStencilDesc.DepthEnable = FALSE;
	maskedStencilDesc.StencilEnable = TRUE;
	maskedStencilDesc.StencilReadMask = 0xFF;
	maskedStencilDesc.StencilWriteMask = 0xFF;

	maskedStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	maskedStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	maskedStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	maskedStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	maskedStencilDesc.BackFace = maskedStencilDesc.FrontFace;

	graphics_device->CreateDepthStencilState(&maskedStencilDesc, &STENCIL_CLIP_STATE);

	// setting default pass state;
	device_context->OMSetDepthStencilState(STENCIL_PASS_STATE.Get(), 1);
}

void CoreEngine::SetIndexBuffer(ID3D11Buffer* indices)
{
	device_context->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0u);
}

void CoreEngine::SetVertexBuffer(ID3D11Buffer* vertices, unsigned int stride)
{
	constexpr unsigned int offset = 0u;
	device_context->IASetVertexBuffers(0u, 1u, &vertices, &stride, &offset);
}

void CoreEngine::SetRenderContext(RenderAction::RenderContext& context)
{
	if (render_context)
		render_context->UnApply(device_context.Get());
	render_context = &context;
	context.Apply(device_context.Get());
}

void CoreEngine::SetStencilBuffer(StencilBuffer& buffer)
{
	ObjectManager<ID3D11RenderTargetView> existing_view;
	ObjectManager<ID3D11DepthStencilView> stv;
	device_context->OMGetRenderTargets(1, &existing_view, &stv);
	device_context->OMSetRenderTargets(1, existing_view.GetAddressOf(), buffer.getView());
}

void CoreEngine::ClearStencilBuffer(StencilBuffer& buffer)
{
	device_context->ClearDepthStencilView(buffer.getView(), D3D11_CLEAR_STENCIL, 0, 0);
}

void CoreEngine::RemoveStencilBuffer()
{
	ObjectManager<ID3D11RenderTargetView> existing_view;
	ObjectManager<ID3D11DepthStencilView> stv;
	device_context->OMGetRenderTargets(1, &existing_view, &stv);
	device_context->OMSetRenderTargets(1, existing_view.GetAddressOf(), nullptr);
}

void CoreEngine::EndStencilClipping(unsigned int ref_value)
{
	device_context->OMSetDepthStencilState(STENCIL_PASS_STATE.Get(), ref_value);
}

void CoreEngine::BeginStencilClipping(unsigned int ref_value)
{
	device_context->OMSetDepthStencilState(STENCIL_CLIP_STATE.Get(), ref_value);
}

StencilBuffer CoreEngine::CreateStencilBuffer(unsigned int width, unsigned int height)
{
	return StencilBuffer(graphics_device.Get(), width , height);
}

Texture CoreEngine::CreateTexture(const Image& image)
{
	return Texture{ graphics_device.Get() , image };
}

ImageSprite CoreEngine::CreateSprite(const Image& image)
{
	return CreateSprite(CreateTexture(image), image.GetWidth(), image.GetHeight());
}

ImageSprite CoreEngine::CreateSprite(Texture texture , unsigned int width , unsigned int height)
{
	const float x_ = width * 0.5;
	const float y_ = -(height * 0.5);

	ImageSprite::VertexType Vertices[] =
	{
		{ -x_ , y_ },
		{ x_ , y_  },
		{-x_ , -y_ },
		{ x_ , -y_ },
	};

	ImageSprite sprite;

	sprite.width = width;
	sprite.height = height;

	sprite.texture = texture;

	sprite.SetTextureCoord(0, 0);
	sprite.SetTextureSize(width, height);

	// create vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(ImageSprite::VertexType) * 4;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(ImageSprite::VertexType);

	D3D11_SUBRESOURCE_DATA subd = { 0 };
	subd.pSysMem = Vertices;

	graphics_device->CreateBuffer(&bd, &subd, &sprite.vertex_buffer);

	return sprite;
}

AnimatedSprite CoreEngine::CreateSprite(const std::vector<Image>& frames, std::chrono::milliseconds duration, std::optional<unsigned int> repeat_count)
{
	std::vector<ImageSprite> sprites;
	sprites.reserve(frames.size());

	for (const auto& frame : frames)
	{
		sprites.push_back(CreateSprite(frame));
	}

	return AnimatedSprite(sprites, duration , repeat_count);
}

MemoryRenderer CoreEngine::CreateRenderer(Image& image)
{
	return MemoryRenderer(graphics_device.Get(), image);
}

WindowRenderer CoreEngine::CreateRenderer(CustomWindow& window)
{
	return WindowRenderer(graphics_device.Get(), window.window_handle , window.GetWidth() , window.GetHeight());
}

void CoreEngine::Draw(unsigned int index_count)
{
	device_context->DrawIndexed(index_count, 0u, 0u);
}