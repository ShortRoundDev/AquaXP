#include "VertexInput.hlsli"
#include "PixelInput.hlsli"

#include "Matrices.hlsli"
#include "Model.hlsli"

float3x3 GetRotationMatrix(float4x4 modelMatrix)
{
    // Extract just the 3x3 upper-left portion and normalize each row
    // This removes scale and maintains rotation
    float3 row1 = normalize(modelMatrix[0].xyz);
    float3 row2 = normalize(modelMatrix[1].xyz);
    float3 row3 = normalize(modelMatrix[2].xyz);
    
    return float3x3(row1, row2, row3);
}

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
    pixel.normal = mul(input.normal, GetRotationMatrix(model));
    
    return pixel;
}