Texture2D<float4> DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 TexCoord : TEXCOORD;
};


float4 main(VS_OUTPUT input) : SV_Target
{

    float4 diffuse = DiffuseMap.Sample(Sampler,input.TexCoord);
    //diffuse = diffuse * input.Color;
    return diffuse; // input.Color;
}