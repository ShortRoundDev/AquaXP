#include "pch.h"
#include "CubeMapBuilder.h"
#include "StringHelpers.h"

using namespace AquaXP;
using namespace std;
using namespace Microsoft::WRL;

CubeMapBuilder::CubeMapBuilder() :
	m_earlyError(),
	m_texture2D(),
	m_path(),
	m_textureOptions(),
	m_srvOptions(),
	m_dsvOptions(),
	m_rtvOptions()
{}

CubeMapBuilder& CubeMapBuilder::withTexture(CubeMapTextureOptions const& options)
{
	m_textureOptions = options;
	return *this;
}

CubeMapBuilder& CubeMapBuilder::withTexture(ComPtr<ID3D11Texture2D> texture)
{
	m_texture2D = texture;
	return *this;
}

CubeMapBuilder& CubeMapBuilder::withTexture(wstring const& path)
{
	m_path = path;
	return *this;
}

CubeMapBuilder& CubeMapBuilder::withTexture(string const& path)
{
	wstring wpath;
	if (!mbStrToWideChar(path, wpath))
	{
		m_earlyError = ErrorCode::WStringConversionFailure;
	}
	else
	{
		m_path = wpath;
	}
	return *this;
}

CubeMapBuilder& CubeMapBuilder::withSRV(SRVOptions const& options)
{
	m_srvOptions = options;
	return *this;
}

CubeMapBuilder& CubeMapBuilder::withDSV(DSVOptions const& options)
{
	m_dsvOptions = options;
	return *this;
}

CubeMapBuilder& CubeMapBuilder::withRTV(RTVOptions const& options)
{
	m_rtvOptions = options;
	return *this;
}

Result<CubeMap> CubeMapBuilder::build(ID3D11Device* device)
{
	if (m_earlyError.has_value())
	{
		return error(m_earlyError.value());
	}
	ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	if (!m_texture2D.has_value())
	{
		if (m_textureOptions.has_value())
		{
			auto textureOptions = m_textureOptions.value();
			D3D11_TEXTURE2D_DESC desc = { };
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = textureOptions.width;
			desc.Height = textureOptions.height;
			desc.MipLevels = textureOptions.mipLevels.value_or(1);
			desc.ArraySize = 6;
			desc.Format = textureOptions.format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = textureOptions.usage.value_or(D3D11_USAGE_DEFAULT);
			desc.CPUAccessFlags = textureOptions.cpuAccessFlags.value_or(0);
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | textureOptions.miscFlags.value_or(D3D11_RESOURCE_MISC_TEXTURECUBE);

			if (textureOptions.bindFlags.has_value())
			{
				desc.BindFlags = textureOptions.bindFlags.value();
			}
			else
			{
				UINT bindFlags = 0;
				if (m_dsvOptions.has_value())
				{
					bindFlags |= D3D11_BIND_DEPTH_STENCIL;
				}
				if (m_rtvOptions.has_value())
				{
					bindFlags |= D3D11_BIND_RENDER_TARGET;
				}
				if (m_srvOptions.has_value())
				{
					bindFlags |= D3D11_BIND_SHADER_RESOURCE;
				}
				desc.BindFlags = bindFlags;
			}

			auto textureResult = CreateTexture2D(device, desc);
			if (!isOk(textureResult))
			{
				return error(textureResult);
			}
			m_texture2D = get(textureResult);
		}
		else if (m_path.has_value())
		{
			ComPtr<ID3D11Texture2D> texture;
			HRESULT res = DirectX::CreateDDSTextureFromFile(
				device,
				m_path.value().c_str(),
				(ID3D11Resource**)texture.GetAddressOf(),
				srv.GetAddressOf()
			);
			if (FAILED(res))
			{
				return HRToError(res);
			}

			m_texture2D = texture;
		}
	}

	if (!m_texture2D.has_value() || m_texture2D.value() == nullptr)
	{
		return ErrorCode::MissingTexture2D;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	m_texture2D.value()->GetDesc(&textureDesc);

	if (srv == nullptr && m_srvOptions.has_value())
	{
		auto srvOptions = m_srvOptions.value();
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = srvOptions.format.value_or(textureDesc.Format);
		srvDesc.ViewDimension = srvOptions.viewDimension.value_or(D3D11_SRV_DIMENSION_TEXTURECUBE);
		srvDesc.TextureCube.MipLevels = textureDesc.MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;

		auto srvResult = CreateShaderResourceView(device, m_texture2D->Get(), srvDesc);
		if (!isOk(srvResult))
		{
			return error(srvResult);
		}
		srv = get(srvResult);
	}

	optional<array<ComPtr<ID3D11DepthStencilView>, 6>> dsvArray = nullopt;
	if (m_dsvOptions.has_value())
	{
		dsvArray.emplace(); // init array

		auto dsvOptions = m_dsvOptions.value();
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Flags = dsvOptions.flags.value_or(0);
		dsvDesc.Format = dsvOptions.format;
		dsvDesc.ViewDimension = dsvOptions.viewDimension.value_or(D3D11_DSV_DIMENSION_TEXTURE2DARRAY);
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.ArraySize = 1;
		dsvDesc.Texture2DArray.FirstArraySlice = 0;

		for (u32 i = 0; i < 6; ++i)
		{
			dsvDesc.Texture2DArray.FirstArraySlice = i;
			auto dsvResult = CreateDepthStencilView(device, m_texture2D->Get(), dsvDesc);
			if (!isOk(dsvResult))
			{
				return error(dsvResult);
			}
			dsvArray.value()[i] = get(dsvResult);
		}
	}

	optional<array<ComPtr<ID3D11RenderTargetView>, 6>> rtvArray = nullopt;
	if (m_rtvOptions.has_value())
	{
		rtvArray.emplace();

		auto rtvOptions = m_rtvOptions.value();
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = rtvOptions.format.value_or(textureDesc.Format);
		rtvDesc.ViewDimension = rtvOptions.viewDimension.value_or(D3D11_RTV_DIMENSION_TEXTURE2DARRAY);
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.ArraySize = 1;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;

		for (u32 i = 0; i < 6; ++i)
		{
			rtvDesc.Texture2DArray.FirstArraySlice = i;
			auto rtvResult = CreateRenderTargetView(device, m_texture2D->Get(), rtvDesc);
			if (!isOk(rtvResult))
			{
				return error(rtvResult);
			}
			rtvArray.value()[i] = get(rtvResult);
		}
	}

	return CubeMap(
		m_texture2D.value(),
		srv,
		rtvArray,
		dsvArray
	);
}