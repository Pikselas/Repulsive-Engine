#pragma once
#include<wrl.h>
#include"Sprite.h"
#include"Texture.h"

class ImageSprite : public Sprite
{
	friend class CoreEngine;
public:
	struct RenderContextData
	{
		ID3D11Buffer* vertex_buffer;
		DirectX::XMMATRIX transformation;
		ID3D11ShaderResourceView* texture_view;
		std::pair<float, float> texture_coord;
		std::pair<float, float> texture_size;
	};
public:
	struct VertexType
	{
		float x, y;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
private:
	Texture texture;
private:
	unsigned int width;
	unsigned int height;
private:
	float texture_coord_start_x;
	float texture_coord_start_y;
	float texture_coord_width;
	float texture_coord_height;
public:
	void Draw(RenderCommandEngine& engine) const override
	{
		RenderContextData context_data
		{
			.vertex_buffer = vertex_buffer.Get(),
			.transformation = GetTransformedWithPosition(),
			.texture_view = texture.GetResourceView(),
			.texture_coord = GetTextureCoord(),
			.texture_size = GetTextureSize()
		};

		engine.SetContextData(&context_data);
		engine.Draw();
	}

	void DrawNonTransformed(RenderCommandEngine& engine) const override
	{
		RenderContextData context_data
		{
			.vertex_buffer = vertex_buffer.Get(),
			.transformation = DirectX::XMMatrixIdentity(),
			.texture_view = texture.GetResourceView(),
			.texture_coord = GetTextureCoord(),
			.texture_size = GetTextureSize()
		};
		engine.SetContextData(&context_data);
		engine.Draw();
	}
public:
	void SetTextureCoord(unsigned int x, unsigned int y)
	{
		texture_coord_start_x = (float)x / texture.GetWidth();
		texture_coord_start_y = (float)y / texture.GetHeight();
	}
	void SetTextureSize(unsigned int width, unsigned int height)
	{
		texture_coord_width = (float)width / texture.GetWidth();
		texture_coord_height = (float)height / texture.GetHeight();
	}
	void SetTexture(Texture tex)
	{
		texture = tex;
	}
public:
	unsigned int GetWidth() const
	{
		return width;
	}
	unsigned int GetHeight() const
	{
		return height;
	}
	std::pair<float, float> GetTextureCoord() const
	{
		return { texture_coord_start_x , texture_coord_start_y };
	}
	std::pair<float, float> GetTextureSize() const
	{
		return { texture_coord_width , texture_coord_height };
	}
};