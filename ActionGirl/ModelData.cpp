#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp/cimport.h>
#include <unordered_map>
#include <vector>
#include "animationdataassimp.h"
#include "Shader.h"
#include "memory.h"
#include "ModelData.h"
#include "vertexproto.h"
#include "DX11Settransform.h"
#include "CDirectxGraphics.h"

const int INTERPOLATENUM = 2;			// ��Ԑ�

std::vector<Texture> ModelData::loadMaterialTextures(
	aiMaterial* mtrl, 
	aiTextureType type, 
	std::string typeName, 
	const aiScene * scene)
{
	std::vector<Texture> textures;		// ���̃}�e���A���Ɋ֘A�Â���DIFFUSE�e�N�X�`���̃��X�g
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	devcon = CDirectXGraphics::GetInstance()->GetImmediateContext();
	
	// �}�e���A������e�N�X�`�������擾��(��{��1��)���[�v����
	for (unsigned int i = 0; i < mtrl->GetTextureCount(type); i++)
	{
		aiString str;

		// �}�e���A�����炉�Ԗڂ̃e�N�X�`���t�@�C�������擾����
		mtrl->GetTexture(type, i, &str);

		// �������Ƀ��[�h���ꂽ�e�N�X�`���ł���΃��[�h���X�L�b�v����
		bool skip = false;

		// ���[�h�ς݃e�N�X�`���������[�v����
		for (unsigned int j = 0; j < m_texturesloaded.size(); j++)
		{
			// �t�@�C�����������ł�������ǂݍ��܂Ȃ�
			if (std::strcmp(m_texturesloaded[j].path.c_str(), str.C_Str()) == 0)
			{
				// �ǂݍ��ݍς݂̃e�N�X�`������DIFFUSE�e�N�X�`���̃��X�g�ɃZ�b�g
				textures.push_back(m_texturesloaded[j]);
				skip = true; 
				break;
			}
		}
		if (!skip)
		{   // �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
			Texture tex;

			std::string filename = std::string(str.C_Str());
			filename = ExtractFileName(filename, '\\');		// �t�@�C�������擾
			filename = m_directory + filename;				// ���\�[�X�f�B���N�g���{�t�@�C����
			//MessageBox(nullptr, filename.c_str(), "load tex", MB_OK);

			bool sts = CreatetSRVfromFile(
				filename.c_str(),
				dev, devcon, &tex.texture);
			if (!sts) {
				MessageBox(nullptr, "Texture couldn't be loaded", "Error!", MB_ICONERROR | MB_OK);
			}

			tex.type = typeName;
			tex.path = str.C_Str();
			// �e�N�X�`������DIFFUSE�e�N�X�`���̃��X�g�ɃZ�b�g
			textures.push_back(tex);
			this->m_texturesloaded.push_back(tex);	// ���̃��f���Ɋ֘A�Â����e�N�X�`�����X�g�ɃZ�b�g
		}
	}
	
	return textures;
}

ModelData::ModelData()
{
}

ModelData::~ModelData()
{

	Exit();
}

void ModelData::CreateBone(aiNode* node)
{
	BONE bone;

	/*
	struct BONE
	{
	//std::string Name;
	aiMatrix4x4 Matrix;						// �����z�u�s��
	aiMatrix4x4 AnimationMatrix;			// �{�[���s��
	aiMatrix4x4 OffsetMatrix;				// �{�[���I�t�Z�b�g�s��
	};

	std::map<std::string, BONE> m_Bone;		//�{�[���f�[�^�i���O�ŎQ�Ɓj
	*/

	// �m�[�h�����L�[�ɂ��ă{�[������ۑ�
	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

bool ModelData::Load(std::string resourcefolder,
	std::string filename)
{
	bool sts = m_assimpscene.Init(filename);
	if (!sts) {
		MessageBox(nullptr, "ModelData load error", "error", MB_OK);
		return false;
	}

	this->m_directory = resourcefolder;		// ���̃��f���̃e�N�X�`�������݂���f�B���N�g��
	
	// �{�[���𐶐�����
	CreateBone(m_assimpscene.GetScene()->mRootNode);

	// �{�[���̔z��ʒu���i�[����
	unsigned int num = 0;
	for (auto& data : m_Bone) {
		data.second.idx = num;
		num++;
	}

	// ai�m�[�h����͂���
	processNode(m_assimpscene.GetScene()->mRootNode, m_assimpscene.GetScene());

	// �{�[���s��i�[�p�̒萔�o�b�t�@�𐶐�����

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	sts = CreateConstantBufferWrite(device, sizeof(ConstantBufferBoneMatrix), &m_constantbufferbonematrix);
	if (!sts) {
		MessageBox(nullptr, "constant buffer create(bonematrix) fail", "error", MB_OK);
	}

	return true;
}

void ModelData::Draw(ID3D11DeviceContext * devcon, XMFLOAT4X4& mtxworld , bool LineOn)
{
	// �A�j���[�V�����f�[�^�������Ă��邩�H
	if (m_assimpscene.HasAnimation())
	{	// �{�[���s���萔�o�b�t�@�ɔ��f������
		UpdateBoneMatrixConstantBuffer();
	}

	for (int i = 0; i < m_meshes.size(); i++)
	{
		// ���[���h�ϊ��s��
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtxworld);
		// �萔�o�b�t�@�Z�b�g����
		m_meshes[i].Draw(devcon,LineOn);
	}
	
}

void ModelData::DrawOBB() {
	// OBB�`��
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].DrawOBB(m_Bone);
	}
}

void ModelData::UpdateOBB(const XMFLOAT4X4& mtxworld) {
	// ���b�V����OBB�X�V
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].UpdateOBB(m_Bone, mtxworld);
	}
}

// ���b�V���̉��
Mesh ModelData::processMesh(aiMesh * mesh, const aiScene * scene,int meshidx)
{
	std::vector<Vertex> vertices;			// ���_
	std::vector<unsigned int> indices;		// �ʂ̍\�����
	std::vector<Texture> textures;			// �e�N�X�`��

	// ���_�����擾
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.m_Pos.x = mesh->mVertices[i].x;
		vertex.m_Pos.y = mesh->mVertices[i].y;
		vertex.m_Pos.z = mesh->mVertices[i].z;

		// �@���x�N�g�������݂��邩�H
		if (mesh->HasNormals()) {
			vertex.m_Normal.x = mesh->mNormals[i].x;
			vertex.m_Normal.y = mesh->mNormals[i].y;
			vertex.m_Normal.z = mesh->mNormals[i].z;
		}
		else {
			vertex.m_Normal.x = 0.0f;
			vertex.m_Normal.y = 0.0f;
			vertex.m_Normal.z = 0.0f;
		}

		// �e�N�X�`�����W�i�O�Ԗځj�����݂��邩�H
		if (mesh->HasTextureCoords(0)) {
			vertex.m_Tex.x = mesh->mTextureCoords[0][i].x;
			vertex.m_Tex.y = mesh->mTextureCoords[0][i].y;
		}

		vertex.m_BoneNum = 0;

		for (unsigned int b = 0; b < 4; b++)
		{
			vertex.m_BoneIndex[b] = -1;
			vertex.m_BoneName[b] = "";
			vertex.m_BoneWeight[b] = 0.0f;
		}

		vertices.push_back(vertex);
	}

	//�{�[���f�[�^������
	for (unsigned int b = 0; b < mesh->mNumBones; b++)
	{
		// ���b�V���Ɋ֘A�Â��Ă�{�[�������擾
		aiBone* bone = mesh->mBones[b];

		// �{�[���I�t�Z�b�g�s����擾
		m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

		// �{�[���Ɋ֘A�Â��Ă��钸�_��I�Ѥ�E�F�C�g�l���Z�b�g����
		for (unsigned int widx = 0; widx < bone->mNumWeights; widx++)
		{
			aiVertexWeight weight = bone->mWeights[widx];

			unsigned int vidx = weight.mVertexId;			// ���̃E�G�C�g�Ɋ֘A�Â��Ă钸�_idx

			// ���b�V���̒��̉��Ԗڂ�
			vertices[vidx].m_BoneWeight[vertices[vidx].m_BoneNum] = weight.mWeight;
			vertices[vidx].m_BoneName[vertices[vidx].m_BoneNum] = bone->mName.C_Str();
			// �Y������{�[�����̃C���f�b�N�X�l���Z�b�g
			vertices[vidx].m_BoneIndex[vertices[vidx].m_BoneNum] = m_Bone[bone->mName.C_Str()].idx;

			vertices[vidx].m_BoneNum++;

			assert(vertices[vidx].m_BoneNum <= 4);
		}
	}

	// �e�N�X�`�������擾����
	if (mesh->mMaterialIndex >= 0)
	{
		// ���̃��b�V���̃}�e���A���C���f�b�N�X�l���擾����
		int	mtrlidx = mesh->mMaterialIndex;

		// �V�[������}�e���A���f�[�^���擾����
		aiMaterial* material = scene->mMaterials[mtrlidx];

		// ���̃}�e���A���Ɋ֘A�Â����e�N�X�`�������o��
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

		// ���̃��b�V���Ŏg�p���Ă���e�N�X�`����ۑ�
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	
	// �ʂ̍\�������擾
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices,textures);
}

void ModelData::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	//�i�e�{�[���Ƃ̑��Έʒu�������s�񁖃{�[���s�񁖃{�[���I�t�Z�b�g�s��j
	aiMatrix4x4 worldMatrix;
	// 
	/*
	TReal a1, a2, a3, a4;   // assimp �s��́A�s�D��
	TReal b1, b2, b3, b4;
	TReal c1, c2, c3, c4;
	TReal d1, d2, d3, d4;
	*/

	worldMatrix = matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void ModelData::Update(
	unsigned int animno, 
	unsigned int animfileno,
	std::vector<AnimationDataAssimp*>& animationcontainer,
	const int INTERPOLATENUM) {
	

	// 0�Ԗڂ̃V�[�������o��
	const aiScene* s = animationcontainer[animfileno]->GetScene();
	
	// �A�j���[�V�����f�[�^�������Ă��邩�H
	if (s->HasAnimations())
	{
		//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
		aiAnimation* animation = s->mAnimations[animno];

		// �{�[���̐��������[�v
		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];

			// �{�[�����݃`�F�b�N(�A�j���[�V�����ɂ̂ݑ��݂���{�[�����������ꍇ�͖�������)
			auto itr = m_Bone.find(nodeAnim->mNodeName.C_Str());        
			if (itr != m_Bone.end()) {
				BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];					

				int f1, f2;

				f1 = m_Frame % nodeAnim->mNumRotationKeys;//�ȈՎ���   
				aiQuaternion rot1 = nodeAnim->mRotationKeys[f1].mValue; // �N�I�[�^�j�I��

				f1 = m_Frame % nodeAnim->mNumPositionKeys;//�ȈՎ���
				aiVector3D pos1 = nodeAnim->mPositionKeys[f1].mValue;

				f2 = m_preFrame % nodeAnim->mNumRotationKeys;//�ȈՎ���  
				aiQuaternion rot2 = nodeAnim->mRotationKeys[f2].mValue; // �N�I�[�^�j�I��

				f2 = m_preFrame % nodeAnim->mNumPositionKeys;//�ȈՎ���	
				aiVector3D pos2 = nodeAnim->mPositionKeys[f2].mValue;

				// ���
				rot1.Interpolate(rot1, rot1, rot2, m_factor);
				pos1.x = pos1.x * (1.0f - m_factor) + pos2.x * (m_factor);
				pos1.y = pos1.y * (1.0f - m_factor) + pos2.y * (m_factor);
				pos1.z = pos1.z * (1.0f - m_factor) + pos2.z * (m_factor);

				bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
			}
		}

		//�ċA�I�Ƀ{�[���}�g���N�X���X�V
		UpdateBoneMatrix(m_assimpscene.GetScene()->mRootNode, aiMatrix4x4());

		// ���b�V����OBB�X�V
//		for (int i = 0; i < m_meshes.size(); i++)
//		{
//			m_meshes[i].UpdateOBB(m_Bone,mtxworld);
//		}
	}
	
	if (m_cnt % INTERPOLATENUM == 0)
	{
		m_preFrame = m_Frame;
		m_Frame++;
		m_factor = 0;
	}

	m_factor = 1.0f / (float)(m_cnt % INTERPOLATENUM + 1);

	m_cnt++;
	

}
void ModelData::setframe()
{
	m_Frame = 0;
}

void ModelData::Exit()
{
	// �e�N�X�`�����\�[�X���������
	for (auto tex :m_texturesloaded)
	{
		if (tex.texture != nullptr) {
			tex.texture->Release();
		}
	}

	// ���b�V���̉��
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Close();
	}

	// �萔�o�b�t�@���
	//m_constantbufferbonematrix->Release();

	// assimp scene ���
	m_assimpscene.Exit();

	// �V�[��(�A�j���[�V�����p��)�̉��
	for (auto s : m_sceneAnimContainer) {
		aiReleaseImport(s);
	}
}

// �m�[�h�̉��
void ModelData::processNode(aiNode * node, const aiScene * scene)
{	
	// �m�[�h���̃��b�V���̐������[�v����
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshindex = node->mMeshes[i];			// �m�[�h��i�Ԗڃ��b�V���̃C���f�b�N�X���擾
		aiMesh* mesh = scene->mMeshes[meshindex];	// �V�[�����烁�b�V���{�̂����o��

		m_meshes.push_back(this->processMesh(mesh, scene,meshindex));
	}

	// �q�m�[�h�ɂ��Ă����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

void ModelData::UpdateBoneMatrixConstantBuffer() {

	ConstantBufferBoneMatrix cb;
	DirectX::XMFLOAT4X4 mtx;

	// �{�[���s���萔�o�b�t�@�ɃZ�b�g
	unsigned int num = 0;
	for (auto data : m_Bone) {
		// aiMatrix4X4��XMFLOAT4X4�֕ϊ�(�]�u����)
		mtx = DX11MtxaiToDX(data.second.Matrix);

		// �]�u����
		DX11MtxTranspose(mtx);

		cb.mBoneMatrix[num] = mtx;
		num++;
	}

	ID3D11DeviceContext* devicecontext;

	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	D3D11_MAPPED_SUBRESOURCE pData;

	// �{�[���s��i�[�p�萔�o�b�t�@�X�V
	HRESULT hr = devicecontext->Map(m_constantbufferbonematrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBufferBoneMatrix));
		devicecontext->Unmap(m_constantbufferbonematrix, 0);
	}

	// GPU�֒萔�o�b�t�@���Z�b�g
	devicecontext->VSSetConstantBuffers(5, 1, &m_constantbufferbonematrix);
}
