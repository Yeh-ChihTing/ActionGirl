#include "CDirectInput.h"
#include "imguiutil.h"
#include "game.h"
#include "CDirectxGraphics.h"
#include "player.h"
#include "enemy.h"
#include "UI.h"
#include "CCamera.h"

extern  SCENE scene;
extern Enemy g_enemy[ENEMYMAX];
extern Enemy Boss;

bool UI::InitUI()
{
	g_Title.Init("assets/pic/Title.png");

	g_mouse.Init("assets/pic/mouse.png");

	g_click.Init("assets/pic/click.png");

	for (int i = 0; i < 31; i++) 
	{
		g_Load[i].Init(LOADINGPIC[i]);
	}

	g_Loading.Init("assets/pic/loading.png");

	g_GameOver.Init("assets/pic/gameover.png");

	g_GameClear.Init("assets/pic/GameClear.png");

	g_pface.Init("assets/pic/PlayerFace.png");
	g_pLevel.Init("assets/pic/LV.png");



	for (int i = 0; i < 2; i++) 
	{
		g_pLevelNum[i].Init("assets/pic/numbertexture.png");
	}

	HPTex.Init("assets/pic/blood.png");
	EXETex.Init("assets/pic/EXP.png");

	for (int i = 0; i < 100; i++) 
	{
		g_pHP[i]= HPTex;
		g_eHP[i]= HPTex;
		g_pEXP[i]= EXETex;
	}

	FrameTex.Init("assets/pic/bloodframe.png");

	for (int i = 0; i < 3; i++)
	{
		g_Frame[i]= FrameTex;
	}

	g_WolfFace.Init("assets/pic/wolfface.png");

	Billboarinit(g_eDamegePic, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f, 1.0f, "assets/pic/kiri.png");

	MiniMapRTT.Init("assets/pic/MIniMapFrame.png");
	MinimapFrame.Init("assets/pic/MIniMapFrame.png");

	return true;
}

void  UI::UpdataUI()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP))
	{		
		py--;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN))
	{
		py++;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT))
	{
		px--;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT))
	{
		px++;
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_Z))
	{
		p_level++;
	}
		
	mouse_x = CDirectInput::GetInstance().GetMousePosX();
	mouse_y = CDirectInput::GetInstance().GetMousePosY();
	
	p_level = Player::GetInstance().GetPlayerLevel();

	p_MAXHP = Player::GetInstance().GetPlayerMAXHP();

	p_HP = Player::GetInstance().GetPlayerHP();

	p_EXP = Player::GetInstance().GetPlayerEXP();

	p_MAXEXP = Player::GetInstance().GetPlayerMAXEXP();

	tu = p_level % 10  % 5 * 0.2;
	tv = p_level % 10  / 5 * 0.5;

	tu2 = p_level % 100 / 10 % 5 * 0.2;
	tv2 = p_level % 100 / 10 / 5 * 0.5;

	attacking = Player::GetInstance().GetAttackFlag();
	targetNum = Player::GetInstance().GetTargetNum();

	e_HP = g_enemy[targetNum].GetEnemyHP();
	BossHP = Boss.GetEnemyHP();

	if (Player::GetInstance().GetHItEnemy()) 
	{
		if (attacking)
		{
			E_Mat = g_enemy[targetNum].GetEnemyMat();
			g_eDamegePic.SetPosition(E_Mat._41, E_Mat._42 + 0.4f, E_Mat._43);
		}
	}

	if (Player::GetInstance().GetHitBoss())
	{
		if (attacking)
		{
			E_Mat = Boss.GetEnemyMat();
			g_eDamegePic.SetPosition(E_Mat._41, E_Mat._42 + 0.4f, E_Mat._43);
		}
	}


}

void UI::DrawUI(bool minimapbig)
{

	// Zバッファ無効化
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

	if (scene == TITLE) 
	{		

		g_Title.Draw(
			190,				// 左上Ｘ座標
			60,				// 左上Ｙ座標
			700,				// 幅
			400,				// 高さ	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

		CountTitle++;

		if (CountTitle < 50) 
		{
			g_click.Draw(
				390,				// 左上Ｘ座標
				560,				// 左上Ｙ座標
				250,				// 幅
				100,				// 高さ	
				0.0f,0.0f,1.0f,1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー
		}
		else if (CountTitle >= 100) 
		{
			CountTitle = 0;
		}		
	}

	if (scene == LOAD) 
	{   
		static int i;

		g_Load[i].Draw(
			360,				// 左上Ｘ座標
			150,				// 左上Ｙ座標
			300,				// 幅
			300,				// 高さ	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー
	
		CountLoading++;

		if (CountLoading >= 2)
		{			
			CountLoading = 0;
			if (i >= 30) {
				i = 0;
			}
			else {
				i++;
			}
		}

		g_Loading.Draw(
			285,				// 左上Ｘ座標
			350,				// 左上Ｙ座標
			450,				// 幅
			250,				// 高さ	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー	
	}

	if (scene == GAME) 
	{				
		DrawPlayerUI();
	
		if (Player::GetInstance().GetHItEnemy()) 
		{
				DrawEnemyUI();
		}

		if (Player::GetInstance().GetHitBoss()) 
		{
			DrawBossUI();
		}
	

		if (Player::GetInstance().GetPlayerHP() <= 0)
		{
			g_GameOver.Draw(
				190,				// 左上Ｘ座標
				75,				// 左上Ｙ座標
				700,				// 幅
				400,				// 高さ	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

			if (Player::GetInstance().GetReStart())
			{
				CountTitle++;

				if (CountTitle < 50)
				{
					g_click.Draw(
						390,				// 左上Ｘ座標
						560,				// 左上Ｙ座標
						250,				// 幅
						100,				// 高さ	
						0.0f, 0.0f, 1.0f, 1.0f,//UV
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー
				}
				else if (CountTitle >= 100)
				{
					CountTitle = 0;
				}
			}
		}

		if (Boss.GetEnemyHP() <= 0) 
		{
			g_GameClear.Draw(
				190,				// 左上Ｘ座標
				75,				// 左上Ｙ座標
				700,				// 幅
				400,				// 高さ	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

			if (Player::GetInstance().GetReStart())
			{
				CountTitle++;

				if (CountTitle < 50)
				{
					g_click.Draw(
						390,				// 左上Ｘ座標
						560,				// 左上Ｙ座標
						250,				// 幅
						100,				// 高さ	
						0.0f, 0.0f, 1.0f, 1.0f,//UV
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー
				}
				else if (CountTitle >= 100)
				{
					CountTitle = 0;
				}
			}
		}

		if (minimapbig) {
			MinimapFrame.Draw(
				510,				// 左上Ｘ座標
				385,				// 左上Ｙ座標
				516,				// 幅
				386,				// 高さ	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

			//MiniMapRTT.DrawRTT(436, 405, 896,642, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));
			MiniMapRTT.DrawRTT(526, 404, 480, 350, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));
		}
		else {

			MinimapFrame.Draw(
				775,				// 左上Ｘ座標
				575,				// 左上Ｙ座標
				258,				// 幅
				194,				// 高さ	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

			//MiniMapRTT.DrawRTT(784, 587, 342, 284, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));
			MiniMapRTT.DrawRTT(787, 589, 230, 166, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));

		}
	
	}

	if (scene == TITLE) 
	{	
		g_mouse.Draw(
			mouse_x,            // 左上Ｘ座標
			mouse_y,            // 左上Ｙ座標
			30,				// 幅
			30,				// 高さ	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー
	}

	// Zバッファ有効化
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();

	if (scene == GAME) 
	{
		
		if (attacking)
		{
			g_eDamegePic.DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
		}		

	}

}

void UI::DrawEnemyUI()
{
	g_WolfFace.Draw(
		390,				// 左上Ｘ座標
		0,				// 左上Ｙ座標
		80,				// 幅
		80,				// 高さ	
		0.0f,0.0f,1.0f,1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

	
	for (int i = 0; i < e_HP; i++)
	{
		g_eHP[i].Draw(
			480 + i * 1,	// 左上Ｘ座標
			30,				// 左上Ｙ座標 45
			1,				// 幅
			30,				// 高さ	
			0.0f, 0.0f, 1.0f, 1.0f,                    //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 		
	}
	

	g_Frame[1].Draw(
		480,				// 左上Ｘ座標
		30,				// 左上Ｙ座標
		100,				// 幅
		30,				// 高さ	
		0.0f,0.0f,1.0f,1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 	
}


void UI::DrawBossUI()
{
	g_WolfFace.Draw(
		390,				// 左上Ｘ座標
		0,				// 左上Ｙ座標
		80,				// 幅
		80,				// 高さ	
		0.0f, 0.0f, 1.0f, 1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

	for (int i = 0; i < BossHP / 500 * 100; i++)
	{
		g_eHP[i].Draw(
			480 + i *4 ,	// 左上Ｘ座標
			30,				// 左上Ｙ座標 45
			4,				// 幅
			30,				// 高さ	
			0.0f, 0.0f, 1.0f, 1.0f,                    //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 		
	}

	g_Frame[1].Draw(
		480,				// 左上Ｘ座標
		30,				// 左上Ｙ座標
		400,				// 幅
		30,				// 高さ	
		0.0f, 0.0f, 1.0f, 1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 	
}
void UI::DrawPlayerUI()
{
	g_pface.Draw(
		0,				// 左上Ｘ座標
		0,				// 左上Ｙ座標
		80,				// 幅
		80,				// 高さ	
		0.0f,0.0f,1.0f,1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 

	g_pLevel.Draw(
		80,				// 左上Ｘ座標
		8,				// 左上Ｙ座標
		30,				// 幅
		30,				// 高さ	
		0.0f,0.0f,1.0f,1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

	if (p_level <= 9)
	{
		g_pLevelNum[0].Draw(
			110,				// 左上Ｘ座標
			7,				// 左上Ｙ座標
			30,				// 幅
			30,				// 高さ	
			tu,tv,0.2f,0.5f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 
	}
	else if (p_level >= 10)
	{
		g_pLevelNum[1].Draw(
			110,			   // 左上Ｘ座標
			7,				   // 左上Ｙ座標
			30,				   // 幅
			30,				   // 高さ	
			tu2,tv2,0.2f,0.5f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 

		g_pLevelNum[0].Draw(
			130,				// 左上Ｘ座標
			7,				// 左上Ｙ座標
			30,				// 幅
			30,				// 高さ	
			tu,tv,0.2f,0.5f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 
	}

	for (int i = 0; i < p_HP / p_MAXHP * 100; i++)
	{
		g_pHP[i].Draw(
			86 + i * 1,				// 左上Ｘ座標
			35,				// 左上Ｙ座標
			1,				// 幅
			30,				// 高さ	
			0.0f,0.0f,1.0f,1.0f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 		
	}

	g_Frame[0].Draw(
		86,				// 左上Ｘ座標
		35,				// 左上Ｙ座標
		100,				// 幅
		30,				// 高さ	
		0.0f,0.0f,1.0f,1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 

	for (int i = 0; i < p_EXP / p_MAXEXP * 100; i++)
	{
		g_pEXP[i].Draw(
			86 + i * 1,				// 左上Ｘ座標
			67,				// 左上Ｙ座標
			1,				// 幅
			15,				// 高さ	
			0.0f, 0.0f, 1.0f, 1.0f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー 		
	}

	g_Frame[2].Draw(
		86,				// 左上Ｘ座標
		67,				// 左上Ｙ座標
		100,				// 幅
		15,				// 高さ	
		0.0f, 0.0f, 1.0f, 1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// 頂点カラー

}

void Billboarinit(CBillBoard& name, float sizeX, float sizeY, float u1, float u2, float v1, float v2, const char* filename) 
{
	name.Init(0, 0, 0, sizeX, sizeY, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	float u[4] = { u1, u1, u2, u2 };
	float v[4] = { v2, v1, v2, v1 };

	name.SetUV(u, v);

	//テクスチャ読み込み
	name.LoadTexTure(filename);

}




