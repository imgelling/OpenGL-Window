struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = inPos;
    output.Color = inColor;
    return output;
}