#include "Common_VertexShader_Includes.hlsli"

VertexShaderOut main(VertexShaderInput vs_inp)
{
    return ProcessVertex(vs_inp);
}