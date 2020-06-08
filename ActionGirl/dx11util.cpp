#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"Shader.h"
#include	"CCamera.h"
#include	"memory.h"
#include	"CLight.h"
#include	"DX11Settransform.h"

// �萔�o�b�t�@��`
ID3D11Buffer*       g_pConstantBuffer1 = nullptr;	// �R���X�^���g�o�b�t�@1

// ���s������`
//CLight				g_directionallight;
//
//void DX11LightInit(DirectX::XMFLOAT4 lightpos) {
//	g_directionallight.Init(CCamera::GetInstance()->GetEye(), lightpos);
//}
//
//void DX11LightUninit() {
//	g_directionallight.Uninit();
//}
//
//void DX11LightUpdate(DirectX::XMFLOAT4 lpos) {
//	g_directionallight.SetEyePos(CCamera::GetInstance()->GetEye());
//	g_directionallight.SetLightPos(lpos);
//	g_directionallight.Update();
//}

CDirectXGraphics* GetDX11Obj() {
	CDirectXGraphics* pins=nullptr;
	pins = CDirectXGraphics::GetInstance();
	return pins;
}

ID3D11Device* GetDX11Device() {
	CDirectXGraphics* pins=nullptr;
	pins = CDirectXGraphics::GetInstance();
	return pins->GetDXDevice();
}

ID3D11DeviceContext*	GetDX11DeviceContext() {
	CDirectXGraphics* pins=nullptr;
	pins = CDirectXGraphics::GetInstance();
	return pins->GetImmediateContext();
}

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen) {
	bool sts;
	CDirectXGraphics* pins=nullptr;
	pins = CDirectXGraphics::GetInstance();

	sts = pins->Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	// �A���t�@�u�����h�L��
	pins->TurnOnAlphaBlending();

	// �r�����s����������������������
	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts) {
		MessageBox(NULL, "SetTransform error", "Error", MB_OK);
		return false;
	}

	return true;
}

void DX11Uninit() {

	DX11SetTransform::GetInstance()->Uninit();

	// �������
	CDirectXGraphics::GetInstance()->Exit();
}

void DX11BeforeRender(float ClearColor[]) {
	// �^�[�Q�b�g�o�b�t�@�N���A
	GetDX11DeviceContext()->ClearRenderTargetView(CDirectXGraphics::GetInstance()->GetRenderTargetView(), ClearColor);
	// Z�o�b�t�@�N���A
	GetDX11DeviceContext()->ClearDepthStencilView(CDirectXGraphics::GetInstance()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11AfterRender() {
	CDirectXGraphics* pins;
	pins = CDirectXGraphics::GetInstance();

	// ��������
	pins->GetSwapChain()->Present(0, 0);
}

ID3D11RenderTargetView* DX11GetRenderTagetView() {
	return CDirectXGraphics::GetInstance()->GetRenderTargetView();
}

// Z�o�b�t�@������
void TurnOffZbuffer() {
	ID3D11RenderTargetView*  rtvtable[1];

	rtvtable[0] = DX11GetRenderTagetView();

	GetDX11DeviceContext()->OMSetRenderTargets(
		1,										// �^�[�Q�b�g
		rtvtable,								// �r���[�e�[�u��
		nullptr									// �[�x�o�b�t�@�Ȃ�
	);

}

// Z�o�b�t�@�L����
void TurnOnZBuffer() {
	ID3D11RenderTargetView*  rtvtable[1];

	rtvtable[0] = DX11GetRenderTagetView();

	CDirectXGraphics::GetInstance()->GetImmediateContext()->OMSetRenderTargets(
		1,										// �^�[�Q�b�g
		rtvtable,								// �r���[�e�[�u��
		CDirectXGraphics::GetInstance()->GetDepthStencilView()			// �[�x�o�b�t�@�Ȃ�
	);
}