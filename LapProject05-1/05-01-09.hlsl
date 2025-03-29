
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

float Rectangle(float2 f2NDC, float fLeft, float fRight, float fTop, float fBottom)
{
    float2 f2Shape = float2(step(fLeft, f2NDC.x), step(f2NDC.x, fRight));
    f2Shape *= float2(step(fTop, f2NDC.y), step(f2NDC.y, fBottom));
    
    return f2Shape.x * f2Shape.y;
}

float4 PSMain(float4 pos : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.f, 0.f, 0.f, 1.f);
    
    float2 f2NDC = float2(pos.x / FRAME_BUFFER_WIDTH, pos.y / FRAME_BUFFER_HEIGHT) - 0.5f;
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    
    float2 f2Horizontal = float2(0.1f, +0.3f);
    float2 f2Vertical = float2(-0.3f, +0.3f);
    
    cColor.b = Rectangle(f2NDC, +0.1f, +0.3f, -0.2f, +0.4f);
    cColor.b += Rectangle(f2NDC, -0.3f, -0.1f, -0.4f, -0.1f);
    
    return cColor;
}