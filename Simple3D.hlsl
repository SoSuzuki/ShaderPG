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
	float4x4	matNormal;		// �@���̕ϊ��s��i��]�Ɗg��̋t�s��j
	float4x4	matW;			// ���[���h�s��
	float4		vecLightDir;	// ���C�g�̕����x�N�g��
	float4		diffuseColor;	// �}�e���A���̐F => �g�U���ˌW��
	float4		ambientColor;	// �e�̐F => �����ˌW��
	float4		specularColor;	// �n�C���C�g�̐F => ���ʔ��ˌW��
	float4		eyePos;			// ���_���W
	float		shininess;		// �n�C���C�g�̋���
	bool		isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos		:	SV_POSITION;	//�ʒu
	float4 normal	:	TEXCOORD0;
	float2 uv		:	TEXCOORD2;		//UV���W
	float4 eyev		:	TEXCOORD1;		//���_�i�J�����ʒu�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;
	
	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);

	//�@������]
	normal.w = 0;
	normal = mul(normal, matNormal);
	outData.normal = normal;

	float4 light = normalize(lightPos);	//�����̍��W
	light = normalize(light);

	outData.color = saturate(dot(normal, light));
	outData.color.a = 1;
	float4 posw = mul(pos, matW);
	outData.eyev = eyePos - posw;

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	// ���C�g�̌���
	float4 lightSource = vecLightDir;	// �O���[�o���ϐ��������U���[�J����
	lightSource = normalize(lightSource);

	// �@���̏C��
	inData.normal = normalize(inData.normal);

	// �g�U���ˌ�
	float4 shade = saturate(dot(inData.normal, -lightSource));
	shade.a = 1;	// �Â��Ɠ����c����ă��͋����I�ɂP
	float4 diffuse;
	//float4 ambient;
	// ���ʔ��ˊ֘A�̏���
	float4 NL = saturate(dot(inData.normal, normalize(lightPos)));
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));
	float4 specular = pow(saturate(dot(reflect, normalize(inData.eyev))), 8);
	if (isTexture == false) {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * diffuseColor;
		// �����ːF�i�Ȃ񂩈Â���j
		//ambient = lightSource * diffuseColor * ambientSource;
	}
	else {
		// �g�U���ːF�i�Ȃ񂩖��邢��j
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv);
		// �����ːF�i�Ȃ񂩈Â���j
		//ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
	}
	return diffuse * inData.color + diffuse * ambientSource + specular;
}