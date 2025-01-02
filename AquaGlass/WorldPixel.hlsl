#include "PixelInput.hlsli"

Texture2D albedo : register(t0);
SamplerState samplerState: register(s0);

float3 lightDir = normalize(float3(1.0f, 0.0f, 1.0f));

float4 main(PixelInput pixel) : SV_TARGET
{
    float3 normal = normalize(pixel.normal);
    
    float diffuse = max(dot(-lightDir, normal), 0.0f);
    float3 viewDir = normalize(-pixel.pos.xyz);
    float3 reflectDir = reflect(lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
    
    float ambient = 0.5f;
    
    return albedo.Sample(samplerState, pixel.tex) * (ambient + diffuse + specular * 0.5f);
}