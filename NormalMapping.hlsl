//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー
Texture2D		normalTex : register(t1);

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
	int			hasTexture;		// テクスチャ貼ってあるかどうか
	int			hasNormalMap;	// ノーマルマップがあるかどうか
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
	float4 eyev		:	POSITION;		//ワールド座標に変換された視線ベクトル
	float4 Neyev	:	POSITION1;		//ノーマルマップ用の接空間に変換された視線ベクトル
	float4 normal	:	POSITION2;		//法線ベクトル
	float4 light	:	POSITION3;
	float4 color	:	POSITION4;		//色->輝度（明るさ）
	float2 uv		:	TEXCOORD;		//UV座標
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
	outData.uv = uv;

	float3  binormal = cross(normal, tangent);

	// wの情報は入ってて欲しくないので0 => 平行移動込みだと変な方向に向いてしまう
	normal.w = 0;
	//法線を回転
	normal = mul(normal, matNormal);
	normal = normalize(normal);	//法線ベクトルをローカル座標に変換したやつ
	outData.normal = normal;

	tangent.w = 0;
	//法線を回転
	tangent = mul(tangent, matNormal);
	tangent = normalize(tangent);	//法線ベクトルをローカル座標に変換したやつ

	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	float4 posw = mul(pos, matW);
	outData.eyev = normalize(posw - eyePos);

	outData.Neyev.x = dot(outData.eyev, tangent);	//接空間の視線ベクトル
	outData.Neyev.y = dot(outData.eyev, binormal);
	outData.Neyev.z = dot(outData.eyev, outData.normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPos);
	light.w = 0;
	light = normalize(light);

	outData.color = mul(light, outData.normal);
	outData.color.w = 0.0;

	outData.light.x = dot(light, tangent);//接空間の光源ベクトル
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, outData.normal);
	outData.light.w = 0;

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
#if 0
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	//float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 diffuse;
	float4 ambient;
	// 鏡面反射関連の処理	NLもreflectもreflect関数で解決するが、考え方の例としてコメント化
	//float4 NL = dot(inData.normal, normalize(lightPos));	// ここのlightPosは逆ベクトル
	//float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));

	float4 specular = pow(saturate(dot(reflect(normalize(lightPos),inData.normal), normalize(inData.eyev))), shininess) * specularColor;
	if (hasTexture == false) {
		// 拡散反射色
		diffuse = lightSource * diffuseColor * inData.color;
		// 環境反射色
		ambient = lightSource * diffuseColor * ambientColor;
	}
	else {
		// 拡散反射色
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// 環境反射色
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
	}

	return diffuse + ambient + specular;
#else
	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
	float4 diffuse;
	float4 ambient;

	if (hasNormalMap)
	{
		//inData.light = normalize(inData.light);
		float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2.0f - 1.0f;
		tmpNormal.w = 0;
		tmpNormal = normalize(tmpNormal);

		float4 NL = clamp(dot(tmpNormal, inData.light), 0, 1);

		float4 light = normalize(lightPos);
		light = normalize(light);
		float4 reflection = reflect(light, tmpNormal);
		float4 specular = pow(saturate(dot(reflection, inData.Neyev)), 2) * specularColor;

		if (hasTexture != 0)
		{
			diffuse = g_texture.Sample(g_sampler, inData.uv) * NL;
			ambient = g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		else
		{
			diffuse = diffuseColor * NL;
			ambient = diffuseColor * ambientColor;
		}
		return diffuse + ambient + specular;
	}
	else
	{
		float4 reflection = reflect(normalize(lightPos), inData.normal);

		float4 specular = pow(saturate(dot(normalize(reflection), inData.eyev)), shininess) * specularColor;
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
		float4 result = diffuse + ambient + specular;
		if (hasTexture)
			result.a = inData.uv.x;
		return result;
	}
#endif
}