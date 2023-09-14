////Texture2D<float4> DiffuseMap : register(t0);
////SamplerState Sampler : register(s0);

//struct VS_OUTPUT
//{
//    float4 Pos : SV_POSITION;
//    float4 Color : COLOR0;
//    float2 TexCoord : TEXCOORD;
//};


//float4 main(VS_OUTPUT input) : SV_Target
//{

//    float4 diffuse = input.Color; // DiffuseMap.Sample(Sampler, input.TexCoord);
//    //diffuse = diffuse * input.Color;
//    return diffuse; // input.Color;
//}

// old triangle
// simple pixel shader
//float4 main() : SV_TARGET
//{
//    return float4(0.0f, 1.0f, 0.0f, 1.0f); // Red, Green, Blue, Alpha
//}

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    // return interpolated color
    return input.color;
}