#pragma once
#include "C2DQuadtex.h"
#include "CBillBoard.h"


class UI
{
private:
	
	C2DQuadTex      MiniMapRTT;
	C2DQuadTex      MinimapFrame;

	C2DQuadTex		g_Title;
	C2DQuadTex		g_mouse;
	C2DQuadTex		g_click;
	C2DQuadTex		g_Load[31];
	C2DQuadTex		g_Loading;
	C2DQuadTex		g_GameOver;
	C2DQuadTex		g_GameClear;
	
	C2DQuadTex		g_pface;
	C2DQuadTex		g_pLevel;
	C2DQuadTex		g_pLevelNum[2];
	C2DQuadTex		g_pHP[100];
	C2DQuadTex		g_pEXP[100];
	
	C2DQuadTex		g_Frame[3];
	
	
	C2DQuadTex		g_WolfFace;
	C2DQuadTex		g_eHP[100];

	C2DQuadTex      HPTex;
	C2DQuadTex      FrameTex;
	

	CBillBoard      g_eDamegePic;
	bool attacking = false;
	int targetNum;

	float tx, ty;
	float px, py;
	int CountTitle = 0;
	int CountLoading = 0;
	//int i = 0;
	
	int mouse_x, mouse_y;
	float tu;
	float tv;
	float tu2;
	float tv2;
	int p_level;
	float p_HP;
	float p_MAXHP;
	float p_EXP;
	float p_MAXEXP;
	
	float e_HP;
	float BossHP;
	XMFLOAT4X4 E_Mat;
	
	std::string LOADINGPIC[31]
	{
		"assets/pic/loading/frame-0.png",
		"assets/pic/loading/frame-1.png",
		"assets/pic/loading/frame-2.png",
		"assets/pic/loading/frame-3.png",
		"assets/pic/loading/frame-4.png",
		"assets/pic/loading/frame-5.png",
		"assets/pic/loading/frame-6.png",
		"assets/pic/loading/frame-7.png",
		"assets/pic/loading/frame-8.png",
		"assets/pic/loading/frame-9.png",
		"assets/pic/loading/frame-10.png",
		"assets/pic/loading/frame-11.png",
		"assets/pic/loading/frame-12.png",
		"assets/pic/loading/frame-13.png",
		"assets/pic/loading/frame-14.png",
		"assets/pic/loading/frame-15.png",
		"assets/pic/loading/frame-16.png",
		"assets/pic/loading/frame-17.png",
		"assets/pic/loading/frame-18.png",
		"assets/pic/loading/frame-19.png",
		"assets/pic/loading/frame-20.png",
		"assets/pic/loading/frame-21.png",
		"assets/pic/loading/frame-22.png",
		"assets/pic/loading/frame-23.png",
		"assets/pic/loading/frame-24.png",
		"assets/pic/loading/frame-25.png",
		"assets/pic/loading/frame-26.png",
		"assets/pic/loading/frame-27.png",
		"assets/pic/loading/frame-28.png",
		"assets/pic/loading/frame-29.png",
		"assets/pic/loading/frame-30.png",
	};

public:

	bool InitUI();
	void UpdataUI();
	void DrawUI(bool minimapbig);
	void DrawPlayerUI();
	void DrawEnemyUI();
	void DrawBossUI();
	static UI* Getintance() {
		static UI instance;
		return &instance;
	}
	
};

void Billboarinit(CBillBoard& name, float sizeX, float sizeY, float u1, float u2, float v1, float v2, const char* filename);