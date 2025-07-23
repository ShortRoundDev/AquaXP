#include "pch.h"
#include "Result.h"

using namespace AquaXP;

ErrorCode HRToError(HRESULT hr)
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
	}
}