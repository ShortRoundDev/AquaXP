#include "PixelInput.hlsli"

Texture2D albedo : register(t0);
SamplerState samplerState: register(s0);

float4 main(PixelInput pixel) : SV_TARGET
{
    return albedo.Sample(samplerState, pixel.tex);
}