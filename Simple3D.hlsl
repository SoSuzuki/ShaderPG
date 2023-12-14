//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer gmodel:register(b0)
{
	float4x4	matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	matW;			// ワールド行列
	float4x4	matNormal;		// ワールド行列	matWから改名
	float4		diffuseColor;	// マテリアルの色 => 拡散反射光
	float4		ambientColor;	// 環境光
	float4		specularColor;	// 鏡面反射光
	float		shininess;
	bool		isTextured;		// テクスチャ貼ってあるかどうか
};

cbuffer gmodel:register(b1)
{
	float4		lightPos;
	float4		eyePos;			// 視点座標
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos		:	SV_POSITION;	//位置
	float4 color	:	COLOR;			//色->輝度（明るさ）
	float4 eyev		:	POSITION1;		//視点（カメラ位置）
	float4 normal	:	POSITION2;
	float4 light	:	POSITION3;
	float2 uv		:	TEXCOORD;		//UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	// wの情報は入ってて欲しくないので0
	normal.w = 0;
	//法線を回転
	normal = mul(normal, matNormal);
	outData.normal = normal;

	float4 light = normalize(lightPos);	//光源の座標

	outData.color = saturate(dot(normal, light));
	//outData.color.a = 1;
	float4 posw = mul(pos, matW);
	outData.eyev = eyePos - posw;

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	//float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 diffuse;
	float4 ambient;
	//float4 ambient;
	// 鏡面反射関連の処理
	float4 NL = dot(inData.normal, normalize(lightPos));	// ここのlightPosは逆ベクトル
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));
	float4 specular = pow(saturate(dot(reflect, normalize(inData.eyev))), shininess) * specularColor;
	if (isTextured == false) {
		// 拡散反射色（なんか明るいやつ）
		diffuse = lightSource * diffuseColor * inData.color;
		// 環境反射色（なんか暗いやつ）
		ambient = lightSource * diffuseColor * ambientColor;
	}
	else {
		// 拡散反射色（なんか明るいやつ）
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// 環境反射色（なんか暗いやつ）
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
	}

	return diffuse + ambient + specular;
	;
}