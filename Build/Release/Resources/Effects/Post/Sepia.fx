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
	//Sepia
	float4 sampleColor = gTexture.Sample(samPoint, input.TexCoord);
	float4 sepiaColor;
	sepiaColor.r = (sampleColor.r*.393) + (sampleColor.g*.769) + (sampleColor.b*.189);
	sepiaColor.g = (sampleColor.r*.349) + (sampleColor.g*.686) + (sampleColor.b*.168);
	sepiaColor.b = (sampleColor.r*.272) + (sampleColor.g*.534) + (sampleColor.b*.131);
	if (sampleColor.r > 0.75 || (sampleColor.r > .5 && sampleColor.g < 0.5 && sampleColor.b < 0.5))
	{
		sepiaColor.r = sampleColor.r;
		sepiaColor.g = sampleColor.g;
		sepiaColor.b = sampleColor.b;
	}
	if (sampleColor.g > 0.75 || (sampleColor.g > .5 && sampleColor.r < 0.5 && sampleColor.b < 0.5))
	{
		sepiaColor.r = sampleColor.r;
		sepiaColor.g = sampleColor.g;
		sepiaColor.b = sampleColor.b;
	}
	if (sampleColor.b > 0.75 || (sampleColor.b > .5 && sampleColor.r < 0.5 && sampleColor.g < 0.5))
	{
		sepiaColor.r = sampleColor.r;
		sepiaColor.g = sampleColor.g;
		sepiaColor.b = sampleColor.b;
	}
	if (sampleColor.r > 0.75 && sampleColor.g > 0.75)
	{
		sepiaColor.r = sampleColor.r;
		sepiaColor.g = sampleColor.g;
		sepiaColor.b = sampleColor.b;
	}
	if (sepiaColor.r > 0.65 && sepiaColor.g > 0.65 && sepiaColor.b > 0.65)
	{
		sepiaColor.r = 210.0f / 256.0f;
		sepiaColor.g = 194.0f / 256.0f;
		sepiaColor.b = 167.0f / 256.0f;
	}
	return float4(sepiaColor.x, sepiaColor.y, sepiaColor.z, 1.0f);
}


//TECHNIQUE
//---------
technique11 Sepia
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

