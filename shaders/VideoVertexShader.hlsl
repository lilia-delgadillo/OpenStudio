struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

VertexOutput main(uint vertexID : SV_VertexID)
{
    // Generate vertex position and texture coordinates.
    VertexOutput output;
    float2 position;
    
    switch (vertexID)
    {
        case 0:
        // Vertex 0
            position = float2(-1.0, 1.0);
            output.texCoord = float2(0.0, 0.0);
            break;

        case 1:
        // Vertex 1
            position = float2(3.0, 1.0);
            output.texCoord = float2(2.0, 0.0);
            break;

        case 2:
        // Vertex 2
            position = float2(-1.0, -3.0);
            output.texCoord = float2(0.0, 2.0);
            break;
        
        default:
            position = float2(0.0, 0.0);
            output.texCoord = float2(0.0, 0.0);
            break;
    }
    
    output.position = float4(position.x, position.y, 0.0, 1.0);
    return output;
}