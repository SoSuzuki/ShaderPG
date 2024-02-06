#pragma once
#include <d3d11.h>
#include <string>
#include <wrl.h>

using namespace Microsoft::WRL;
using std::string;

class Texture
{
private:
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
	XMFLOAT2 imgSize_;

public:
	Texture();
	~Texture();
	HRESULT Load(string fileName);
	void Release();

	XMFLOAT2 GetTextureSize() { return imgSize_; }
	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }

};

