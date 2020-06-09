#include <windows.h>
#include <stdio.h>
#include "game.h"
#include "object.h"
#include "CModel.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "CLight.h"
#include "imguiutil.h"
#include "player.h"
#include "enemy.h"
#include"RenderToTexture.h"
#include"UI.h"


EnemyLoad EnemyModelLoad;
Enemy g_enemy[ENEMYMAX];
Enemy Boss;
extern SCENE scene;
XMFLOAT3 g_enemypos[ENEMYMAX]
{
	/*{800.0f, 0.f, 310.0f},
	{640.0f, 0.0f,300.0f},*/
	{-5.8f,0.0f,-1.5f},
    {0.0f,0.0f,0.0f},
    {9.0f,0.0f,-24.0f}
};

const char* vs[] = {
	"shader/vs.fx",
	"shader/vsoutline.fx",

};

// ピクセルシェーダー
const char* ps[] = {
	"shader/pstoon.fx",
	"shader/psoutline.fx",
};

XMFLOAT3 BossPos{ 6.0f,0.0f,35.0f };

bool Object::Init(bool sts)
{
	

	sts = g_Skydome.Init("assets/skydome/Skydome151003by.x", "shader/vs.fx", "shader/psskydome.fx",
		"assets/skydome/");
	//if (!sts) {
	//	MessageBox(NULL, "load Skydome model error", "error", MB_OK);
	//	return false;
	//}

	WriteOnConsole("スカイドームを初期化した\n");



	/*sts = g_ground.Init("assets/shima/shima_001.obj", "shader/vs.fx", "shader/ps.hlsl",
		"assets/shima/");*/

	sts = g_ground.InitToon("assets/shima/shima_001.obj", vs, ps,
		"assets/shima/");
	//if (!sts) {
	//	MessageBox(NULL, "load ground model error", "error", MB_OK);
	//	return false;
	//}
	WriteOnConsole("地形を初期化した\n");

	StageHitInit(&g_ground);
	WriteOnConsole("地形判定を初期化した\n");

	Player::GetInstance().Init();

	WriteOnConsole("プレイヤーを初期化した\n");


	EnemyModelLoad.EnemyModelLoad();

	for (int i = 0; i < ENEMYMAX; i++)
	{
		g_enemy[i].Init();
		g_enemy[i].InitEnemyStatus(g_enemypos[i], 100, 20, 50/*,M_wolf*/);

	}



	Boss.Init();
	Boss.InitBossStatus(BossPos, 500, 30, 100);

	WriteOnConsole("敵を初期化した\n");
		

	//単位行列にする
	DX11MtxIdentity(g_mtxskydome);
	DX11MtxIdentity(g_mtxground);
	XMFLOAT3 oringin = { 0.0f,0.0f,0.0f };
	DX11MakeWorldMatrix(g_mtxground, oringin, oringin, 1.0f);

	return true;
}

void Object::Updata() {


	static XMFLOAT3 angle = { 0.0f,0.0f,0.0f };
	//角度情報

	static XMFLOAT3 trans ={ 0.0f, 0.0f,0.0f };

	Player::GetInstance().Updata();

	//for (int i = 0; i < ENEMYMAX; i++)
	//{
	//	g_enemy[i].UpData();		
	//}

	//Boss.UpDataBoss();

	//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F1)) {
	//	Cmode = LookPlayer;
	//}
	//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F2)) {
	//	Cmode = FollowPlayer;
	//}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_F3)) {
		OBBOn = !OBBOn;	
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_F4)) {
		LineOn = !LineOn;
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_F5)) {
		ImGuiOn = !ImGuiOn;
	}

	DX11MakeWorldMatrix(g_mtxskydome, angle, trans, 0.3f);
	g_mtxskydome._41 = Player::GetInstance().GetPlayerMat()._41;
	g_mtxskydome._42 = Player::GetInstance().GetPlayerMat()._42;
	g_mtxskydome._43 = Player::GetInstance().GetPlayerMat()._43;	

}

void Object::Render()
{
	if (scene != LOAD) 
	{

		Player::GetInstance().Render(OBBOn, false);
		// モデル描画		
		//g_ground.Draw(g_mtxground, "shader/vs.fx", "shader/ps.hlsl", LineOn);
		g_ground.DrawToon(g_mtxground,vs,ps, LineOn);	

		g_Skydome.Draw(g_mtxskydome, "shader/vs.fx", "shader/psskydome.fx", false);
	}
	
	if (scene == GAME) 
	{
		
		//Enemy::GetInstance().Render(OBBOn, false);
		for (int i = 0; i < ENEMYMAX; i++)
		{
			g_enemy[i].Render(OBBOn, false);
		}	

		Boss.Render(OBBOn, false);

		if (ImGuiOn)ImGuiRender();
	}


}

void Object::MiniMapRender()
{
	
	//g_Skydome.Draw(g_mtxskydome, false);
	//g_ground.Draw(g_mtxground, "shader/vs.fx", "shader/ps.hlsl",LineOn);
	g_ground.DrawToon(g_mtxground, vs, ps, LineOn);

	Player::GetInstance().RenderInMiniMap();

	for (int i = 0; i < ENEMYMAX; i++)
	{
		g_enemy[i].RenderMiniMap();
	}

	Boss.RenderMiniMapBoss();

	

}

void Object::ObjectExit() 
{
	Player::GetInstance().PlayerExit();

	//for (int i = 0; i = ENEMYMAX; i++)
	//{
	//	g_enemy[i].EnemyExit();
	//}

	//Boss.EnemyExit();

	g_Skydome.Uninit(false);
	g_ground.Uninit(false);
}



void Object::ImGuiRender()
{
	extern float nowFPS;
	bool sts = false;
	//Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	std::string s;
	std::string s2;

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!)
	ImGui::Begin(u8"Object");

	ImGui::Text("FPS  %.2f", nowFPS);
	s = u8"OBBは";

	if (OBBOn)
	{
		s2 = u8"ON";
		ImGui::Text("%s %s", s.c_str(), s2.c_str());
	}
	else
	{
		s2 = u8"OFF";
		ImGui::Text("%s %s", s.c_str(), s2.c_str());
	}

	static ImGuiComboFlags Playerflags = 1;
	static ImGuiComboFlags Enemyflags = 1;
	ImGui::CheckboxFlags("PlayerImGuiOnoff", (unsigned int*)&Playerflags, ImGuiComboFlags_PopupAlignLeft);
	ImGui::CheckboxFlags("EnemyImGuiOnoff", (unsigned int*)&Enemyflags, ImGuiComboFlags_PopupAlignLeft);

	ImGui::Text("%d", g_ground.GetModelData().GetMeshes().size());

	ImGui::Text("1::HomeposX:%.2f HomeposY:%.2f  HomeposZ:%.2f ",
		g_enemy[0].GetHomepos().x, g_enemy[0].GetHomepos().y, g_enemy[0].GetHomepos().z);

	ImGui::Text("2::HomeposX:%.2f HomeposY:%.2f  HomeposZ:%.2f ",
		g_enemy[1].GetHomepos().x, g_enemy[1].GetHomepos().y, g_enemy[1].GetHomepos().z);

	for (int i = 0; i < ENEMYMAX; i++) 
	{
		ImGui::Text("%d Len:%.2f", i + 1, Player::GetInstance().GetTargetNum());

	}
	/*for (int i = 0; i < ENEMYMAX; i++) {
		ImGui::Text("%dHP%.2f", i, g_enemy[i].GetEnemyHP());

		if (g_enemy[i].GetDead()) 
		{
			ImGui::Text("DeadTRUE");
		}
		else{ ImGui::Text("DeadFalse"); }

		if (g_enemy[i].Reborn())
		{
			ImGui::Text("RebornTRUE");
		}
		else { ImGui::Text("RebornFalse"); }

		ImGui::Text("Cnt:%d", g_enemy[i].RebornCnt());
	}*/
	ImGui::Text("movemet%.2f", Player::GetInstance().getmovement());
	
	
	ImTextureID my_tex_id = RenderTexture::GetInstance()->GetOutputTexture();

	float my_tex_w = 200;
	float my_tex_h = 200;
	ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	
	ImGui::End();
	
	if(Playerflags==1)Player::GetInstance().ImGuiRender();
	
	if (Enemyflags == 1)g_enemy[0].ImGuiRender();
	

	// Rendering
	imguidraw();
}

void Object::HitTurnRed(std::vector<COBB*> a, std::vector<COBB*> b) {
	
	bool sts = false;
	
	for (auto Attacker : a)
	{
		ImGui::Text("%s", Attacker->GetName().c_str());
		for (auto Attacked : b)
		{
			sts = Attacker->Collision(*Attacked);
			if (sts)break;
		}

		if (sts)
		{
			ImGui::Text("%s", Attacker->GetName().c_str());
			Attacker->SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.3f));
		}
		else
		{
			//Attacker->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
}


