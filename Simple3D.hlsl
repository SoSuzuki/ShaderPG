//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matNormal;		// ���[���h�s��	matW�������
	float4		diffuseColor;	// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4		lightDirection;	// ���C�g�̕����x�N�g��
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
	float2 uv		:	TEXCOORD;		//UV���W
};


//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	float4 mulEyePos = eyePos - mul(pos, matNormal);
	//�@������]
	normal = mul(normal, matNormal);

	//float4 light = float4( 1.0, 0.8, -1.5, 0);	// �����̌����i���̍��W���������"����"�j
	float4 light = float4(-1, 0, 0, 0);
	light = normalize(light);
	//outData.color = clamp(dot(normal, light), 0, 1);// �͈͂��i��

	
	float4 reflect = 2 * normal * dot(normal, mulEyePos) - mulEyePos;
	outData.color = clamp(reflect, 0, 1);

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	/*
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 
	float4 diffuse;
	float4 ambient;
	if (isTexture) {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
	}
	else {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * diffuseColor * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * diffuseColor * ambientSource;
	}
	return (diffuse + ambient);

	*/
	/*
	// Postarization
	float4 output = floor(g_texture.Sample(g_sampler,inData.uv) * 8.0) / 8;
	return output;
	*/

	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	// �����i�J�����ʒu�j�𐳋K��
	float4 nEyePos = normalize(eyePos - matNormal);
	float4 diffuse;
	float4 ambient;
	float4 specular;
	if (isTexture) {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
		// ���ʔ��ːF�i�Ȃ񂩃c���c���Ȃ�j
		specular = pow(saturate(dot(outData.color, nEyePos)), diffuseColor);
	}
	else {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * diffuseColor * inData.color;
		// �����ːF�i�Ȃ񂩈Â���j
		ambient = lightSource * diffuseColor * ambientSource;
	}
	return (diffuse + ambient + specular);
}