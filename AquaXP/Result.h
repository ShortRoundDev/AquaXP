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

	inline WCHAR const* ErrorMessage(ErrorCode code)
	{
		switch (code)
		{
		case ErrorCode::UnknownError:  return L"UnknownError";
		case ErrorCode::ReBoxFailed:  return L"ReBoxFailed";
		case ErrorCode::WStringConversionFailure:  return L"WStringConversionFailure";
		case ErrorCode::NoDisplayModeFound:  return L"NoDisplayModeFound";
		case ErrorCode::QueryPerformanceCounterFailed:  return L"QueryPerformanceCounterFailed";
		case ErrorCode::InternalException:  return L"InternalException";
		case ErrorCode::D3D11ErrorFileNotFound:  return L"D3D11ErrorFileNotFound";
		case ErrorCode::D3D11ErrorTooManyUniqueStateObjects:  return L"D3D11ErrorTooManyUniqueStateObjects";
		case ErrorCode::D3D11ErrorTooManyUniqueViewObjects:  return L"D3D11ErrorTooManyUniqueViewObjects";
		case ErrorCode::D3D11ErrorDeferredContextMapWithoutInitialDiscard:  return L"D3D11ErrorDeferredContextMapWithoutInitialDiscard";
		case ErrorCode::EFail:  return L"EFail";
		case ErrorCode::EInvalidArg:  return L"EInvalidArg";
		case ErrorCode::EOutOfMemory:  return L"EOutOfMemory";
		case ErrorCode::ENotImplemented:  return L"ENotImplemented";
		case ErrorCode::SFalse:  return L"SFalse";
		case ErrorCode::DXGIErrorAccessDenied:  return L"DXGIErrorAccessDenied";
		case ErrorCode::DXGIErrorAccessLost:  return L"DXGIErrorAccessLost";
		case ErrorCode::DXGIErrorAlreadyExists:  return L"DXGIErrorAlreadyExists";
		case ErrorCode::DXGIErrorCannotProtectContent:  return L"DXGIErrorCannotProtectContent";
		case ErrorCode::DXGIErrorDeviceHung:  return L"DXGIErrorDeviceHung";
		case ErrorCode::DXGIErrorDeviceRemoved:  return L"DXGIErrorDeviceRemoved";
		case ErrorCode::DXGIErrorDeviceReset:  return L"DXGIErrorDeviceReset";
		case ErrorCode::DXGIErrorDriverInternalError:  return L"DXGIErrorDriverInternalError";
		case ErrorCode::DXGIErrorFrameStatisticsDisjoint:  return L"DXGIErrorFrameStatisticsDisjoint";
		case ErrorCode::DXGIErrorGraphicsVidPNSourceInUse:  return L"DXGIErrorGraphicsVidPNSourceInUse";
		case ErrorCode::DXGIErrorInvalidCall:  return L"DXGIErrorInvalidCall";
		case ErrorCode::DXGIErrorMoreData:  return L"DXGIErrorMoreData";
		case ErrorCode::DXGIErrorNameAlreadyExists:  return L"DXGIErrorNameAlreadyExists";
		case ErrorCode::DXGIErrorNonexclusive:  return L"DXGIErrorNonexclusive";
		case ErrorCode::DXGIErrorNotCurrentlyAvailable:  return L"DXGIErrorNotCurrentlyAvailable";
		case ErrorCode::DXGIErrorNotFound:  return L"DXGIErrorNotFound";
		case ErrorCode::DXGIErrorRemoteClientDisconnected:  return L"DXGIErrorRemoteClientDisconnected";
		case ErrorCode::DXGIErrorRemoteOutOfMemory:  return L"DXGIErrorRemoteOutOfMemory";
		case ErrorCode::DXGIErrorRestrictToOutputStale:  return L"DXGIErrorRestrictToOutputStale";
		case ErrorCode::DXGIErrorSDKComponentMissing:  return L"DXGIErrorSDKComponentMissing";
		case ErrorCode::DXGIErrorSessionDisconnected:  return L"DXGIErrorSessionDisconnected";
		case ErrorCode::DXGIErrorUnsupported:  return L"DXGIErrorUnsupported";
		case ErrorCode::DXGIErrorWaitTimeout:  return L"DXGIErrorWaitTimeout";
		case ErrorCode::DXGIErrorWasStillDrawing:  return L"DXGIErrorWasStillDrawing";
		case ErrorCode::FileError:  return L"FileError";
		case ErrorCode::DepthBufferCreationFailed:  return L"DepthBufferCreationFailed";
		case ErrorCode::Texture2DCreationFailed:  return L"Texture2DCreationFailed";
		case ErrorCode::SRVCreationFailed:  return L"SRVCreationFailed";
		case ErrorCode::DSVCreationFailed:  return L"DSVCreationFailed";
		case ErrorCode::RTVCreationFailed:  return L"RTVCreationFailed";
		case ErrorCode::WICError:  return L"WICError";
		case ErrorCode::MissingTexture2D:  return L"MissingTexture2D";
		case ErrorCode::EmptyCBuffer:  return L"EmptyCBuffer";
		case ErrorCode::FileNotFound:  return L"FileNotFound";
		default: return L"Unknown Error";
		}
	}

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
