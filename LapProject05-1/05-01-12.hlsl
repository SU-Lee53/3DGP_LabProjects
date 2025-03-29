
#define FRAME_BUFFER_WIDTH 800.f
#define FRAME_BUFFER_HEIGHT 600.f

float4 VSMain(uint nVertexID : SV_VertexID) : SV_POSITION
{
    float4 output = 0;
    
    float size = 1.0f;
    switch (nVertexID)
    {
        case 0:
            output = float4(-size, +size, 0.0f, 1.0f);
            break;
     
        case 1:
            output = float4(+size, +size, 0.0f, 1.0f);
            break;
     
        case 2:
            output = float4(+size, -size, 0.0f, 1.0f);
            break;
     
        case 3:
            output = float4(-size, +size, 0.0f, 1.0f);
            break;
     
        case 4:
            output = float4(+size, -size, 0.0f, 1.0f);
            break;
     
        case 5:
            output = float4(-size, -size, 0.0f, 1.0f);
            break;
    }
    
    return output;
}

float RegularPolygon(float2 f2NDC, float fSides, float fRadius)
{
    float fAngle = atan(f2NDC.y / f2NDC.x);
    float fSlices = (2.0f * 3.141592f) / fSides;
    
    float fShape = step(cos(floor((fAngle / fSlices) + 0.5f) * fSlices - fAngle) * length(f2NDC), fRadius);
    
    return fShape;
}

float4 PSMain(float4 pos : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.f, 0.f, 0.f, 1.f);
    
    float2 f2NDC = float2(pos.x / FRAME_BUFFER_WIDTH, pos.y / FRAME_BUFFER_HEIGHT) - 0.5f;
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    
    cColor.b = RegularPolygon(f2NDC - float2(-0.3f, -0.1f), 8.0f, 0.2f);
    cColor.r = RegularPolygon(f2NDC - float2(+0.3f, +0.2f), 4.0f, 0.15f);
    
    return cColor;
}