#pragma once
#include <ShaderHelper.h>
#include <StringHelpers.h>

namespace AquaXP
{
	using GeometryShader = Microsoft::WRL::ComPtr<ID3D11GeometryShader>;

	template<template<typename> typename Alloc = std::allocator>
	Result<GeometryShader> CreateGeometryShader(
		ID3D11Device* device,
		std::wstring const& path
	)
	{
		using ByteAllocType = Alloc<u8>;
		using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
		static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
			"Byte Allocator must be for u8 type");

		std::shared_ptr<u8[]> byteCode;
		sz byteCodeSize;
		auto res = InitShaderCode<ID3D11GeometryShader, Alloc>(
			&ID3D11Device::CreateGeometryShader,
			device,
			path,
			byteCode,
			byteCodeSize
		);
		if(!isOk(res))
		{
			return error(res);
		}

		return get(res);
	}

	template<template<typename> typename Alloc = std::allocator>
	Result<GeometryShader> CreateGeometryShader(
		ID3D11Device* device,
		std::string const& path
	)
	{
		std::wstring wpath;
		if (!mbStrToWideChar(path, wpath))
		{
			return ErrorCode::WStringConversionFailure;
		}

		return CreateGeometryShader<Alloc>(device, wpath);
	}
}
