//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <initguid.h>
#include <stdio.h>
#include "game.h"
#include "CDirectxGraphics.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "CLight.h"
#include "imguiutil.h"
#include "object.h"
#include "player.h"
#include"enemy.h"
#include "UI.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------

CLight			g_directionallight;	// 平行光源

extern Enemy g_enemy[ENEMYMAX];
extern XMFLOAT3 g_enemypos[ENEMYMAX];
extern Enemy Boss;
extern XMFLOAT3 BossPos;
bool mouseOnoff = true;
SCENE scene;
float CHight;
bool MapBiger = false;
//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	WriteOnConsole("初期化開始\n");

	// DirectX11 グラフィックス初期化
	bool sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if(!sts){
		MessageBox(nullptr, "CDirectXGraphics Initエラー", "error!!", MB_OK);
		return false;
	}

	// アルファブレンディングオン
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();

	
	// カメラ
	XMFLOAT3 eye = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 lookat = { 0.0f,0.0f,0.0f };
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	// カメラ初期化
	CCamera::GetInstance()->Init(1.0f, 1000.0f, XM_PI / 6.0f, SCREEN_X, SCREEN_Y, eye, lookat, up);
	WriteOnConsole("カメラ初期化した\n");

	// 定数バッファ初期化
	DX11SetTransform::GetInstance()->Init();

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst,hwnd,width,height);

	
	// ライト初期化
	XMFLOAT4 lightpos(0, 3, -5, 0);
	g_directionallight.Init(eye, lightpos);
	WriteOnConsole("ライト初期化した\n");
	Object::GetInstance()->Init(sts);

	
	RenderTexture::GetInstance()->InitRTT(SCREEN_X / 4, SCREEN_Y / 4);
	WriteOnConsole("RTTを初期化した\n");
	
	UI::Getintance()->InitUI();
	WriteOnConsole("UIを初期化した\n");

	// imgui初期化
	imguiinit(hwnd);
	WriteOnConsole("初期化完成\n");

	WriteOnConsole("起動する\n");

	return	true;
}


//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameInput()
{
	// 入力を受け付ける
	CDirectInput::GetInstance().GetKeyBuffer();
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameUpdate()
{	
	UI::Getintance()->UpdataUI();

	if (scene == TITLE)
	{
		CCamera::GetInstance()->TitleCamera();

		if (CDirectInput::GetInstance().GetMouseLButtonCheck())
		{
			scene = LOAD;
			
		}

	

		Player::GetInstance().TitleUpdata();
	}

	if (scene == LOAD)
	{
		static int count = 0;
		count++;
		if (count > 100)
		{
			if (!Player::GetInstance().GetReStart()) 
			{
				Player::GetInstance().InitPlayerStatus();
				scene = GAME;
				count = 0;
				MapBiger = false;
				XMFLOAT4 lightpos(-17, 5, -40, 0);
				g_directionallight.Init(XMFLOAT3(0.0f, 0.0f, -10.0f), lightpos);
			}
			else 
			{
				scene = TITLE;
				XMFLOAT4 lightpos(0, 3, -5, 0);
				g_directionallight.Init(XMFLOAT3(0.0f, 0.0f, -10.0f), lightpos);
				
				Player::GetInstance().SetReStart();
				for (int i = 0; i < ENEMYMAX; i++)
				{
					g_enemy[i].InitEnemyStatus(g_enemypos[i], 100, 20, 50/*,M_wolf*/);
					
				}
				Boss.InitBossStatus(BossPos, 500, 30, 300);

				count = 0;
			}
		}	
	}	

	if (scene == GAME)
	{

		CCamera::GetInstance()->Lookthat(Player::GetInstance().GetPlayerMat(), 12.0, CHight);

		Object::GetInstance()->Updata();

		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_P))
		{
			scene = TITLE;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_M))
		{
			MapBiger = !MapBiger;
			Player::GetInstance().SetMiniMapPlayerSize(MapBiger);
			for (int i = 0; i < ENEMYMAX; i++) {
				g_enemy[i].SetMiniMapEnemySize(MapBiger);
			}
			Boss.SetMiniMapBossSize(MapBiger);
		}
	}

	CDirectInput::GetInstance().GetMouseState(mouseOnoff);

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_TAB))
	{
		mouseOnoff = !mouseOnoff;
	}

}

//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameRender()
{

	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* rtv;
	rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();


	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(
		rtv,			// レンダーターゲットビュー
		ClearColor);	// クリアするカラー値

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();


	// Zバッファ、ステンシルバッファクリア
	devcontext->ClearDepthStencilView(
		dstv,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする

	
	XMFLOAT4X4 mtx;		// 行列

	// ビュー変換行列セット
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// プロジェクション変換行列セット
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	Object::GetInstance()->Render();

	//ui
	UI::Getintance()->DrawUI(MapBiger);

	if (scene == GAME)
	{
		//GetMiniMapRTT
		RenderTexture::GetInstance()->RenderMiniMap(devcontext, SCREEN_X, SCREEN_Y, MapBiger);
	}
	
	

	// 描画後処理
	IDXGISwapChain* swapchain;
	swapchain = CDirectXGraphics::GetInstance()->GetSwapChain();
	swapchain->Present(
		0,		// フレームの表示を垂直方向の空白と同期させる方法を指定する整数。
		0);		// スワップチェーン表示オプションを含む整数値。 
				// これらのオプションはDXGI_PRESENT定数で定義されています
}

void WriteOnConsole(const char* word)
{
	HANDLE                     hStdOutput;
	DWORD                      dwWriteByte;
	TCHAR                      szBuf[256];
	CONSOLE_SCREEN_BUFFER_INFO screenBuffer;

	hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOutput, &screenBuffer);

	lstrcpy(szBuf, TEXT(word));
	WriteConsole(hStdOutput, szBuf, lstrlen(szBuf), &dwWriteByte, NULL);

}

//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameMain()
{
	GameInput();					// 入力
	GameUpdate();					// 更新
	GameRender();					// 描画
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameExit()
{
	// モデル終了処理
	//g_model.Uninit();
	Object::GetInstance()->ObjectExit();

	RenderTexture::GetInstance()->UnInit();

	// 光源終了処理
	g_directionallight.Uninit();

	// DirectX11 グラフィックス終了処理
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
