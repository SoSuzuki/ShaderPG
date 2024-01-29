//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[
Texture2D		normalTex : register(t1);

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer gmodel:register(b0)
{
	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matW;			// ���[���h�s��
	float4x4	matNormal;		// ���[���h�s��	matW�������
	float4		diffuseColor;	// �}�e���A���̐F => �g�U���ˌ�
	float4		ambientColor;	// ����
	float4		specularColor;	// ���ʔ��ˌ�
	float		shininess;
	int			hasTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
	int			hasNormalMap;	// �m�[�}���}�b�v�����邩�ǂ���
};

cbuffer gmodel:register(b1)
{
	float4		lightPos;
	float4		eyePos;			// ���_���W
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos		:	SV_POSITION;	//�ʒu
	float4 eyev		:	POSITION;		//���[���h���W�ɕϊ����ꂽ�����x�N�g��
	float4 Neyev	:	POSITION1;		//�m�[�}���}�b�v�p�̐ڋ�Ԃɕϊ����ꂽ�����x�N�g��
	float4 normal	:	POSITION2;		//�@���x�N�g��
	float4 light	:	POSITION3;
	float4 color	:	POSITION4;		//�F->�P�x�i���邳�j
	float2 uv		:	TEXCOORD;		//UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;

	float3  binormal = cross(normal, tangent);

	// w�̏��͓����Ăė~�����Ȃ��̂�0 => ���s�ړ����݂��ƕςȕ����Ɍ����Ă��܂�
	normal.w = 0;
	//�@������]
	normal = mul(normal, matNormal);
	normal = normalize(normal);	//�@���x�N�g�������[�J�����W�ɕϊ��������
	outData.normal = normal;

	tangent.w = 0;
	//�@������]
	tangent = mul(tangent, matNormal);
	tangent = normalize(tangent);	//�@���x�N�g�������[�J�����W�ɕϊ��������

	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	float4 posw = mul(pos, matW);
	outData.eyev = normalize(posw - eyePos);

	outData.Neyev.x = dot(outData.eyev, tangent);	//�ڋ�Ԃ̎����x�N�g��
	outData.Neyev.y = dot(outData.eyev, binormal);
	outData.Neyev.z = dot(outData.eyev, outData.normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPos);
	light.w = 0;
	light = normalize(light);

	outData.color = mul(light, outData.normal);
	outData.color.w = 0.0;

	outData.light.x = dot(light, tangent);//�ڋ�Ԃ̌����x�N�g��
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, outData.normal);
	outData.light.w = 0;

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
#if 0
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	//float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 diffuse;
	float4 ambient;
	// ���ʔ��ˊ֘A�̏���	NL��reflect��reflect�֐��ŉ������邪�A�l�����̗�Ƃ��ăR�����g��
	//float4 NL = dot(inData.normal, normalize(lightPos));	// ������lightPos�͋t�x�N�g��
	//float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));

	float4 specular = pow(saturate(dot(reflect(normalize(lightPos),inData.normal), normalize(inData.eyev))), shininess) * specularColor;
	if (hasTexture == false) {
		// �g�U���ːF
		diffuse = lightSource * diffuseColor * inData.color;
		// �����ːF
		ambient = lightSource * diffuseColor * ambientColor;
	}
	else {
		// �g�U���ːF
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// �����ːF
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