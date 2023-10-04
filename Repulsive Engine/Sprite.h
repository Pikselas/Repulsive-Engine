#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<DirectXMath.h>

#include<span>

#include"CoreEngine.h"
class Sprite
{
private:
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f , 1.0f);
	DirectX::XMMATRIX transformation = DirectX::XMMatrixIdentity();
public:
	virtual void Draw(CoreEngine& engine) const = 0;
public:
	void SetTransformation(const DirectX::XMMATRIX transformation)
	{
		this->transformation = transformation;
	}
	DirectX::XMMATRIX GetTransformation() const
	{
		return transformation * DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), 0.0f);
	}
};