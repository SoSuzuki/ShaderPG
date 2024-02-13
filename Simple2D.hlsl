#if 1
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
	VS_OUT outData = (VS_OUT)0;

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


	//float4 result = diffuse + ambient + specular;
	//result.a = (result.r + result.g + result.b) / 3;
	//return result;
}
#else
//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D       g_texture : register(t0);   //テクスチャー
SamplerState    g_sampler : register(s0);   //サンプラー
Texture2D       normalTex : register(t1);

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer gmodel:register(b0)
{
    float4x4    matWVP;         // ワールド・ビュー・プロジェクションの合成行列
    float4x4    matW;           // ワールド行列
    float4x4    matNormal;      //ノーマルのローカルへの変換行列から平行移動成分をとったやつ
    float4      diffuseColor;   //マテリアルの色＝拡散反射係数
    float4      ambientColor;   //環境光
    float4      specularColor;  //鏡面反射＝ハイライトの係数
    float       shininess;      //ハイライトの広がりの大きさ
    int         hasTextured;    //テクスチャーが貼られているかどうか
    int         hasNormalMap;   //ノーマルマップがあるかどうか
    float       scrollX;
    float       scrollY;
};

cbuffer gmodel:register(b1)
{
    float4      lightPos;      //光源の位置（平行光源の時は、その位置から原点へのベクトル
    float4      eyePos;        //視点位置＝カメラ位置
};


//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = (float2)uv;

	float3 binormal = cross(tangent, normal);
	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	outData.normal = normalize(mul(normal, matNormal));
	normal.w = 0;

	tangent = mul(tangent, matNormal);
	tangent = normalize(tangent);
	tangent.w = 0;

	float4 eye = normalize(mul(pos, matW) - eyePos);
	outData.eyev = eye;

	outData.Neyev.x = dot(eye, tangent);
	outData.Neyev.y = dot(eye, binormal);
	outData.Neyev.z = dot(eye, outData.normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPos);
	light.w = 0;
	light = normalize(light);

	outData.color = mul(light, outData.normal);
	outData.color.w = 0.0f;

	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, outData.normal);
	outData.light.w = 0;

	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
	float4 diffuse;
	float4 ambient;
	float4 Specular;

	float2 tmpNormalUV = inData.uv;
	tmpNormalUV.x = tmpNormalUV.x + scrollX;
	tmpNormalUV.y = tmpNormalUV.y + scrollY;

	if (hasNormalMap) {
		float4 tmpNormal = normalTeX.Sample(g_sampler, tmpNormalUV) * 2.0f - 1.0f;
		float4 tmpNormal2 = normalTeX.Sample(g_sampler, inData.uv + (float2)(scrollX * 0.4 + (-scrollY) * 0.4)) * 2.0f - 1.0f;
		tmpNormal.w = 0;
		tmpNormal2.w = 0;
		tmpNormal = normalize(tmpNormal + tmpNormal2);


		float4 NL = clamp(dot(normalize(inData.light), tmpNormal), 0, 1);

		float4 reflection = reflect(normalize(inData.light), tmpNormal);
		float4 Specular = pow(saturate(dot(reflection, inData.Neyev)), shininess) * speculerColor;
		if (hasTexture != 0)
		{
			diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * NL;
			ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		else
		{
			diffuse = lightSource * diffuseColor * NL;
			ambient = lightSource * diffuseColor * ambientColor;
		}
		return  diffuse + ambient + Specular;
	}
	else
	{
		float4 reflection = reflect(normalize(lightPos), inData.normal);

		float4 Specular = pow(saturate(dot(reflection, inData.eyev)), shininess) * speculerColor;
		if (hasTexture == 0)
		{
			diffuse = lightSource * diffuseColor * inData.color;
			ambient = lightSource * diffuseColor * ambientColor;
		}
		else
		{
			diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
			ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		float4 result = Specular;
		result.a = ((result.r + result.g + result.b) / 3) * 1.5;

		return result;
	}
}
#endif