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

	// Z�o�b�t�@������
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

	if (scene == TITLE) 
	{		

		g_Title.Draw(
			190,				// ����w���W
			60,				// ����x���W
			700,				// ��
			400,				// ����	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

		CountTitle++;

		if (CountTitle < 50) 
		{
			g_click.Draw(
				390,				// ����w���W
				560,				// ����x���W
				250,				// ��
				100,				// ����	
				0.0f,0.0f,1.0f,1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[
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
			360,				// ����w���W
			150,				// ����x���W
			300,				// ��
			300,				// ����	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[
	
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
			285,				// ����w���W
			350,				// ����x���W
			450,				// ��
			250,				// ����	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[	
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
				190,				// ����w���W
				75,				// ����x���W
				700,				// ��
				400,				// ����	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

			if (Player::GetInstance().GetReStart())
			{
				CountTitle++;

				if (CountTitle < 50)
				{
					g_click.Draw(
						390,				// ����w���W
						560,				// ����x���W
						250,				// ��
						100,				// ����	
						0.0f, 0.0f, 1.0f, 1.0f,//UV
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[
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
				190,				// ����w���W
				75,				// ����x���W
				700,				// ��
				400,				// ����	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

			if (Player::GetInstance().GetReStart())
			{
				CountTitle++;

				if (CountTitle < 50)
				{
					g_click.Draw(
						390,				// ����w���W
						560,				// ����x���W
						250,				// ��
						100,				// ����	
						0.0f, 0.0f, 1.0f, 1.0f,//UV
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[
				}
				else if (CountTitle >= 100)
				{
					CountTitle = 0;
				}
			}
		}

		if (minimapbig) {
			MinimapFrame.Draw(
				510,				// ����w���W
				385,				// ����x���W
				516,				// ��
				386,				// ����	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

			//MiniMapRTT.DrawRTT(436, 405, 896,642, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));
			MiniMapRTT.DrawRTT(526, 404, 480, 350, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));
		}
		else {

			MinimapFrame.Draw(
				775,				// ����w���W
				575,				// ����x���W
				258,				// ��
				194,				// ����	
				0.0f, 0.0f, 1.0f, 1.0f,//UV
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

			//MiniMapRTT.DrawRTT(784, 587, 342, 284, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));
			MiniMapRTT.DrawRTT(787, 589, 230, 166, 0, 0, 1, 1, XMFLOAT4(1, 1, 1, 1));

		}
	
	}

	if (scene == TITLE) 
	{	
		g_mouse.Draw(
			mouse_x,            // ����w���W
			mouse_y,            // ����x���W
			30,				// ��
			30,				// ����	
			0.0f,0.0f,1.0f,1.0f,//UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[
	}

	// Z�o�b�t�@�L����
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
		390,				// ����w���W
		0,				// ����x���W
		80,				// ��
		80,				// ����	
		0.0f,0.0f,1.0f,1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

	
	for (int i = 0; i < e_HP; i++)
	{
		g_eHP[i].Draw(
			480 + i * 1,	// ����w���W
			30,				// ����x���W 45
			1,				// ��
			30,				// ����	
			0.0f, 0.0f, 1.0f, 1.0f,                    //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 		
	}
	

	g_Frame[1].Draw(
		480,				// ����w���W
		30,				// ����x���W
		100,				// ��
		30,				// ����	
		0.0f,0.0f,1.0f,1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 	
}


void UI::DrawBossUI()
{
	g_WolfFace.Draw(
		390,				// ����w���W
		0,				// ����x���W
		80,				// ��
		80,				// ����	
		0.0f, 0.0f, 1.0f, 1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

	for (int i = 0; i < BossHP / 500 * 100; i++)
	{
		g_eHP[i].Draw(
			480 + i *4 ,	// ����w���W
			30,				// ����x���W 45
			4,				// ��
			30,				// ����	
			0.0f, 0.0f, 1.0f, 1.0f,                    //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 		
	}

	g_Frame[1].Draw(
		480,				// ����w���W
		30,				// ����x���W
		400,				// ��
		30,				// ����	
		0.0f, 0.0f, 1.0f, 1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 	
}
void UI::DrawPlayerUI()
{
	g_pface.Draw(
		0,				// ����w���W
		0,				// ����x���W
		80,				// ��
		80,				// ����	
		0.0f,0.0f,1.0f,1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 

	g_pLevel.Draw(
		80,				// ����w���W
		8,				// ����x���W
		30,				// ��
		30,				// ����	
		0.0f,0.0f,1.0f,1.0f, //UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

	if (p_level <= 9)
	{
		g_pLevelNum[0].Draw(
			110,				// ����w���W
			7,				// ����x���W
			30,				// ��
			30,				// ����	
			tu,tv,0.2f,0.5f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 
	}
	else if (p_level >= 10)
	{
		g_pLevelNum[1].Draw(
			110,			   // ����w���W
			7,				   // ����x���W
			30,				   // ��
			30,				   // ����	
			tu2,tv2,0.2f,0.5f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 

		g_pLevelNum[0].Draw(
			130,				// ����w���W
			7,				// ����x���W
			30,				// ��
			30,				// ����	
			tu,tv,0.2f,0.5f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 
	}

	for (int i = 0; i < p_HP / p_MAXHP * 100; i++)
	{
		g_pHP[i].Draw(
			86 + i * 1,				// ����w���W
			35,				// ����x���W
			1,				// ��
			30,				// ����	
			0.0f,0.0f,1.0f,1.0f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 		
	}

	g_Frame[0].Draw(
		86,				// ����w���W
		35,				// ����x���W
		100,				// ��
		30,				// ����	
		0.0f,0.0f,1.0f,1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 

	for (int i = 0; i < p_EXP / p_MAXEXP * 100; i++)
	{
		g_pEXP[i].Draw(
			86 + i * 1,				// ����w���W
			67,				// ����x���W
			1,				// ��
			15,				// ����	
			0.0f, 0.0f, 1.0f, 1.0f, //UV
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[ 		
	}

	g_Frame[2].Draw(
		86,				// ����w���W
		67,				// ����x���W
		100,				// ��
		15,				// ����	
		0.0f, 0.0f, 1.0f, 1.0f,//UV
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));		// ���_�J���[

}

void Billboarinit(CBillBoard& name, float sizeX, float sizeY, float u1, float u2, float v1, float v2, const char* filename) 
{
	name.Init(0, 0, 0, sizeX, sizeY, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	float u[4] = { u1, u1, u2, u2 };
	float v[4] = { v2, v1, v2, v1 };

	name.SetUV(u, v);

	//�e�N�X�`���ǂݍ���
	name.LoadTexTure(filename);

}




