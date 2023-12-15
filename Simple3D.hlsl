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
	float4x4	matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	matNormal;		// 法線の変換行列（回転と拡大の逆行列）
	float4x4	matW;			// ワールド行列
	float4		vecLightDir;	// ライトの方向ベクトル
	float4		diffuseColor;	// マテリアルの色 => 拡散反射係数
	float4		ambientColor;	// 影の色 => 環境反射係数
	float4		specularColor;	// ハイライトの色 => 鏡面反射係数
	float4		eyePos;			// 視点座標
	float		shininess;		// ハイライトの強さ
	bool		isTexture;		// テクスチャ貼ってあるかどうか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos		:	SV_POSITION;	//位置
	float4 normal	:	TEXCOORD0;
	float2 uv		:	TEXCOORD2;		//UV座標
	float4 eyev		:	TEXCOORD1;		//視点（カメラ位置）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;
	
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);

	//法線を回転
	normal.w = 0;
	normal = mul(normal, matNormal);
	outData.normal = normal;

	float4 light = normalize(lightPos);	//光源の座標
	light = normalize(light);

	outData.color = saturate(dot(normal, light));
	outData.color.a = 1;
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
	// ライトの向き
	float4 lightSource = vecLightDir;	// グローバル変数だから一旦ローカルに
	lightSource = normalize(lightSource);

	// 法線の修正
	inData.normal = normalize(inData.normal);

	// 拡散反射光
	float4 shade = saturate(dot(inData.normal, -lightSource));
	shade.a = 1;	// 暗いと透明…よってαは強制的に１
	float4 diffuse;
	//float4 ambient;
	// 鏡面反射関連の処理
	float4 NL = saturate(dot(inData.normal, normalize(lightPos)));
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));
	float4 specular = pow(saturate(dot(reflect, normalize(inData.eyev))), 8);
	if (isTexture == false) {
		// 拡散反射色（なんか明るいやつ）
		diffuse = lightSource * diffuseColor;
		// 環境反射色（なんか暗いやつ）
		//ambient = lightSource * diffuseColor * ambientSource;
	}
	else {
		// 拡散反射色（なんか明るいやつ）
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv);
		// 環境反射色（なんか暗いやつ）
		//ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
	}
	return diffuse * inData.color + diffuse * ambientSource + specular;
}