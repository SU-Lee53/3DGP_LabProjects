
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

#define HALF_WIDTH      (FRAME_BUFFER_WIDTH * 0.5f)
#define HALF_HEIGHT     (FRAME_BUFFER_HEIGHT * 0.5f)

inline bool IsZero(float fValue, float fEpsilon)
{
    return abs(fValue) <= fEpsilon;
}

inline bool IsEqual(float fA, float fB, float fEpsilon)
{
    return abs(fA - fB) <= fEpsilon;
}


float4 PSMain(float4 pos : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if ((int) pos.x == (int) HALF_WIDTH)
        cColor.g = 1.0f;
    if ((int) pos.y == (int) HALF_HEIGHT)
        cColor.r = 1.0f;
    
    float fDistance = distance((int2) pos.xy, float2(HALF_WIDTH, HALF_HEIGHT));
    if (IsEqual(fDistance, 100.0f, 0.5f))
        cColor.b = 1.0f;
    
    return cColor;
}