struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 0.0f, 0.0f, 1.0f);
//}

float4 main(VS_OUTPUT input) : SV_Target
{
    return input.Color;    // Set the color of the pixel to what we defined for the vertex.
}