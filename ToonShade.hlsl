//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

Texture2D g_toon_texture : register(t1);
Texture2D g_toon_texture2 : register(t2);

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
	bool		isTextured;		// �e�N�X�`���\���Ă��邩�ǂ���
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
	float4 color	:	COLOR;			//�F->�P�x�i���邳�j
	float4 eyev		:	POSITION1;		//���_�i�J�����ʒu�j
	float4 normal	:	POSITION2;
	float4 light	:	POSITION3;
	float2 uv		:	TEXCOORD;		//UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;
	pos = pos + normal * 0.05;
	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	// w�̏��͓����Ăė~�����Ȃ��̂�0
	normal.w = 0;
	//�@������]
	normal = mul(normal, matNormal);
	outData.normal = normal;

	float4 light = normalize(lightPos);	//�����̍��W

	outData.color = saturate(dot(normal, light));
	//outData.color.a = 1;
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
	float4 lightSource = float4(1.0,1.0,1.0,1.0);
	//float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 diffuse;
	float4 ambient;
	// ���ʔ��ˊ֘A�̏���	NL��reflect��reflect�֐��ŉ������邪�A�l�����̗�Ƃ��ăR�����g��
	//float4 NL = dot(inData.normal, normalize(lightPos));	// ������lightPos�͋t�x�N�g��
	//float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));

	float4 specular = pow(saturate(dot(reflect(normalize(lightPos),inData.normal), normalize(inData.eyev))), shininess) * specularColor;

	// ���̕ӂŊg�U���˂̒l���c
/*	// �v�Z���ł�����Ⴄ���(�񐄏�)
	float4 n1 = float4(1 / 4.0, 1 / 4.0, 1 / 4.0, 1);
	float4 n2 = float4(2 / 4.0, 2 / 4.0, 2 / 4.0, 1);
	float4 n3 = float4(3 / 4.0, 3 / 4.0, 3 / 4.0, 1);
	float4 n4 = float4(4 / 4.0, 4 / 4.0, 4 / 4.0, 1);

	float tI = 0.1 * step(n1, inData.color) + 0.3 * step(n2, inData.color)
		+ 0.3 * step(n3, inData.color) + 0.4 * step(n4, inData.color);
*/
	float2 uv;
	uv.x = inData.color.x;
	uv.y = 0;

	// tI�����̂܂�return���Ď���
	float tI = g_toon_texture.Sample(g_sampler, uv);

	if (isTextured == false) {
		// �g�U���ːF
		diffuse = lightSource * diffuseColor * tI;
		// �����ːF
		ambient = lightSource * diffuseColor * ambientColor;
	}
	else {
		// �g�U���ːF
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
		// �����ːF
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
	}

	//return diffuse + ambient + specular;
	return diffuse + ambient;


	// �֊s = �����x�N�g���Ɩʂ̖@���̂Ȃ��p�x(cos)��90�x�t��
	//if (abs(dot(normalize(inData.eyev),inData.normal)) < 0.2)
	//	return	float4(0, 0, 0, 1);
	//else
	//	return float4(1, 1, 1, 1);

	// if�g��Ȃ��p�^�[�����s�� 
	//float tJ = g_toon_texture2.Sample(g_sampler, uv)
}