

float4 VSMain(uint nVertexID : SV_VertexID) : SV_POSITION
{
    float4 output;
    
    // Returns vertex by index that organize primitive(triangle)
    // The coordinate of vertex is transformed coordinate(SV_POSITION) which is, projection-space coordinate.
    if (nVertexID == 0)
        output = float4(0.0, 0.5, 0.5, 1.0);
    else if (nVertexID == 1)
        output = float4(0.5, -0.5, 0.5, 1.0);
    else if (nVertexID == 2)
        output = float4(-0.5, -0.5, 0.5, 1.0);
    
    return output;
}

float4 PSMain(float4 pos : SV_POSITION) : SV_TARGET
{
    return float4(1.0, 1.0, 0.0, 1.0);
}