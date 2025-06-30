#pragma once
#include <ShaderHelper.h>

namespace AquaXP
{
	template<template<typename> typename Alloc = std::allocator>
	class GeometryShader
	{
		using ByteAllocType = Alloc<u8>;
		using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
		static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
			"Byte Allocator must be for u8 type");

	public:
		GeometryShader(
			ID3D11Device* device,
			std::wstring const& path
		) : m_status(true),
			m_shader()
		{
			std::shared_ptr<u8[]> byteCode;
			sz byteCodeSize;
			if (!InitShaderCode<ID3D11GeometryShader, Alloc>(
				&ID3D11Device::CreateGeometryShader,
				device,
				path,
				byteCode,
				byteCodeSize,
				m_shader.GetAddressOf()
			)) {
				m_status = false;
				return;
			}
		}

		void use(ID3D11DeviceContext* context) const
		{
			context->GSSetShader(m_shader.Get(), NULL, 0);
		}

		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11GeometryShader> getShader() const
		{
			return m_shader;
		}

	private:
		bool m_status;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_shader;
	};
}
