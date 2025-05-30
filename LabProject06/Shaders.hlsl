
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    
    return output;
}

float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}