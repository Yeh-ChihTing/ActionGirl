#pragma once

#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include <map>
#include "obb.h"
#include "vertexproto.h"

// �e�N�X�`���\����
struct Texture {
	std::string type;							// �e�N�X�`�����
	std::string path;							// �e�N�X�`���p�X��
	ID3D11ShaderResourceView *texture;			// SRV
};

class Mesh {
public:
	std::vector<Vertex> m_vertices;			// ���_�f�[�^
	std::vector<unsigned int> m_indices;	// �C���f�b�N�X
	std::vector<Texture> m_textures;		// �e�N�X�`��
	ID3D11Device* m_dev;					// �f�o�C�X	

	std::unordered_map<std::string, std::vector<DirectX::XMFLOAT3>>	m_bonevertices;		// �{�[�����̒��_�f�[�^

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw(ID3D11DeviceContext *devcon, bool LineOn);
	std::vector<COBB*> m_obbvectorcontainer;		// OBB���X�g
	void Close();

	void DrawOBB(std::map<std::string, BONE>& bone);
	void UpdateOBB(std::map<std::string, BONE>& bone, const XMFLOAT4X4& mtxworld);

	std::vector<Vertex> GetVertices() {
		return m_vertices;
	}

	std::vector<unsigned int> GetIndices() {
		return m_indices;
	}
private:
	ID3D11Buffer*  m_VertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer*  m_IndexBuffer;			// �C���f�b�N�X�o�b�t�@

	bool setupMesh();
	void updatevertexbuffer();
};