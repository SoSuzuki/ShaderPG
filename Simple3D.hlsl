//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

#if 0
//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matNormal;		// ���[���h�s��	matW�������
	float4		diffuseColor;	// �}�e���A���̐F => �g�U���ˌW��
	float4		lightPosition;
	float4		eyePos;			// ���_���W
	bool		isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos		:	SV_POSITION;	//�ʒu
	float4 color	:	COLOR;			//�F->�P�x�i���邳�j
	float4 revvec	:	POSITION0;		//�t�x�N�g��
	float4 eyepos	:	POSITION1;		//���_�i�J�����ʒu�j
	float2 uv		:	TEXCOORD;		//UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION,float4 eyepos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	//�@������]
	normal = mul(normal, matNormal);

	float4 light = float4(1.0, 2.0, -1.5, 0);	//�����̌����i���̍��W���������"����"�j
	light = normalize(light);
	outData.color = clamp(dot(normal, light), 0, 1);

	outData.eyepos = normalize(eyePos - mul(pos, matNormal));
	float4 reverseVec = 2 * normal * dot(normal, lightPosition) - lightPosition;
	outData.revvec = saturate(reverseVec);

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 specularSource = saturate(pow(dot(inData.revvec, inData.eyepos), diffuseColor.w) * 2.0 * inData.color);
	float4 diffuse;
	float4 ambient;
	float4 specular;
	if (isTexture) {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
		// ���ʔ��ːF�i�Ȃ񂩃c���c���Ȃ�j
		specular = lightSource * g_texture.Sample(g_sampler, inData.uv) * specularSource;
	}
	else {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * diffuseColor * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * diffuseColor * ambientSource;
		// ���ʔ��ːF�i�Ȃ񂩃c���c���Ȃ�j
		specular = lightSource * diffuseColor * specularSource;
	}
	return saturate(diffuse + specular + ambient);
}
#else
//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer gmodel:register(b0)
{
	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matW;			// ���[���h�s��
	float4x4	matNormal;		// ���[���h�s��	matW�������
	float4		diffuseColor;	// �}�e���A���̐F => �g�U���ˌW��
	bool		isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
};

cbuffer gmodel:register(b1)
{
	float4		lightPosition;
	float4		eyePosition;	// ���_���W
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos		:	SV_POSITION;	//�ʒu
	float4 color	:	COLOR;			//�F->�P�x�i���邳�j
	float4 eyev		:	POSITION1;		//���_�i�J�����ʒu�j
	float4 normal	:	POSITION2;
	float4 light	:	POSITION3;
	float2 uv		:	TEXCOORD;		//UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : SV_POSITION, float4 eyev : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	//�@������]
	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normalize(normal);
	outData.normal = normal;

	float4 light = normalize(lightPosition);	//�����̌����i���̍��W���������"����"�j
	outData.light = normalize(light);

	outData.color = saturate(dot(normal, light));
	float4 posw = mul(pos, matW);
	outData.eyev = eyePosition - posw;

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 diffuse;
	float4 ambient;
	// ���ʔ��ˊ֘A�̏���
	float4 NL = saturate(dot(inData.normal, normalize(lightPosition)));
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPosition));
	float4 specular = pow(saturate(dot(reflect, normalize(inData.eyev))), 8);
	if (isTexture==0) {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * diffuseColor * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * diffuseColor * ambientSource;
	}
	else {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
	}
	return diffuse + specular + ambient;
}
#endif