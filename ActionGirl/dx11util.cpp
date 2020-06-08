#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"Shader.h"
#include	"CCamera.h"
#include	"memory.h"
#include	"CLight.h"
#include	"DX11Settransform.h"

// 定数バッファ定義
ID3D11Buffer*       g_pConstantBuffer1 = nullptr;	// コンスタントバッファ1

// 平行光源定義
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

	// アルファブレンド有効
	pins->TurnOnAlphaBlending();

	// ＳｅｔＴｒａｎｓｆｏｒｍ初期化
	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts) {
		MessageBox(NULL, "SetTransform error", "Error", MB_OK);
		return false;
	}

	return true;
}

void DX11Uninit() {

	DX11SetTransform::GetInstance()->Uninit();

	// 解放処理
	CDirectXGraphics::GetInstance()->Exit();
}

void DX11BeforeRender(float ClearColor[]) {
	// ターゲットバッファクリア
	GetDX11DeviceContext()->ClearRenderTargetView(CDirectXGraphics::GetInstance()->GetRenderTargetView(), ClearColor);
	// Zバッファクリア
	GetDX11DeviceContext()->ClearDepthStencilView(CDirectXGraphics::GetInstance()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11AfterRender() {
	CDirectXGraphics* pins;
	pins = CDirectXGraphics::GetInstance();

	// ｓｗａｐ
	pins->GetSwapChain()->Present(0, 0);
}

ID3D11RenderTargetView* DX11GetRenderTagetView() {
	return CDirectXGraphics::GetInstance()->GetRenderTargetView();
}

// Zバッファ無効化
void TurnOffZbuffer() {
	ID3D11RenderTargetView*  rtvtable[1];

	rtvtable[0] = DX11GetRenderTagetView();

	GetDX11DeviceContext()->OMSetRenderTargets(
		1,										// ターゲット
		rtvtable,								// ビューテーブル
		nullptr									// 深度バッファなし
	);

}

// Zバッファ有効化
void TurnOnZBuffer() {
	ID3D11RenderTargetView*  rtvtable[1];

	rtvtable[0] = DX11GetRenderTagetView();

	CDirectXGraphics::GetInstance()->GetImmediateContext()->OMSetRenderTargets(
		1,										// ターゲット
		rtvtable,								// ビューテーブル
		CDirectXGraphics::GetInstance()->GetDepthStencilView()			// 深度バッファなし
	);
}