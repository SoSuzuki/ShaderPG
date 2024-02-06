#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}

using namespace DirectX;

class Sprite
{
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;		//頂点座標変換行列
		XMMATRIX	uvTrans;	//テクスチャ座標変換
		XMFLOAT4	color;		//テクスチャとの合成
		float		scroll;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//UV座標
	};

protected:
	uint64_t vertexNum_;				//頂点数
	std::vector<VERTEX>vertices_;	//頂点情報
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ


	uint64_t indexNum_;			//インデックス数
	std::vector<int> index_;	//インデックス情報

	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;			//テクスチャ

public:
	Sprite();
	~Sprite();

	float scrollVal_;

	virtual HRESULT Initialize();
	virtual void Draw(Transform& transform);
	void Draw(Transform& transform, RECT rect, float alpha);
	void Release();

	/// <summary>
	///  画像サイズの取得
	/// </summary>
	/// <returns>画像サイズ</returns>
	XMFLOAT2 GetTextureSize() { return pTexture_->GetTextureSize(); }

	ID3D11Buffer* GetVertexBuffer() { return pVertexBuffer_; }
	ID3D11Buffer* GetIndexBuffer() { return pIndexBuffer_; }
	ID3D11Buffer* GetConstantBuffer() { return pConstantBuffer_; }

	HRESULT Load(string _filename);

private:
	//---------Initializeから呼ばれる関数---------
	virtual void InitVertexData();	//頂点情報の準備
	HRESULT CreateVertexBuffer();	//頂点バッファを作成

	virtual void InitIndexData();	//インデックス情報の準備
	HRESULT CreateIndexBuffer();	//インデックスバッファを作成

	HRESULT CreateConstantBuffer();	//コンスタントバッファを作成

	HRESULT LoadTexture();			//テクスチャをロード

	//---------Draw関数から呼ばれる関数---------
	void PassDataToCB(XMMATRIX worldMatrix);	//コンスタントバッファに情報を渡す
	void SetBufferToPipeline();							//各バッファをパイプラインに


};

