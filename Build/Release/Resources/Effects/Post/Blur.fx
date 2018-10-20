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
	output.Position = float4(input.Position, 1);
	// Set the TexCoord
	output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
	float width;
	float height;
	gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
	float dx = 1 / width;
	float dy = 1 / height;
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 5; i++)
	{
		float du = dx * (i*2);
		for (int j = 0; j < 5; j++)
		{
			float dv = dy * (j*2);

			float2 texCoord;
			texCoord.x = input.TexCoord.x + du;
			texCoord.y = input.TexCoord.y + dv;

			float4 sampleColor = gTexture.Sample(samPoint, texCoord);
			finalColor.x += sampleColor.x;
			finalColor.y += sampleColor.y;
			finalColor.z += sampleColor.z;
		}
	}

	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
	finalColor.x /= (5 * 5);
	finalColor.y /= (5 * 5);
	finalColor.z /= (5 * 5);

	// Step 5: return the final color

	return finalColor;
}


//TECHNIQUE
//---------
technique11 Blur
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