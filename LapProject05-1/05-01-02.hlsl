

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

float4 PSMain(float4 pos : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.f, 0.f, 0.f, 1.f);
    cColor.r = pos.x / FRAME_BUFFER_WIDTH;
    
    
    return cColor;
}