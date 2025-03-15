#pragma once

namespace AquaXP
{
    AQUAXP_API Microsoft::WRL::ComPtr<ID3D11CommandList> GetCommandList(
        ID3D11DeviceContext* context,
        BOOL restoreDeferredContextState = FALSE
    );
};
