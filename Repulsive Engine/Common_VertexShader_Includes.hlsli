#include "Common_Includes.hlsli"

cbuffer sprite_transform
{
    matrix transformation_view;
};

cbuffer screen_size
{
    float2 half_screen_size;
    float2 padding;
};

VertexShaderOut ProcessVertex(VertexShaderInput vs_inp)
{
    VertexShaderOut Out;
    Out.pos = mul(float4(vs_inp.pos, 1.0f), transformation_view);
    Out.worldPos = vs_inp.pos;
    Out.transformedPos = mul(float4(vs_inp.pos, 1.0), transformation_view);
    Out.color = vs_inp.col;
    return Out;
}