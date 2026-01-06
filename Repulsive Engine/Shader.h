#pragma once
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <array>
#include <span>
#include <filesystem>
#include<d3dcompiler.h>

#include "EngineResources.h"
#include "RenderActionContextComponent.h"

namespace Shader
{
	class Shader
	{		
	private:
		Resource::ConstantBuffer::Binder buffer_binder = nullptr;
	public:
		virtual ~Shader() = default;
	protected:
		void set_const_buffer_binder(Resource::ConstantBuffer::Binder binder)
		{
			buffer_binder = binder;
		}
	public:
		virtual void Bind(ID3D11DeviceContext* context) const = 0;
		Resource::ConstantBuffer::Binder GetConstantBufferBinder() const
		{
			return buffer_binder;
		}
	};

	class ShaderConfiguration : public RenderAction::ContextComponent
	{
	public:
		std::list<std::reference_wrapper<Shader>> shaders;
	public:
		ShaderConfiguration() = default;
	public:
		void AddShader(Shader& shader)
		{
			shaders.push_back(shader);
		}
	public:
		void BindToContext(ID3D11DeviceContext* context) const override
		{
			for (const auto& shader : shaders)
			{
				shader.get().Bind(context);
			}
		}
		template<typename T>
		T* getShader() const
		{
			for (auto& shader : shaders)
			{
				if (auto s = dynamic_cast<T*>(&(shader.get())); s != nullptr)
				{
					return s;
				}
			}
			return nullptr;
		}
	};

	class VertexShader : public Shader
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	public:
		struct InputElemDesc
		{
		public:
			enum class INPUT_FORMAT
			{
				FLOAT2 = DXGI_FORMAT_R32G32_FLOAT,
				FLOAT3 = DXGI_FORMAT_R32G32B32_FLOAT,
				FLOAT4 = DXGI_FORMAT_R32G32B32A32_FLOAT,
				UINT4 = DXGI_FORMAT_R8G8B8A8_UNORM
			};
		public:
			std::string SEMANTIC_NAME;
			INPUT_FORMAT FORMAT;
			unsigned int OFFSET;
		};

	public:
		VertexShader() = default;
		VertexShader(ID3D11Device* device, const std::filesystem::path& cso_file, const std::span<const InputElemDesc> descs)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
			D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
			std::vector<D3D11_INPUT_ELEMENT_DESC> iedescs;
			for (const auto& desc : descs)
			{
				iedescs.emplace_back(desc.SEMANTIC_NAME.c_str(), 0, static_cast<DXGI_FORMAT>(desc.FORMAT), 0, desc.OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0);
			}
			
			device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &shader);
			device->CreateInputLayout(iedescs.data(), (UINT)iedescs.size(), shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &input_layout);

			set_const_buffer_binder(&ID3D11DeviceContext::VSSetConstantBuffers);
		}

		void Bind(ID3D11DeviceContext* context) const override
		{
			context->VSSetShader(shader.Get(), nullptr, 0u);
			context->IASetInputLayout(input_layout.Get());
		}

		ID3D11VertexShader* get()
		{
			return shader.Get();
		}
	};

	class PixelShader : public Shader
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	public:
		PixelShader() = default;
		PixelShader(ID3D11Device* device, const std::filesystem::path& cso_file)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
			D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
			device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &shader);
			set_const_buffer_binder(&ID3D11DeviceContext::PSSetConstantBuffers);
		}
	public:
		void Bind(ID3D11DeviceContext* context) const override
		{
			context->PSSetShader(shader.Get(), nullptr, 0);
		}
	};

	class GeometryShader : public Shader
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
	public:
		GeometryShader() = default;
		GeometryShader(ID3D11Device* device , const std::filesystem::path& cso_file)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
			D3DReadFileToBlob(cso_file.c_str(), &pBlob);
			device->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &shader);
			set_const_buffer_binder(&ID3D11DeviceContext::GSSetConstantBuffers);
		}
	public:
		void Bind(ID3D11DeviceContext* context) const override
		{
			context->GSSetShader(shader.Get(), nullptr, 0u);
		}
	};

	namespace Config
	{
		class ImageSpriteShader : public ShaderConfiguration
		{
			VertexShader v_shader;
			PixelShader p_shader;
		public:
			ImageSpriteShader(ID3D11Device* device , const std::filesystem::path& p_dir)
			{
				const std::array<VertexShader::InputElemDesc, 1> input_desc
				{
					VertexShader::InputElemDesc{"POSITION" ,VertexShader::InputElemDesc::INPUT_FORMAT::FLOAT2 , 0},
				};

				v_shader = VertexShader{ device , p_dir / "VertexShader.cso", std::span<const VertexShader::InputElemDesc>(input_desc) };
				p_shader = PixelShader{ device , p_dir / "PixelShader.cso" };

				AddShader(v_shader);
				AddShader(p_shader);
			}
		};

		class Object3DShader : public ShaderConfiguration
		{
			PixelShader p_shader;
			VertexShader v_shader;
			GeometryShader g_shader;
		public:
			Object3DShader(ID3D11Device* device, const std::filesystem::path& p_dir)
			{
				const std::array<VertexShader::InputElemDesc,2> input_desc
				{
					VertexShader::InputElemDesc{"POSITION" ,VertexShader::InputElemDesc::INPUT_FORMAT::FLOAT3 , 0},
					VertexShader::InputElemDesc{"COLOR" , VertexShader::InputElemDesc::INPUT_FORMAT::UINT4, sizeof(float) * 3}
				};

				v_shader = VertexShader{ device , p_dir / "VS_Object3D.cso" , input_desc };
				p_shader = PixelShader{ device , p_dir / "PS_Object3D.cso" };
				g_shader = GeometryShader{ device , p_dir / "GS_Object3D.cso" };

				AddShader(v_shader);
				AddShader(g_shader);
				AddShader(p_shader);
			}
		};
	}
}