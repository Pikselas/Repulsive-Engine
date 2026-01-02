#pragma once
#include <d3d11.h>

namespace RenderAction
{
	class ContextComponent
	{
	public:
		virtual void BindToContext(ID3D11DeviceContext*) const = 0;
		virtual ~ContextComponent() = default;
	};
}