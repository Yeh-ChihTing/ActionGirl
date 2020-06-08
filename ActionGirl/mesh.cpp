#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "memory.h"
#include "CDirectxGraphics.h"
#include "shader.h"
#include "vertexproto.h"
#include "Mesh.h"
#include "obb.h"
#include "dx11mathutil.h"

using namespace std;
using namespace DirectX;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	this->m_textures = textures;
	this->setupMesh();
	//		this->m_deformvertices.resize(vertices.size());			// GPU�Ή��ŕs�v



	std::vector<XMFLOAT3> tempv;

	// �{�[����o�^
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = 0; j < 4; j++) {
			if (vertices[i].m_BoneName[j]!="") {
				m_bonevertices[vertices[i].m_BoneName[j]] = tempv;
			}
		}
	}

	// ���_��o�^
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = 0; j < 4; j++) {
			// �{�[�����݃`�F�b�N
			auto itr = m_bonevertices.find(vertices[i].m_BoneName[j].c_str());
			if (itr != m_bonevertices.end()) {
				std::vector<XMFLOAT3>& vv = m_bonevertices[vertices[i].m_BoneName[j].c_str()];
				vv.push_back(vertices[i].m_Pos);
			}
		}
	}

	// OBB�쐬
	for (auto data : m_bonevertices) {
		COBB* pb = new COBB();
		pb->CalculateOBB(data.second);
		pb->CreateBoxMesh();
		pb->SetName(data.first.data());			// ���O(�ްݖ�)���Z�b�g
		m_obbvectorcontainer.push_back(pb);
	}
}

void Mesh::DrawOBB(std::map<std::string, BONE>& bone) {

	for (int i = 0; i < m_obbvectorcontainer.size(); i++) {
		// �{�[�����݃`�F�b�N(�A�j���[�V�����ɂ̂ݑ��݂���{�[�����������ꍇ�͖�������)
		auto itr = bone.find(m_obbvectorcontainer[i]->GetName());
		if (itr != bone.end()) {
			m_obbvectorcontainer[i]->Draw();
		}
	}
}

void Mesh::UpdateOBB(std::map<std::string, BONE>& bone, const XMFLOAT4X4& mtxworld) {

	for (int i = 0; i < m_obbvectorcontainer.size(); i++) {

		// �{�[�����݃`�F�b�N(���f���̒��_�f�[�^�ɑ��݂���{�[���ɂ̂ݑΉ�)
//		auto itr = bone.find(m_obbvectorcontainer[i]->GetName());
//		if (itr != bone.end()) {

			// �{�[���ɑΉ������s������o��
			aiMatrix4x4 aimtx = bone[m_obbvectorcontainer[i]->GetName()].Matrix;

			XMFLOAT4X4 dxmtx;
			dxmtx = DX11MtxaiToDX(aimtx);		// aiMatrix��DX�s���

			// ���[���h�ϊ��s����|���Z
			DX11MtxMultiply(dxmtx, dxmtx, mtxworld);
			// OBB�̎p���E�ʒu���X�V
			m_obbvectorcontainer[i]->Update(dxmtx);
//		}
	}
}

void Mesh::Draw(ID3D11DeviceContext *devcon,bool LineOn)
{
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
		// ���_�o�b�t�@���Z�b�g
		devcon->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		// �C���f�b�N�X�o�b�t�@���Z�b�g
		devcon->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// �g�|���W�[���Z�b�g
		if (!LineOn) 
		{
			devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
		}
		else 
		{
			devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		}
		// SRV���Z�b�g
		if (m_textures.size() >= 1) {
			devcon->PSSetShaderResources(0, 1, &m_textures[0].texture);
		}
		// �C���f�b�N�X�o�b�t�@�𗘗p���ĕ`��
		devcon->DrawIndexed(static_cast<unsigned int>(m_indices.size()), 0, 0);
}

void Mesh::Close()
{
	for (int i = 0; i < m_obbvectorcontainer.size(); i++) {
		delete m_obbvectorcontainer[i];
	}
	m_obbvectorcontainer.erase(m_obbvectorcontainer.begin(), m_obbvectorcontainer.end());

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}

bool Mesh::setupMesh()
{
		// ���_�o�b�t�@����
		bool sts = CreateVertexBufferWrite(m_dev,
		static_cast<unsigned int>(sizeof(Vertex)),			// �X�g���C�h
		static_cast<unsigned int>(m_vertices.size()),		// ���_��
		m_vertices.data(),									// ���_�f�[�^
		&m_VertexBuffer);
		if (!sts) {
			return false;
		}

		// �C���f�b�N�X�o�b�t�@����
		sts = CreateIndexBuffer(m_dev,
			static_cast<unsigned int>(m_indices.size()),
			m_indices.data(),
			&m_IndexBuffer);
		if (!sts) {
			return false;
		}
		return true;
}

void Mesh::updatevertexbuffer() {

	HRESULT hr;

	ID3D11DeviceContext* devicecontext;

	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	D3D11_MAPPED_SUBRESOURCE pData;

	// �萔�o�b�t�@�X�V
	hr = devicecontext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		//			memcpy_s(pData.pData, pData.RowPitch, (void*)m_deformvertices.data(), sizeof(aiVertex)*m_deformvertices.size());
		memcpy_s(pData.pData, pData.RowPitch, (void*)m_vertices.data(), sizeof(Vertex)*m_vertices.size());
		devicecontext->Unmap(m_VertexBuffer, 0);
	}
}