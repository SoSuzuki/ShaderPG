//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	matrix g_matWorld;			//頂点座標変換行列
	matrix g_matTexture;		//テクスチャ座標変換行列
	float4 g_vecColor;			//テクスチャ合成色
	float  g_scroll;			//スクロール量の変数
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//位置
	float2 uv	  :	TEXCOORD;		//UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド行列をかけて
	//ワールド座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, g_matWorld);
	outData.uv = mul(uv, g_matTexture);

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	float4 output;
	float2 tmpUV = inData.uv;
	tmpUV.x += g_scroll;
	output = g_vecColor * g_texture.Sample(g_sampler, tmpUV);

	//return g_texture.Sample(g_sampler, inData.uv);
	return output;
}