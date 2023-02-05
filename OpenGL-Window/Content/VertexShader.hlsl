struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = inPos;    //No calculations done here, just returns the position of the vertices
    output.Color = inColor;
    return output;        //send color and position to pixel shader
}