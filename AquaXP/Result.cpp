#include "pch.h"
#include "Result.h"

using namespace AquaXP;

ErrorCode AquaXP::HRToError(HRESULT hr)
{
	switch (hr)
	{
	/* Win32 codes */
	case E_FAIL: return ErrorCode::EFail;
	case E_INVALIDARG: return ErrorCode::EInvalidArg;
	case E_OUTOFMEMORY: return ErrorCode::EOutOfMemory;
	case E_NOTIMPL: return ErrorCode::ENotImplemented;
	case S_FALSE: return ErrorCode::SFalse;
	/* D3D11 */
	case D3D11_ERROR_FILE_NOT_FOUND: return ErrorCode::D3D11ErrorFileNotFound;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS: return ErrorCode::D3D11ErrorTooManyUniqueStateObjects;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS: return ErrorCode::D3D11ErrorTooManyUniqueViewObjects;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD: return ErrorCode::D3D11ErrorDeferredContextMapWithoutInitialDiscard;
	/* DXGI */
	case DXGI_ERROR_ACCESS_DENIED: return ErrorCode::DXGIErrorAccessDenied;
	case DXGI_ERROR_ACCESS_LOST: return ErrorCode::DXGIErrorAccessLost;
	case DXGI_ERROR_ALREADY_EXISTS: return ErrorCode::DXGIErrorAlreadyExists;
	case DXGI_ERROR_CANNOT_PROTECT_CONTENT: return ErrorCode::DXGIErrorCannotProtectContent;
	case DXGI_ERROR_DEVICE_HUNG: return ErrorCode::DXGIErrorDeviceHung;
	case DXGI_ERROR_DEVICE_REMOVED: return ErrorCode::DXGIErrorDeviceRemoved;
	case DXGI_ERROR_DEVICE_RESET: return ErrorCode::DXGIErrorDeviceReset;
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR: return ErrorCode::DXGIErrorDriverInternalError;
	case DXGI_ERROR_FRAME_STATISTICS_DISJOINT: return ErrorCode::DXGIErrorFrameStatisticsDisjoint;
	case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE: return ErrorCode::DXGIErrorGraphicsVidPNSourceInUse;
	case DXGI_ERROR_INVALID_CALL: return ErrorCode::DXGIErrorInvalidCall;
	case DXGI_ERROR_MORE_DATA: return ErrorCode::DXGIErrorMoreData;
	case DXGI_ERROR_NAME_ALREADY_EXISTS: return ErrorCode::DXGIErrorNameAlreadyExists;
	case DXGI_ERROR_NONEXCLUSIVE: return ErrorCode::DXGIErrorNonexclusive;
	case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE: return ErrorCode::DXGIErrorNotCurrentlyAvailable;
	case DXGI_ERROR_NOT_FOUND: return ErrorCode::DXGIErrorNotFound;
	case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED: return ErrorCode::DXGIErrorRemoteClientDisconnected;
	case DXGI_ERROR_REMOTE_OUTOFMEMORY: return ErrorCode::DXGIErrorRemoteOutOfMemory;
	case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE: return ErrorCode::DXGIErrorRestrictToOutputStale;
	case DXGI_ERROR_SDK_COMPONENT_MISSING: return ErrorCode::DXGIErrorSDKComponentMissing;
	case DXGI_ERROR_SESSION_DISCONNECTED: return ErrorCode::DXGIErrorSessionDisconnected;
	case DXGI_ERROR_UNSUPPORTED: return ErrorCode::DXGIErrorUnsupported;
	case DXGI_ERROR_WAIT_TIMEOUT: return ErrorCode::DXGIErrorWaitTimeout;
	case DXGI_ERROR_WAS_STILL_DRAWING: return ErrorCode::DXGIErrorWasStillDrawing;
	default: return ErrorCode::UnknownError;
	}
}