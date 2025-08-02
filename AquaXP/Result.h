#pragma once

namespace AquaXP
{
	enum class ErrorCode
	{
		// Framework codes
		UnknownError,
		ReBoxFailed,
		WStringConversionFailure,
		NoDisplayModeFound,
		QueryPerformanceCounterFailed,
		InternalException,
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
		// TextureBuilder Codes
		MissingTexture2D,
		// CBuffer Codes
		EmptyCBuffer,
		// File related codes
		FileNotFound
	};

	template<typename T>
	using Result = std::variant<T, ErrorCode>;

	struct Unit { };

	template<typename T>
	bool tryGet(Result<T> const& result, T& data)
	{
		if (std::holds_alternative<T>(result))
		{
			data = std::get<T>(result);
			return true;
		}
		return false;
	}

	template<typename T>
	T get(Result<T> const& result)
	{
		return std::get<T>(result);
	}

	template<typename T>
	T get(Result<T>&& result)
	{
		return std::get<T>(std::move(result));
	}

	template<typename T>
	std::optional<T> getOpt(Result<T> const& result)
	{
		if (std::holds_alternative<T>(result))
		{
			return std::get<T>(result);
		}
		return std::nullopt;
	}

	template<typename T>
	bool isOk(Result<T> const& result)
	{
		return std::holds_alternative<T>(result);
	}

	template<typename T>
	Result<T> ok(T const& data)
	{
		return data;
	}

	template<typename T>
	bool tryError(Result<T> const& result, ErrorCode& error)
	{
		if (std::holds_alternative<ErrorCode>(result))
		{
			error = std::get<ErrorCode>(result);
			return true;
		}
		return false;
	}

	template<typename T>
	ErrorCode error(Result<T> const& result)
	{
		return std::get<ErrorCode>(result);
	}

	template<typename T>
	std::optional<ErrorCode> errorOpt(Result<T> const& result)
	{
		if (std::holds_alternative<ErrorCode>(result))
		{
			return std::get<ErrorCode>(result);
		}
		return std::nullopt;
	}

	template<typename T>
	Result<T> tryDo(std::function<T()> function)
	{
		try
		{
			return function();
		}
		catch (...)
		{
			return ErrorCode::InternalException;
		}
	}

	template<typename T, typename... Args>
	Result<T> tryCreate(Args&& ...args)
	{
		try
		{
			return T(std::forward<Args>(args)...);
		}
		catch (...)
		{
			return ErrorCode::InternalException;
		}
	}

	AQUAXP_API ErrorCode HRToError(HRESULT hr);
}
