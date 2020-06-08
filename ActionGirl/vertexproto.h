#pragma once
#include	<directxmath.h>
#include	<assimp/Importer.hpp>

typedef	int32_t VertexSuu; // ���_��
struct Vertex {
	DirectX::XMFLOAT3 m_Pos;		// �ʒu
	DirectX::XMFLOAT3 m_Normal;		// �@��
	DirectX::XMFLOAT2 m_Tex;		// UV���W
	int				m_BoneIndex[4];	//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float			m_BoneWeight[4];	// �E�F�C�g�l
	int				m_BoneNum;		// �{�[����
	std::string		m_BoneName[4];	//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
};

typedef	int32_t IndexSuu;     // �C���f�b�N�X�̐��B
typedef	uint32_t Index;       // �C���f�b�N�X�B

struct BONE
{
	//std::string Name;
	unsigned int idx = -1;			// �{�[���z��̃C���f�b�N�X�ԍ�
	aiMatrix4x4 Matrix;				// �����z�u�s��
	aiMatrix4x4 AnimationMatrix;	// �{�[���s��
	aiMatrix4x4 OffsetMatrix;		// �{�[���I�t�Z�b�g�s��
};

struct ConstantBufferBoneMatrix		// �{�[���s��i�[�p�̃R���X�^���g�o�b�t�@
{
	XMFLOAT4X4  mBoneMatrix[400];
};
