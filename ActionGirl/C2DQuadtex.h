#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"DX11Settransform.h"
#include	"Shader.h"
#include	"dx11mathutil.h"
#include    "game.h"
#include    "RenderToTexture.h"
using namespace DirectX;

class C2DBaseTex {
protected:
	struct ConstantBufferViewPort
	{
		XMFLOAT4	Viewporthidth;			// �r���[�|�[�g��
		XMFLOAT4	Viewportheight;			// �r���[�|�[�g����
	};

	struct Vertex {
		DirectX::XMFLOAT3	Pos;			// ���W
		DirectX::XMFLOAT4	Color;			// ���_�J���[
		DirectX::XMFLOAT2	Tex;			// �e�N�X�`�����W
	};

	ID3D11Buffer*			m_pVertexBuffer = nullptr;	// ���_�o�b�t�@
	ID3D11VertexShader*		m_pVertexShader = nullptr;	// ���_�V�F�[�_�[
	ID3D11PixelShader*		m_pPixelShader = nullptr;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*		m_pVertexLayout = nullptr;	// ���_���C�A�E�g

	ID3D11Device*			m_dev = nullptr;			// DIRECT3DDEVICE11�f�o�C�X
	ID3D11DeviceContext*	m_devcontext = nullptr;		// DIRECT3DDEVICE11�f�o�C�X�R���e�L�X�g

	XMFLOAT4				m_col;
	ID3D11Buffer*			m_pConstantBufferViewPort = nullptr;	// �R���X�^���g�o�b�t�@2
	ConstantBufferViewPort  m_CBViewPort;				// b5

	ID3D11ShaderResourceView* m_srv = nullptr;	// shader resource view
	ID3D11RasterizerState*	g_rasterStateFrontCulling = nullptr;

public:
	bool Init(std::string filaname) {

		m_dev = CDirectXGraphics::GetInstance()->GetDXDevice();
		m_devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		// �R���X�^���g�o�b�t�@�쐬
		bool sts = CreateConstantBuffer(
			m_dev,			// �f�o�C�X
			sizeof(ConstantBufferViewPort),	// �T�C�Y
			&m_pConstantBufferViewPort);	// �R���X�^���g�o�b�t�@�r���[�|�[�g
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(constant buffer viewport) error", "Error", MB_OK);
			return false;
		}

		// ���_�f�[�^�̒�`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int numElements = ARRAYSIZE(layout);

		// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
		sts = CreateVertexShader(m_dev,
			"shader/2dquadtexvs.fx",
			"main",
			"vs_5_0",
			layout,
			numElements,
			&m_pVertexShader,
			&m_pVertexLayout);
		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
			m_dev,							// �f�o�C�X�I�u�W�F�N�g
			"shader/2dquadtexps.fx",
			"main",
			"ps_5_0",
			&m_pPixelShader);
		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		// �V�F�[�_�[���\�[�X�r���[����
		sts = CreatetSRVfromFile(filaname.c_str(), m_dev, m_devcontext, &m_srv);
		if (!sts) {
			MessageBox(nullptr, "CreatetSRVfromFile error", "error", MB_OK);
			return false;
		}

		ID3D11Device* device;
		device = CDirectXGraphics::GetInstance()->GetDXDevice();

		D3D11_RASTERIZER_DESC rasterDesc;

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		//rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;		// ���C���t���[���ɂ������Ƃ�
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;


		HRESULT hr = device->CreateRasterizerState(
			&rasterDesc,
			&g_rasterStateFrontCulling);

		return true;
	}


	void UpdateCB() {

		int vph = CDirectXGraphics::GetInstance()->GetViewPortHeight();
		int vpw = CDirectXGraphics::GetInstance()->GetViewPortWidth();


		m_CBViewPort.Viewportheight.x =  static_cast<float>(vph);
		m_CBViewPort.Viewporthidth.x = static_cast<float>(vpw);

		m_devcontext->UpdateSubresource(m_pConstantBufferViewPort, 0, nullptr, &m_CBViewPort, 0, 0);		// �R���X�^���g�o�b�t�@�X�V
		m_devcontext->VSSetConstantBuffers(5, 1, &m_pConstantBufferViewPort);				// �R���X�^���g�o�b�t�@��b5���W�X�^�փZ�b�g
	}

	void Uninit() {
		if (m_pConstantBufferViewPort) {
			m_pConstantBufferViewPort->Release();
			m_pConstantBufferViewPort = nullptr;
		}
		// �s�N�Z���V�F�[�_�[���
		if (m_pPixelShader) {
			m_pPixelShader->Release();
			m_pPixelShader = nullptr;
		}

		// ���_�V�F�[�_�[���
		if (m_pVertexShader) {
			m_pVertexShader->Release();
			m_pVertexShader = nullptr;
		}

		// ���_���C�A�E�g���
		if (m_pVertexLayout) {
			m_pVertexLayout->Release();
			m_pVertexLayout = nullptr;
		}

		// �r�q�u���
		if (m_srv) {
			m_srv->Release();
			m_srv = nullptr;
		}
	}

};

class C2DQuadTex :public C2DBaseTex{
private:

	float	m_x = 0;
	float	m_y = 0;

	Vertex				m_Vertex[4];		// ���_���W���S

	// �w�x����
	void SetVertexData(float x, float y,float width,float height,XMFLOAT4 col, float Tu, float Tv, float th, float Th) {
		// ���_���W�Z�b�g
		m_Vertex[0].Pos.x = x - width / 2.0f;
		m_Vertex[0].Pos.y = y - height / 2.0f;
		m_Vertex[0].Pos.z = 0.0f;
		m_Vertex[0].Color = col;
		m_Vertex[0].Tex.x = Tu;
		m_Vertex[0].Tex.y = Tv;

		m_Vertex[1].Pos.x = m_Vertex[0].Pos.x + width;
		m_Vertex[1].Pos.y = m_Vertex[0].Pos.y;
		m_Vertex[1].Pos.z = 0.0f;
		m_Vertex[1].Color = col;
		m_Vertex[1].Tex.x = Tu+ th;
		m_Vertex[1].Tex.y = Tv;

		m_Vertex[2].Pos.x = m_Vertex[0].Pos.x;
		m_Vertex[2].Pos.y = m_Vertex[0].Pos.y + height;
		m_Vertex[2].Pos.z = 0.0f;
		m_Vertex[2].Color = col;
		m_Vertex[2].Tex.x = Tu;
		m_Vertex[2].Tex.y = Tv + Th;

		m_Vertex[3].Pos.x = m_Vertex[0].Pos.x + width;
		m_Vertex[3].Pos.y = m_Vertex[0].Pos.y + height;
		m_Vertex[3].Pos.z = 0.0f;
		m_Vertex[3].Color = col;
		m_Vertex[3].Tex.x = Tu + th;
		m_Vertex[3].Tex.y = Tv + Th;
	}


public:
	void ChangePosSize(float x, float y, float width, float height, XMFLOAT4 color, float Tu, float Tv, float th, float Th){
		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = m_devcontext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			SetVertexData(x, y, width, height, color,Tu,Tv,th,Th);
			memcpy_s(pData.pData, pData.RowPitch, (void*)(m_Vertex), sizeof(Vertex)*4);
			m_devcontext->Unmap(m_pVertexBuffer, 0);
		}
	}

	void SetPos(float x,float y,float width, float height) {
		m_x = x + width /2.0f;
		m_y = y + height/2.0f;
		
	}

	void Draw(float x, float y,float width, float height , float Tu, float Tv, float th, float Th, XMFLOAT4 color) {
		
		ChangePosSize(x, y, width, height, color, Tu, Tv, th, Th);

		m_x = 0 + width / 2.0f;
		m_y = 0 + height / 2.0f;
		
		XMFLOAT3 trans;
		XMFLOAT4X4 mtx;

		trans.x = m_x;
		trans.y = m_y;
		trans.z = 0;

		DX11MtxTranslation(trans, mtx);
		// ���[���h�ϊ��s��
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

		m_devcontext->RSSetState(g_rasterStateFrontCulling);

		// ���_�o�b�t�@���Z�b�g����
		unsigned int stride = sizeof(Vertex);
		unsigned  offset = 0;
		m_devcontext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		m_devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j

		// ���_���C�A�E�g�Z�b�g
		m_devcontext->IASetInputLayout(m_pVertexLayout);

		m_devcontext->VSSetShader(m_pVertexShader, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
		m_devcontext->PSSetShader(m_pPixelShader, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

		
		m_devcontext->PSSetShaderResources(0, 1, &m_srv);

		m_devcontext->Draw(4, 0);									// �`�悷��C���f�b�N�X��(4)

	}

	void DrawRTT(float x, float y, float width, float height, float Tu, float Tv, float th, float Th, XMFLOAT4 color) {

		ChangePosSize(x, y, width, height, color, Tu, Tv, th, Th);

		m_x = 0 + width / 2.0f;
		m_y = 0 + height / 2.0f;

		XMFLOAT3 trans;
		XMFLOAT4X4 mtx;

		trans.x = m_x;
		trans.y = m_y;
		trans.z = 0;

		DX11MtxTranslation(trans, mtx);
		// ���[���h�ϊ��s��
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

		// ���_�o�b�t�@���Z�b�g����
		unsigned int stride = sizeof(Vertex);
		unsigned  offset = 0;
		m_devcontext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		m_devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j

		// ���_���C�A�E�g�Z�b�g
		m_devcontext->IASetInputLayout(m_pVertexLayout);

		m_devcontext->VSSetShader(m_pVertexShader, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
		m_devcontext->PSSetShader(m_pPixelShader, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

		m_srv = RenderTexture::GetInstance()->GetOutputTexture();
		m_devcontext->PSSetShaderResources(0, 1, &m_srv);

		m_devcontext->Draw(4, 0);									// �`�悷��C���f�b�N�X��(4)

	}


	bool Init(/*float x,float y,float width ,float height,float Tu, float Tv, float th, float Th , XMFLOAT4 color,*/std::string filename) {

		bool sts;

		m_dev = CDirectXGraphics::GetInstance()->GetDXDevice();
		m_devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		// ���N���X�̏�������
		C2DBaseTex::Init(filename);

		//m_col = color;

		// ���_���W�Z�b�g	
		//SetVertexData(x, y, width, height, color, Tu, Tv, th, Th);

		// ���_�o�b�t�@�쐬
		sts = CreateVertexBufferWrite(
			m_dev,
			sizeof(Vertex),			// �P���_������o�C�g��
			4,						// ���_��
			m_Vertex,				// ���_�f�[�^�i�[�������擪�A�h���X
			&m_pVertexBuffer);		// ���_�o�b�t�@
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
			return false;
		}

		// �r���[�|�[�g�l���X�V
		UpdateCB();

		return true;
	}

	

	void Uninit() {

		// ���N���X�̏�������
		C2DBaseTex::Uninit();

		if (m_pVertexBuffer) {
			m_pVertexBuffer->Release();
			m_pVertexBuffer = nullptr;
		}
	}


	
};