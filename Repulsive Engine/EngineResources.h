#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <ranges>

namespace Resource
{
	class ConstantBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> BUFFER;
	public:
		using Binder = void (ID3D11DeviceContext::*)(UINT, UINT, ID3D11Buffer* const*);
	public:
		ConstantBuffer() = default;
		ConstantBuffer(ID3D11Device* device, const size_t size)
		{
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = size;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			device->CreateBuffer(&desc, nullptr, &BUFFER);
		}
		auto GetBuffer() const
		{
			return BUFFER;
		}
	};

	inline void BindConstantBuffers(ID3D11DeviceContext* cntx, std::span<ConstantBuffer> buffers, ConstantBuffer::Binder bind)
	{
		std::vector<ID3D11Buffer*> buff;
		auto b = buffers | std::ranges::views::transform([](const ConstantBuffer& b) -> ID3D11Buffer* { return b.GetBuffer().Get(); });
		std::ranges::copy(b, std::back_inserter(buff));
		std::mem_fn(bind)(cntx,0,buff.size(),buff.data());
	}
}