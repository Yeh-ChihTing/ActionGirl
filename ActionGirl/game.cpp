//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
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
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------

CLight			g_directionallight;	// ���s����

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
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	WriteOnConsole("�������J�n\n");

	// DirectX11 �O���t�B�b�N�X������
	bool sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if(!sts){
		MessageBox(nullptr, "CDirectXGraphics Init�G���[", "error!!", MB_OK);
		return false;
	}

	// �A���t�@�u�����f�B���O�I��
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();

	
	// �J����
	XMFLOAT3 eye = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 lookat = { 0.0f,0.0f,0.0f };
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	// �J����������
	CCamera::GetInstance()->Init(1.0f, 1000.0f, XM_PI / 6.0f, SCREEN_X, SCREEN_Y, eye, lookat, up);
	WriteOnConsole("�J��������������\n");

	// �萔�o�b�t�@������
	DX11SetTransform::GetInstance()->Init();

	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(hinst,hwnd,width,height);

	
	// ���C�g������
	XMFLOAT4 lightpos(0, 3, -5, 0);
	g_directionallight.Init(eye, lightpos);
	WriteOnConsole("���C�g����������\n");
	Object::GetInstance()->Init(sts);

	
	RenderTexture::GetInstance()->InitRTT(SCREEN_X / 4, SCREEN_Y / 4);
	WriteOnConsole("RTT������������\n");
	
	UI::Getintance()->InitUI();
	WriteOnConsole("UI������������\n");

	// imgui������
	imguiinit(hwnd);
	WriteOnConsole("����������\n");

	WriteOnConsole("�N������\n");

	return	true;
}


//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput()
{
	// ���͂��󂯕t����
	CDirectInput::GetInstance().GetKeyBuffer();
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
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
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender()
{

	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* rtv;
	rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();


	// �^�[�Q�b�g�o�b�t�@�N���A
	devcontext->ClearRenderTargetView(
		rtv,			// �����_�[�^�[�Q�b�g�r���[
		ClearColor);	// �N���A����J���[�l

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();


	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	devcontext->ClearDepthStencilView(
		dstv,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����

	
	XMFLOAT4X4 mtx;		// �s��

	// �r���[�ϊ��s��Z�b�g
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// �v���W�F�N�V�����ϊ��s��Z�b�g
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
	
	

	// �`��㏈��
	IDXGISwapChain* swapchain;
	swapchain = CDirectXGraphics::GetInstance()->GetSwapChain();
	swapchain->Present(
		0,		// �t���[���̕\���𐂒������̋󔒂Ɠ�����������@���w�肷�鐮���B
		0);		// �X���b�v�`�F�[���\���I�v�V�������܂ސ����l�B 
				// �����̃I�v�V������DXGI_PRESENT�萔�Œ�`����Ă��܂�
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
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameMain()
{
	GameInput();					// ����
	GameUpdate();					// �X�V
	GameRender();					// �`��
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameExit()
{
	// ���f���I������
	//g_model.Uninit();
	Object::GetInstance()->ObjectExit();

	RenderTexture::GetInstance()->UnInit();

	// �����I������
	g_directionallight.Uninit();

	// DirectX11 �O���t�B�b�N�X�I������
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
