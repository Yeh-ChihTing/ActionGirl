#pragma once
#include	<directxmath.h>
#include	<d3d11.h>
#include	"DX11util.h"
#include	"Shader.h"
/*----------------------------------------------------------------------

	�r���{�[�h�N���X	

-----------------------------------------------------------------------*/

class CBillBoard{
	DirectX::XMFLOAT4X4			m_mat;			// �r���{�[�h�p�̍s��
	float						m_x;			// �r���{�[�h�̈ʒu���W
	float						m_y;					
	float						m_z;
	float						m_XSize;		// �r���{�[�h�̂w�T�C�Y
	float						m_YSize;		// �r���{�[�h�̂x�T�C�Y
	float                       m_angle;        //�r���{�[�h�̉�]�p�x
	DirectX::XMFLOAT4			m_Color;		// ���_�J���[�l
	ID3D11Device*				m_dev = nullptr;			// �f�o�C�X
	ID3D11DeviceContext*		m_devcontext = nullptr;		// �f�o�C�X�R���e�L�X�g
	ID3D11ShaderResourceView* 	m_srv = nullptr;			// Shader Resourceview�e�N�X�`��
	ID3D11Buffer*				m_vbuffer = nullptr;		// ���_�o�b�t�@
	ID3D11BlendState*			m_pBlendStateSrcAlpha = nullptr;
	ID3D11BlendState*			m_pBlendStateOne = nullptr;
	ID3D11BlendState*			m_pBlendStateDefault = nullptr;
	ID3D11VertexShader*			m_pVertexShader = nullptr;	// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*			m_pPixelShader = nullptr;	// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*			m_pVertexLayout = nullptr;	// ���_�t�H�[�}�b�g��`
	ID3D11RasterizerState*	g_rasterStateFrontCulling = nullptr;
	// ���_�t�H�[�}�b�g
	struct MyVertex{
		float				x,y,z;
		DirectX::XMFLOAT4	color;
		float				tu,tv;
		
	};

	MyVertex				m_Vertex[4];			// �r���{�[�h�̒��_���W
private:
	// �r���{�[�h�̒��_���W���v�Z
	void CalcVertex();
	// �r���{�[�h�p�̍s��𐶐�
	void CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& cameramat);
	// �`��
	void Draw();
	// �\�[�X�A���t�@��ݒ肷��
	void SetBlendStateSrcAlpha();
	// �u�����h�X�e�[�g�𐶐�����
	void CreateBlendStateSrcAlpha();
	// ���Z������ݒ肷��
	void SetBlendStateOne();
	// �u�����h�X�e�[�g�i���Z�����j�𐶐�����
	void CreateBlendStateOne();
	// �f�t�H���g�̃u�����h�X�e�[�g��ݒ肷��
	void SetBlendStateDefault();
	// �f�t�H���g�̃u�����h�X�e�[�g�𐶐�����
	void CreateBlendStateDefault();
public:
	CBillBoard():m_x(0),m_y(0),m_z(0),m_srv(nullptr),m_dev(nullptr), m_devcontext(nullptr){
	}

	bool Init(float x, float y, float z, float xsize, float ysize, DirectX::XMFLOAT4 color){
		m_x = x;
		m_y = y;
		m_z = z;
		m_XSize = xsize;
		m_YSize = ysize;
		m_Color = color;
		
		// �f�o�C�X�擾
		m_dev = GetDX11Device();
		// �f�o�C�X�R���e�L�X�g�擾
		m_devcontext = GetDX11DeviceContext();

		// ���_�f�[�^�̒�`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);
		// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
		bool sts = CreateVertexShader(m_dev,
			"shader/vstriangle.fx",
			"main",
			"vs_4_0",
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
			"shader/psbillboard.fx", 
			"main", 
			"ps_4_0",
			&m_pPixelShader);

		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
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

		CalcVertex();						// �r���{�[�h�p�̒��_�f�[�^�쐬	

		CreateBlendStateSrcAlpha();			// �A���t�@�u�����f�B���O�p�u�����h�X�e�[�g����
		CreateBlendStateOne();				// ���Z�����p�̃u�����h�X�e�[�g����
		CreateBlendStateDefault();			// �f�t�H���g�̃u�����h�X�e�[�g����

		return true;
	}

	// �f�X�g���N�^
	virtual ~CBillBoard(){
		if (m_vbuffer != nullptr) {
			m_vbuffer->Release();
			m_vbuffer = nullptr;
		}

		if(m_srv!=nullptr){
			m_srv->Release();
			m_srv = nullptr;
		}

		if (m_pBlendStateOne != nullptr) {
			m_pBlendStateOne->Release();
			m_pBlendStateOne = nullptr;
		}

		if (m_pBlendStateSrcAlpha != nullptr) {
			m_pBlendStateSrcAlpha->Release();
			m_pBlendStateSrcAlpha = nullptr;
		}

		if (m_pBlendStateDefault != nullptr) {
			m_pBlendStateDefault->Release();
			m_pBlendStateDefault = nullptr;
		}

		if (m_pPixelShader != nullptr) {
			m_pPixelShader->Release();
			m_pPixelShader = nullptr;
		}

		if (m_pVertexShader != nullptr) {
			m_pVertexShader->Release();
			m_pVertexShader = nullptr;
		}

		if (m_pVertexLayout != nullptr) {
			m_pVertexLayout->Release();
			m_pVertexLayout = nullptr;
		}
	}

	// �t�u���W���Z�b�g����
	void SetUV(float u[],float v[]){
		m_Vertex[0].tu = u[0];
		m_Vertex[0].tv = v[0];

		m_Vertex[1].tu = u[1];
		m_Vertex[1].tv = v[1];

		m_Vertex[2].tu = u[2];
		m_Vertex[2].tv = v[2];

		m_Vertex[3].tu = u[3];
		m_Vertex[3].tv = v[3];

		CalcVertex();						// �r���{�[�h�p�̒��_�f�[�^�쐬	
	}

	// �ʒu���w��
	void SetPosition(float x,float y,float z);

	// �r���{�[�h��`��
	void DrawBillBoard(const DirectX::XMFLOAT4X4& cameramat);

	// �r���{�[�h�`����Z����
	void DrawBillBoardAdd(const DirectX::XMFLOAT4X4& cameramat);

	// �r���{�[�h��Z���𒆐S�ɂ��ĉ�]�����ĕ`��
	void DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &cameramat, float radian);
	
	// �T�C�Y���Z�b�g
	void SetSize(float x, float y);

	// �J���[���Z�b�g
	void SetColor(DirectX::XMFLOAT4 col);

	//	�e�N�X�`���ǂݍ���
	bool LoadTexTure(const char* filename);

};