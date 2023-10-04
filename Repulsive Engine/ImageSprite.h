#pragma 
#include<filesystem>
#include"Image.h"
#include"Sprite.h"

class ImageSprite : public Sprite
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			 TEXTURE;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TEXTURE_VIEW;
private:
	struct VertexType
	{
		float x, y;
		float u, v;
	};
public:
	ImageSprite(CoreEngine& engine, const Image& image) 
	{

		const float x_ = image.GetWidth() * 0.5;
		const float y_ = -(image.GetHeight() * 0.5);

		const VertexType Vertices[] =
		{
			{ -x_ , y_ , 0.0f , 0.0f},
			{ x_ , y_  , 1.0f , 0.0f},
			{-x_ , -y_  , 0.0f , 1.0f},
			{ x_ , -y_  , 1.0f , 1.0f},
		};
		constexpr unsigned int Indices[] =
		{
			2 , 0 ,1,
			2 , 1 ,3,
		};

		// create vertex buffer
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexType) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizeof(VertexType);

		D3D11_SUBRESOURCE_DATA subd = { 0 };
		subd.pSysMem = Vertices;											

		engine.GetGraphicsDevice()->CreateBuffer(&bd, &subd, &vertex_buffer);

		D3D11_BUFFER_DESC ibd = { 0 };
		ibd.ByteWidth = sizeof(unsigned int) * 6;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(unsigned int);

		D3D11_SUBRESOURCE_DATA isubd = { 0 };
		isubd.pSysMem = Indices;

		// create index buffer
		engine.GetGraphicsDevice()->CreateBuffer(&ibd, &isubd, &index_buffer);

		// create texture data
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = image.GetWidth();
		desc.Height = image.GetHeight();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		auto data = image.Raw();
		subresource_data.pSysMem = data;
		subresource_data.SysMemPitch = sizeof(*data) * image.GetWidth();

		subresource_data.SysMemSlicePitch = 0;

		engine.GetGraphicsDevice()->CreateTexture2D(&desc, &subresource_data, &TEXTURE);
		engine.GetGraphicsDevice()->CreateShaderResourceView(TEXTURE.Get(), nullptr, &TEXTURE_VIEW);

	}

	void Draw(CoreEngine& engine) const override
	{
		engine.SetComponent(index_buffer.Get());
		engine.SetComponent(TEXTURE_VIEW.Get());
		engine.SetComponent(GetTransformation());
		engine.SetComponent(vertex_buffer.Get(), sizeof(VertexType));

		engine.Draw(6);
	}
};