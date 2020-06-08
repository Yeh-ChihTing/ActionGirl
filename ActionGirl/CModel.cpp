#define NOMINMAX
#include	<windows.h>
#include	"CModel.h"
#include	"Shader.h"
#include	"CDirectxGraphics.h"
#include	"DX11Settransform.h"
#include	"ModelData.h"


bool CModel::Init(const char* filename, const char* vsfile, const char* psfile,std::string texfolder) {

	bool sts;

	std::string fname(filename);
	
	sts = m_assimpfile.Load(texfolder,fname);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}
	m_filetype = eASSIMPFILE;

	// ���_�f�[�^�̒�`�i�A�j���[�V�����Ή��j
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_SINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	//sts = CreateVertexShader(device,
	//	vsfile,
	//	"main",
	//	"vs_4_0",
	//	layout,
	//	numElements,
	//	&m_pVertexShader,
	//	&m_pVertexLayout);
	//if (!sts) {
	//	MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
	//	return false;
	//}

	// �s�N�Z���V�F�[�_�[�𐶐�
	//sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
	//	device,		// �f�o�C�X�I�u�W�F�N�g
	//	psfile,
	//	"main",
	//	"ps_4_0",
	//	&m_pPixelShader);
	//if (!sts) {
	//	MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
	//	return false;
	//}

	sts = g_shaderhashmap.SetPixelShader(psfile);
	if (!sts) {
		std::string f(psfile);
		std::string errormessage;
		errormessage = f + "SetPixelShader error";
		MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
		return false;
	}

	g_shaderhashmap.SetVertexShader(
		vsfile,			// ���_�V�F�[�_�[�t�@�C����
		layout,					// ���_���C�A�E�g
		numElements);			// �G�������g��
	if (!sts) {
		std::string f(vsfile);
		std::string errormessage;
		errormessage = f + " " + "SetVertexShader error";
		MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
		return false;
	}

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

bool CModel::InitToon(const char* filename, const char* vsfilename[], const char* psfilename[], std::string texfolder) {
	bool sts;

	std::string fname(filename);

	sts = m_assimpfile.Load(texfolder, fname);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}
	m_filetype = eASSIMPFILE;

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{

		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_SINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);


	// �s�N�Z���V�F�[�_�[����
	for (int i = 0; i < 2; i++) {
		bool sts = g_shaderhashmap.SetPixelShader(psfilename[i]);
		if (!sts) {
			std::string f(psfilename[i]);
			std::string errormessage;
			errormessage = f + "SetPixelShader error";
			MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
			return false;
		}
	}

	// ���_�V�F�[�_�[����
	for (int i = 0; i < 2; i++)
	{
		bool sts = g_shaderhashmap.SetVertexShader(
			vsfilename[i],			// ���_�V�F�[�_�[�t�@�C����
			layout,					// ���_���C�A�E�g
			numElements);			// �G�������g��
		if (!sts) {
			std::string f(vsfilename[i]);
			std::string errormessage;
			errormessage = f + " " + "SetVertexShader error";
			MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
			return false;
		}
	}

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	devcon = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �g�D�[���p�e�N�X�`��
	sts = CreatetSRVfromFile("assets/ToonPaint.png", dev, devcon, &g_toonsrv);
	//sts = CreatetSRVfromFile("assets/HAL/toon1.bmp", dev, devcon, &g_toonsrv);
	if (!sts) {
		MessageBox(nullptr, "load Toon texture load error", "error", MB_OK);
		return false;
	}

	// SRV���Z�b�g
	devcon->PSSetShaderResources(1, 1, &g_toonsrv);

	D3D11_RASTERIZER_DESC rasterDesc;

	// setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// �\�ʃJ�����O
	HRESULT hr = device->CreateRasterizerState(
		&rasterDesc,
		&g_rasterStateFrontCulling);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateRasterizerState error", "error", MB_OK);
		return false;
	}

	// ���ʃJ�����O
	rasterDesc.CullMode = D3D11_CULL_BACK;
	hr = device->CreateRasterizerState(
		&rasterDesc,
		&g_rasterStateBackCulling);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateRasterizerState error", "error", MB_OK);
		return false;
	}

	return true;

}




// �A�j���[�V�����X�V
void CModel::Update(
	unsigned int animno,
	const XMFLOAT4X4& mtxworld,
	const int INTERPOLATENUM){

	// �����X�L���f�[�^�X�V
	m_assimpfile.Update(animno, m_AnimFileIdx,m_animationcontainer, INTERPOLATENUM);

	// OBB���X�V
	m_assimpfile.UpdateOBB(mtxworld);
}

void CModel::setframe() {
	m_assimpfile.setframe();
}

void CModel::UpdateOBB(const XMFLOAT4X4& mtxworld) 
{
	// OBB���X�V
	m_assimpfile.UpdateOBB(mtxworld);
}

void CModel::Draw(XMFLOAT4X4& mtxworld, const char* vsfile, const char* psfile, bool LineOn) {

	ID3D11DeviceContext*	devcontext;			// �f�o�C�X�R���e�L�X�g
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	devcontext->RSSetState(g_rasterStateBackCulling);

	// i�Ԗڂ̃p�X
	ID3D11InputLayout* layout;
	layout = g_shaderhashmap.GetVertexLayout(vsfile);
	ID3D11VertexShader* vsh;
	vsh = g_shaderhashmap.GetVertexShader(vsfile);
	ID3D11PixelShader* psh;
	psh = g_shaderhashmap.GetPixelShader(psfile);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);
	// ���_�V�F�[�_�[���Z�b�g
	devcontext->VSSetShader(vsh, nullptr, 0);
	// �s�N�Z���V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(psh, nullptr, 0);
	// �`��
	m_assimpfile.Draw(devcontext,mtxworld, LineOn);
}

void CModel::DrawToon(XMFLOAT4X4& mtxworld, const char* vsfilename[], const char* psfilename[], bool LineOn)
{
	ID3D11DeviceContext*	devcontext;			// �f�o�C�X�R���e�L�X�g
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �}���`�p�X�����_�����O
	for (int i = 0; i < 2; i++) {

		if (i == 0)
		{
			devcontext->RSSetState(g_rasterStateBackCulling);
		}

		if (i == 1) {
			devcontext->RSSetState(g_rasterStateFrontCulling);
		}

		// i�Ԗڂ̃p�X
		ID3D11InputLayout* layout;
		layout = g_shaderhashmap.GetVertexLayout(vsfilename[i]);
		ID3D11VertexShader* vsh;
		vsh = g_shaderhashmap.GetVertexShader(vsfilename[i]);
		ID3D11PixelShader* psh;
		psh = g_shaderhashmap.GetPixelShader(psfilename[i]);

		// ���_�t�H�[�}�b�g���Z�b�g
		devcontext->IASetInputLayout(layout);
		// ���_�V�F�[�_�[���Z�b�g
		devcontext->VSSetShader(vsh, nullptr, 0);
		// �s�N�Z���V�F�[�_�[���Z�b�g
		devcontext->PSSetShader(psh, nullptr, 0);
		// �`��
		m_assimpfile.Draw(devcontext, mtxworld, LineOn);
	}

}

void CModel::Uninit(bool ani) {

	if (ani) {
		// �A�j���[�V�����f�[�^���
		for (auto sa : m_animationcontainer) {
			sa->Exit();
			delete sa;
		}
	}

	// ���_�V�F�[�_�[���
	if (m_pVertexShader) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	// �s�N�Z���V�F�[�_�[���
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	// ���_���C�A�E�g���
	if (m_pVertexLayout) {
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}
}