#pragma 
#include<wrl.h>
#include"Sprite.h"
#include"Texture.h"
class ImageSprite : public Sprite
{
	friend class CoreEngine;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
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
		engine.SetComponent(GetTransformation());
		DrawNonTransformed(engine);
	}

	void DrawNonTransformed(RenderCommandEngine& engine) const override
	{
		engine.SetComponent(index_buffer.Get());
		engine.SetComponent(texture.GetResourceView(), GetTextureCoord(), GetTextureSize());
		engine.SetComponent(vertex_buffer.Get());

		engine.Draw();
	}
public:
	void SetTextureCoord(float x , float y)
	{
		texture_coord_start_x = x;
		texture_coord_start_y = y;
	}
	void SetTextureSize(float width, float height)
	{
		texture_coord_width = width;
		texture_coord_height = height;
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