#include "pch.h"
#include "ContextHelpers.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace AquaXP;

ComPtr<ID3D11CommandList> AquaXP::GetCommandList(ID3D11DeviceContext* context, BOOL restoreDeferredContextState)
{
    ComPtr<ID3D11CommandList> commandList;
    if (FAILED(context->FinishCommandList(restoreDeferredContextState, commandList.GetAddressOf())))
    {
        // warn user: probably an immediate context
        return nullptr;
    }
    return commandList;
}