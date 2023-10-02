struct VertexShaderOut // this is the ouput type
{
    float2 tex : TEXCOORD; // this is the user defined semantic for color which will be passed to pixel shader
    float4 pos : SV_POSITION; // this is the SYSTEM_VALUE SEMANTIC this is fixed (and defined by the API) 
};


cbuffer cbuff
{
    matrix transform;
};

VertexShaderOut main(float2 pos : POSITION, float2 tex : TEXCOORD)
{
    VertexShaderOut Out;
    
    // 
    
    Out.pos = mul(transform , float4(pos, 0.0f, 1.0f));
    Out.tex = tex;
    return Out;
}