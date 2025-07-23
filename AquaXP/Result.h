#pragma once

namespace AquaXP
{
	enum class ErrorCode
	{
		// Framework codes
		UnknownError,
		ReBoxFailed,
		// D3D11 HRESULT codes
		D3D11ErrorFileNotFound,
		D3D11ErrorTooManyUniqueStateObjects,
		D3D11ErrorTooManyUniqueViewObjects,
		D3D11ErrorDeferredContextMapWithoutInitialDiscard,
		EFail,
		EInvalidArg,
		EOutOfMemory,
		ENotImplemented,
		SFalse,
		// DXGI Error Codes
		DXGIErrorAccessDenied,
		DXGIErrorAccessLost,
		DXGIErrorAlreadyExists,
		DXGIErrorCannotProtectContent,
		DXGIErrorDeviceHung,
		DXGIErrorDeviceRemoved,
		DXGIErrorDeviceReset,
		DXGIErrorDriverInternalError,
		DXGIErrorFrameStatisticsDisjoint,
		DXGIErrorGraphicsVidPNSourceInUse,
		DXGIErrorInvalidCall,
		DXGIErrorMoreData,
		DXGIErrorNameAlreadyExists,
		DXGIErrorNonexclusive,
		DXGIErrorNotCurrentlyAvailable,
		DXGIErrorNotFound,
		DXGIErrorRemoteClientDisconnected,
		DXGIErrorRemoteOutOfMemory,
		DXGIErrorRestrictToOutputStale,
		DXGIErrorSDKComponentMissing,
		DXGIErrorSessionDisconnected,
		DXGIErrorUnsupported,
		DXGIErrorWaitTimeout,
		DXGIErrorWasStillDrawing,
		// File Errors
		FileError,
		// Texture Codes
		DepthBufferCreationFailed,
		Texture2DCreationFailed,
		SRVCreationFailed,
		DSVCreationFailed,
		RTVCreationFailed,
		WICError,
		// File related codes
		FileNotFound
	};

	template<typename T>
	using Result = std::variant<T, ErrorCode>;

	template<typename T>
	bool tryGet(Result<T> result, T& data)
	{
		if (std::holds_alternative<T>(result))
		{
			data = std::get<T>(result);
			return true;
		}
		return false;
	}

	template<typename T>
	T get(Result<T> result)
	{
		return std::get<T>(result);
	}

	template<typename T>
	std::optional<T> getOpt(Result<T> result)
	{
		if (std::holds_alternative<T>(result))
		{
			return std::get<T>(result);
		}
		return std::nullopt;
	}

	template<typename T>
	bool isOk(Result<T> result)
	{
		return std::holds_alternative<T>(result);
	}

	template<typename T>
	bool tryError(Result<T> result, ErrorCode& error)
	{
		if (std::holds_alternative<ErrorCode>)
		{
			error = std::get<ErrorCode>(result);
			return true;
		}
		return false;
	}

	template<typename T>
	ErrorCode error(Result<T> result)
	{
		return std::get<ErrorCode>(result);
	}

	template<typename T>
	std::optional<ErrorCode> errorOpt(Result<T> result)
	{
		if (std::holds_alternative<ErrorCode>(result))
		{
			return std::get<ErrorCode>(result);
		}
		return std::nullopt;
	}
	
	AQUAXP_API ErrorCode HRToError(HRESULT hr);
}
