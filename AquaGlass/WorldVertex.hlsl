#include "VertexInput.hlsli"
#include "PixelInput.hlsli"

#include "Matrices.hlsli"
#include "Model.hlsli"

PixelInput main(VertexInput input)
{
    PixelInput pixel;
    pixel.color = input.color;
    pixel.pos = float4(input.position.xyz, 1.0f);
    
    pixel.pos = mul(pixel.pos, model);
    pixel.pos = mul(pixel.pos, world);
    pixel.pos = mul(pixel.pos, view);
    pixel.pos = mul(pixel.pos, projection);
    
    pixel.tex = input.uv;
    
    return pixel;
}