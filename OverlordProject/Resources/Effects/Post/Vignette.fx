//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState gDSS_EnableDepthWriting
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};


/// Create Rasterizer State (Backface culling) 
RasterizerState gRS_FrontCulling
{
	CullMode = NONE;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	output.Position = float4(input.Position.rgb, 1);
	// Set the TexCoord
	output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	//float4 sampleColor = gTexture.Sample(samPoint, input.TexCoord);

	//Vignette
	float4 color = float4(0, 0, 0, 1);
	float2 dist = input.TexCoord - 0.5f;
	dist.x = 1 - dot(dist, dist);
	color.a *= 1.0f - saturate(pow(dist.x, 5.5f));
	return color;
}


//TECHNIQUE
//---------
technique11 Vignette
{
    pass P0
    {          
        // Set states...
		SetRasterizerState(gRS_FrontCulling);
		SetDepthStencilState(gDSS_EnableDepthWriting, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

