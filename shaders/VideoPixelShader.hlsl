Texture2D videoTexture : register(t0);
SamplerState videoSampler : register(s0);

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelInput input) : SV_TARGET
{
    float4 color = videoTexture.Sample(videoSampler, input.texCoord);
    return color;
}