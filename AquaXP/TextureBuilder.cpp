#include "pch.h"
#include "TextureBuilder.h"
#include "StringHelpers.h"

using namespace AquaXP;
using namespace std;

TextureBuilder::TextureBuilder() :
	m_texture(),
	m_textureOptions(),
	m_srvOptions(),
	m_dsvOptions(),
	m_rtvOptions()
{}

TextureBuilder& TextureBuilder::withTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> const& texture)
{
	m_texture = texture;
	return *this;
}

TextureBuilder& TextureBuilder::withTexture(TextureOptions const& options)
{
	m_textureOptions = options;
	return *this;
}

TextureBuilder& TextureBuilder::withTexture(string const& path, TextureOptions const& options)
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
	m_textureOptions = options;
	return *this;
}

TextureBuilder& TextureBuilder::withTexture(wstring const& path, TextureOptions const& options)
{
	m_path = path;
	m_textureOptions = options;
	return *this;
}

TextureBuilder& TextureBuilder::withSRV(SRVOptions const& options)
{
	m_srvOptions = options;
	return *this;
}

TextureBuilder& TextureBuilder::withRTV(RTVOptions const& options)
{
	m_rtvOptions = options;
	return *this;
}

TextureBuilder& TextureBuilder::withDSV(DSVOptions const& options)
{
	m_dsvOptions = options;
	return *this;
}

Result<Texture> TextureBuilder::build(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (m_earlyError.has_value())
	{
		return error(m_earlyError.value());
	}
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	if (!m_texture.has_value())
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

		if (!m_path.has_value() && m_textureOptions.has_value())
		{
			auto textureOptions = m_textureOptions.value();
			D3D11_TEXTURE2D_DESC desc = { };
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = textureOptions.width;
			desc.Height = textureOptions.height;
			desc.MipLevels = textureOptions.mipLevels.value_or(1);
			desc.ArraySize = textureOptions.arraySize.value_or(1);
			desc.Format = textureOptions.format;
			desc.SampleDesc = textureOptions.sampleDesc;
			desc.Usage = textureOptions.usage.value_or(D3D11_USAGE_DEFAULT);
			desc.CPUAccessFlags = textureOptions.cpuAccessFlags.value_or(0);
			desc.MiscFlags = textureOptions.miscFlags.value_or(D3D11_RESOURCE_MISC_GENERATE_MIPS);
			desc.BindFlags = textureOptions.bindFlags.value_or(bindFlags);

			auto textureResult = CreateTexture2D(device, desc);
			if (!isOk(textureResult))
			{
				return error(textureResult);
			}

			m_texture = get(textureResult);
		}
		else if (m_path.has_value())
		{
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			auto options = m_textureOptions.value();
			HRESULT res = CreateWICTextureFromFileEx(
				device,
				context,
				m_path.value().c_str(),
				0L,
				options.usage.value_or(D3D11_USAGE_DEFAULT),
				options.bindFlags.value_or(bindFlags),
				options.cpuAccessFlags.value_or(0),
				options.miscFlags.value_or(D3D11_RESOURCE_MISC_GENERATE_MIPS),
				DirectX::WIC_LOADER_FLAGS::WIC_LOADER_FORCE_RGBA32,
				resource.GetAddressOf(),
				srv.GetAddressOf()
			);
			if (FAILED(res))
			{
				return ErrorCode::WICError;
			}

			resource.As(&m_texture.value());
		}
	}

	if (!m_texture.has_value())
	{
		return ErrorCode::MissingTexture2D;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	m_texture.value().Get()->GetDesc(&textureDesc);
	auto isMsaa = textureDesc.SampleDesc.Count > 1;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	if (m_rtvOptions.has_value())
	{
		auto rtvOptions = m_rtvOptions.value();
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = rtvOptions.format.value_or(textureDesc.Format);
		rtvDesc.ViewDimension = rtvOptions.viewDimension.value_or(isMsaa
			? D3D11_RTV_DIMENSION_TEXTURE2DMS
			: D3D11_RTV_DIMENSION_TEXTURE2D
		);
		auto rtvResult = CreateRenderTargetView(device, m_texture.value().Get(), rtvDesc);
		if (!isOk(rtvResult))
		{
			return error(rtvResult);
		}
		rtv = get(rtvResult);
	}

	if (srv != nullptr && m_srvOptions.has_value())
	{
		auto srvOptions = m_srvOptions.value();
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = srvOptions.format.value_or(textureDesc.Format);
		srvDesc.ViewDimension = srvOptions.viewDimension.value_or(isMsaa
			? D3D11_SRV_DIMENSION_TEXTURE2DMS
			: D3D11_SRV_DIMENSION_TEXTURE2D
		);

		auto srvResult = CreateShaderResourceView(device, m_texture.value().Get(), srvDesc);
		if (!isOk(srvResult))
		{
			return error(srvResult);
		}
		srv = get(srvResult);
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr;
	if (m_dsvOptions.has_value())
	{
		auto dsvOptions = m_dsvOptions.value();
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = dsvOptions.format;
		dsvDesc.ViewDimension = dsvOptions.viewDimension.value_or(isMsaa
			? D3D11_DSV_DIMENSION_TEXTURE2DMS
			: D3D11_DSV_DIMENSION_TEXTURE2D
		);
		dsvDesc.Flags = dsvOptions.flags.value_or(0);

		auto dsvResult = CreateDepthStencilView(device, m_texture.value().Get(), dsvDesc);
		if (!isOk(dsvResult))
		{
			return error(dsvResult);
		}
		dsv = get(dsvResult);
	}

	return Texture(
		m_texture.value(),
		srv,
		dsv,
		rtv
	);
}