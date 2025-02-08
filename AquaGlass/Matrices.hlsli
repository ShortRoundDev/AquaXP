cbuffer CameraBuffer : register(b0)
{
    matrix view;
    matrix projection;
    float3 pos;
}