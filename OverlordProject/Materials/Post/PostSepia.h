#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostSepia : public PostProcessingMaterial
{
public:
	PostSepia();
	~PostSepia();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	static ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostSepia(const PostSepia &obj);
	PostSepia& operator=(const PostSepia& obj);
};
