//*****************************************************************************
//!	@file	game.h
//!	@brief	�Q�[������
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

#define		SCREEN_X		1024
#define		SCREEN_Y		768
#define		FULLSCREEN      0

enum SCENE {
	TITLE,
	LOAD,
	GAME,
	GAMEOVER,
	GAMEClEAR,
};

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
void WriteOnConsole(const char* word);
void GameMain();
void GameExit();
void GameInput();
void GameUpdate();
void GameRender();

//******************************************************************************
//	End of file.
//******************************************************************************
