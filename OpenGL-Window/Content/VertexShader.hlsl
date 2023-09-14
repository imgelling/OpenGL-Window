////Texture2D<float4> DiffuseMap : register(t0);

//struct VS_OUTPUT
//{
//    float4 Pos : SV_POSITION;
//    float4 Color : COLOR0;
//    float2 TexCoord : TEXCOORD;
//};

//VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR, float2 inTexCoord : TEXCOORD)
//{
//    VS_OUTPUT output = (VS_OUTPUT) 0;
//    output.Pos = inPos;
//    output.Color = inColor;
//    output.TexCoord = inTexCoord;
//    return output;
//}

// old triangle
// simple vertex shader
//float4 main(float3 pos : POSITION) : SV_POSITION
//{
//    return float4(pos, 1.0f);
//}

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}