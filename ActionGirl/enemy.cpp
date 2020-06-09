
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "CModel.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "imguiutil.h"
#include "enemy.h"
#include "object.h"
#include "StageHit.h"
#include"UI.h"
#include"CCamera.h"

#define Wait 40



bool Enemy::Init(CModel* cmodel) {
	
	static bool sts;

	g_modelenemy = cmodel;
	/*sts = g_modelenemy.Init("assets/wolf/Wolf.fbx", "shader/vsoneskin.hlsl", "shader/psmira.hlsl",
		"assets/wolf/textures/");*/

	//sts = g_modelenemy->InitToon("assets/wolf/Wolf.fbx", vsfilename, psfilename,"assets/wolf/textures/");
	//if (!sts) {
	//	MessageBox(NULL, "load enemy model error", "error", MB_OK);
	//	return false;
	//}
	

	// ASSIMPを使用したアニメーションの読み込み
	sts = g_modelenemy->LoadAnimation("assets/wolf/Wolf.fbx");
	if (!sts) {
		MessageBox(NULL, "load enemy animation error", "error", MB_OK);
		return false;
	}

	Billboarinit(g_eMiniMapPos, 1.5f, 1.5f, 0, 1, 0, 1, "assets/pic/MiniMapWolf.png");

	Billboarinit(BossMiniMapPos, 1.5f, 1.5f, 0, 1, 0, 1, "assets/pic/mao.png");

	DX11MtxIdentity(g_mtxenemy);
	g_modelenemy->GetOBBList(g_enemyobblist);
	
	return true;
}

void Enemy::InitEnemyStatus(XMFLOAT3 enemypos,int HP, int Attack, int EXP/*,CModel model*/)
{
	ReBorn = false;
	ReBornCnt = 0;
	EnemyActive = StandE;
	EnemyStatus.HPMAX = HP;
	EnemySize = 1.0f;
	EnemyStatus.HP = HP;
	EnemyStatus.Attack = Attack;
	EnemyStatus.EXP = EXP;
	EnemyHomePos= g_EnemyPos = enemypos;		
}

void Enemy::InitBossStatus(XMFLOAT3 enemypos, int HP, int Attack, int EXP/*,CModel model*/)
{
	ReBorn = false;
	ReBornCnt = 0;
	EnemyActive = StandE;
	EnemyStatus.HPMAX = HP;
	EnemySize = 3.0f;
	EnemyStatus.HP = HP;
	EnemyStatus.Attack = Attack;
	EnemyStatus.EXP = EXP;
	EnemyHomePos = g_EnemyPos = enemypos;
}

void Enemy::UpData()
{
	seangle = angle;
	
	//DX11MakeWorldMatrix(g_mtxenemy, angle, g_EnemyPos, 30.0f);
	DX11MakeWorldMatrix(g_mtxenemy, angle, g_EnemyPos, EnemySize);

	g_modelenemy-> Update(animno,			// アニメーション番号
		g_mtxenemy, 1);				// モデル表示位置と姿勢


	//GetLenToTarget(g_EnemyPos, Player::GetInstance().GetPlayerPos());
	

	if (EnemyStatus.HP >= 0)
	{
		GetGround();
		angle.x = GetUpAngle(g_EnemyPos, EnemyPosFront);

		switch (EnemyActive)
		{
		case StandE:
			//animno = 0;
			Enemyaround(g_EnemyPos,0.0f,5);


			if (Player::GetInstance().GetPlayerHP() >= 0)
			{

				if (CHit(g_EnemyPos, Player::GetInstance().GetPlayerPos(), 0.2f, 0.2f) ||
					Hit(g_enemyobblist, Player::GetInstance().GetPlayerObblist()))
				{
					EnemyActive = FollowE;

					break;
				}
			}

		
			break;

		case FollowE:
			
			angle.y = GetAngleToTarget(g_EnemyPos, Player::GetInstance().GetPlayerPos());
			//
			animno = 3;
			movement = 0.03f;

			if (Hitbysword(g_enemyobblist, Player::GetInstance().GetPlayerObblist()))
			{
				EnemyActive = StandToAttack;
				break;
			}
			if (Hit(g_enemyobblist, Player::GetInstance().GetPlayerObblist()))
			{
				EnemyActive = StandToAttack;
				break;
			}

			if (Player::GetInstance().GetPlayerHP() <= 0) {
				EnemyActive = StandE;
				E_around = GoHome;
				break;
			}

			//if (!CHit(g_EnemyPos, Player::GetInstance().GetPlayerPos(),8,25))
			//{
			//	//GotTarget = true;	
			//	animno = 3;
			//	movement = 0.06f;
			//
			//	if (LenToTarget > 300) 
			//	{
			//		
			//	}
			//}
			//else {
			//	//GotTarget = false;
			//	animno = 0;
			//	movement = 0.0f;
			//}


			Move(g_EnemyPos, g_mtxenemy, movement);

			break;

		case AttackTarget:
			animno = 0;
			break;


		case StandToAttack:

			animno = 0;
			angle.y = GetAngleToTarget(g_EnemyPos, Player::GetInstance().GetPlayerPos());
			static int cnt = 0;
			cnt++;
			if (cnt >= 100) {
				EnemyActive = FollowE;
				cnt = 0;
				break;
			}

			break;
				
		}
	}

	if (Die()) 
	{
		ReBornNow();
	}

	if (g_EnemyPos.y < -30.0f) 
	{
		EnemyStatus.HP = 0;
	}
	
	//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	//{
	//	EnemyStatus.HP = 0;
	//}

	g_eMiniMapPos.SetPosition(g_mtxenemy._41, 15, g_mtxenemy._43);

}

void Enemy::UpDataBoss()
{
	seangle = angle;

	//DX11MakeWorldMatrix(g_mtxenemy, angle, g_EnemyPos, 30.0f);
	DX11MakeWorldMatrix(g_mtxenemy, angle, g_EnemyPos, EnemySize);

	g_modelenemy->Update(animno,			// アニメーション番号
		g_mtxenemy, 1);				// モデル表示位置と姿勢


	//GetLenToTarget(g_EnemyPos, Player::GetInstance().GetPlayerPos());


	if (EnemyStatus.HP >= 0)
	{
		GetGround();
		angle.x = GetUpAngle(g_EnemyPos, EnemyPosFront);

		switch (EnemyActive)
		{
		case StandE:
			//animno = 0;
			Enemyaround(g_EnemyPos, 0.0f, 5);


			if (Player::GetInstance().GetPlayerHP() >= 0)
			{

				if (CHit(g_EnemyPos, Player::GetInstance().GetPlayerPos(), 0.6f, 0.2f) ||
					Hit(g_enemyobblist, Player::GetInstance().GetPlayerObblist()))
				{
					EnemyActive = FollowE;

					break;
				}
			}


			break;

		case FollowE:

			angle.y = GetAngleToTarget(g_EnemyPos, Player::GetInstance().GetPlayerPos());
			//
			animno = 3;
			movement = 0.03f;

			if (Hitbysword(g_enemyobblist, Player::GetInstance().GetPlayerObblist()))
			{
				EnemyActive = StandToAttack;
				break;
			}

			if (CHit(g_EnemyPos, Player::GetInstance().GetPlayerPos(), 0.6f, 0.2f) ||
				Hit(g_enemyobblist, Player::GetInstance().GetPlayerObblist()))
			{
				EnemyActive = StandToAttack;
				break;
			}

			if (Player::GetInstance().GetPlayerHP() <= 0) {
				EnemyActive = StandE;
				E_around = GoHome;
				break;
			}

			//if (!CHit(g_EnemyPos, Player::GetInstance().GetPlayerPos(),8,25))
			//{
			//	//GotTarget = true;	
			//	animno = 3;
			//	movement = 0.06f;
			//
			//	if (LenToTarget > 300) 
			//	{
			//		
			//	}
			//}
			//else {
			//	//GotTarget = false;
			//	animno = 0;
			//	movement = 0.0f;
			//}


			Move(g_EnemyPos, g_mtxenemy, movement);

			break;

		case AttackTarget:
			animno = 0;
			break;


		case StandToAttack:

			animno = 0;
			angle.y = GetAngleToTarget(g_EnemyPos, Player::GetInstance().GetPlayerPos());
			static int cnt = 0;
			cnt++;
			if (cnt >= 100) {
				EnemyActive = FollowE;
				cnt = 0;
				break;
			}

			break;

		}
	}


	if (g_EnemyPos.y < -30.0f)
	{
		EnemyStatus.HP = 0;
	}

	BossMiniMapPos.SetPosition(g_mtxenemy._41, 15, g_mtxenemy._43);

}
void Enemy::ReBornNow() 
{	
		ReBornCnt++;
		if (ReBornCnt >= 500)
		{			
			InitEnemyStatus(EnemyHomePos, EnemyStatus.HPMAX, EnemyStatus.Attack, EnemyStatus.EXP);
		}
}

void Enemy::Render(bool ObbOn, bool LineOn)
{
	
	//if (EnemyStatus.HP > 0)
	if(EnemyActive!=DieP && !Die())
	{
		//g_modelenemy.Draw(g_mtxenemy, LineOn);
		g_modelenemy->DrawToon(g_mtxenemy, vsfilename, psfilename, LineOn);
	}
	
	if (ObbOn)
	{
		g_modelenemy->DrawOBB();
	}
}

void Enemy::RenderMiniMap() {
	XMFLOAT4X4 Minimapmat;
	Minimapmat = CCamera::GetInstance()->GetCameraMatrix();
	if (EnemyActive != DieP && !Die())
	{
		g_eMiniMapPos.DrawBillBoard(Minimapmat);
	}
}

void Enemy::RenderMiniMapBoss() {
	XMFLOAT4X4 Minimapmat;
	Minimapmat = CCamera::GetInstance()->GetCameraMatrix();
	if (EnemyActive != DieP && !Die())
	{
		BossMiniMapPos.DrawBillBoard(Minimapmat);
	}
}

void Enemy::SetMiniMapEnemySize(bool minimapBig) 
{
	if (minimapBig)
	{
		g_eMiniMapPos.Init(0, 0, 0, 4.0f, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		g_eMiniMapPos.Init(0, 0, 0, 2.0f, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Enemy::SetMiniMapBossSize(bool minimapBig)
{
	if (minimapBig)
	{
		BossMiniMapPos.Init(0, 0, 0, 8.0f, 8.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		BossMiniMapPos.Init(0, 0, 0, 4.0f, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Enemy::Move(XMFLOAT3& mypos, XMFLOAT4X4& mymat,float move)
{
	//XMFLOAT3 Govector;
	//Govector.x=Targetpos.x - mypos.x;
	////Govector.y = Targetpos.y - mypos.y;
	//Govector.z = Targetpos.z  - mypos.z;
	//mypos.x += Govector.x*move;
	////mypos.y += Govector.y*move;
	//mypos.z += Govector.z*move;
	//mymat._41 = mypos.x;
	////mymat._42 = mypos.y;
	//mymat._43 = mypos.z;

	mypos.x -= mymat._31*move;
	mypos.y -= mymat._32*move;
	mypos.z -= mymat._33*move;
	mymat._41 = mypos.x;
	mymat._42 = mypos.y;
	mymat._43 = mypos.z;
}

float Enemy::GetAngleToTarget(XMFLOAT3& mypos, XMFLOAT3 Targetpos)
{
	
	float X, Z, TurnAngle;
	X = Targetpos.x - mypos.x;
	Z = Targetpos.z - mypos.z;
	TurnAngle = atan2(Z, X);
	
	return  Rturn = -(TurnAngle * 180 / PI) - 90;
	//return  Rturn = -(TurnAngle * 180/ PI);
}

bool Enemy::GetLenToTarget(XMFLOAT3& mypos, XMFLOAT3 Targetpos)
{
	float X,Y, Z, len;
	X = Targetpos.x - mypos.x;
	Y = Targetpos.y - mypos.y;
	Z = Targetpos.z - mypos.z;
	len = sqrt(X * X + Y * Y + Z * Z);

	return len; 
}

float Enemy::GetUpAngle(XMFLOAT3& mypos, XMFLOAT3& frontpos)
{
	frontpos.x = g_mtxenemy._41 - g_mtxenemy._31;
    frontpos.z = g_mtxenemy._43 - g_mtxenemy._33;

	float X, Y, Z, len;
	X= frontpos.x - mypos.x;
	Y = frontpos.y - mypos.y;
	Z = frontpos.z - mypos.z;
	len = sqrt(X * X + Y * Y + Z * Z);

	return  UpAngle = tan(Y / len) * 180 / PI;
}

//float Enemy::GetSideAngle(XMFLOAT3& mypos, XMFLOAT3& Lside, XMFLOAT3& Rside) 
//{
//	Lside.x = g_mtxenemy._41 - g_mtxenemy._11;
//	Lside.z= g_mtxenemy._43 - g_mtxenemy._13;
//
//	float X,Y,Z, len;
//
//	X = Lside.x - mypos.x;
//	Y = Lside.y - mypos.y;
//	Z = Lside.z - mypos.z;
//	len = sqrt(X * X + Y * Y + Z * Z);
//		
//	return SideAngle =  tan(Y / len) * 180 / PI;
//}

void Enemy::GetGround() 
{
	XMFLOAT3 ans;

	bool sts = SearchAllSurface(g_EnemyPos, ans);
	if (sts) 
	{
		//GoDown = false;
		//ans.y += 0.1f;			//
		ans.y += 0.0005f;
		g_EnemyPos.y = ans.y;
	}

	sts = SearchAllSurface(EnemyPosFront, ans);
	if (sts) 
	{
		//GoDown = false;
		//ans.y += 0.1f;			// 
		ans.y += 0.0005f;
		EnemyPosFront.y = ans.y;
	}

	//sts = SearchAllSurface(EnemyPosLSide, ans);
	//if (sts)
	//{
	//	//GoDown = false;
	//	ans.y += 0.1f;			// 
	//	EnemyPosLSide.y = ans.y;
	//}
}

bool Enemy::Hit(std::vector<COBB*> a, std::vector<COBB*> b) {

	bool sts = false;

	for (auto Attacker : a)
	{
		for (auto Attacked : b)
		{
			sts = Attacker->Collision(*Attacked);
			if (sts)break;
		}

		if (sts)
		{
			//ImGui::Text("%s", Attacker->GetName().c_str());
			//Attacker->SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.3f));
			return true;
		}
		else
		{
			//Attacker->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f));
			return false;
		}
	}
}


bool Enemy::Hitbysword(std::vector<COBB*> a, std::vector<COBB*> b) {

	bool sts = false;

	for (auto Attacker : a)
	{
		for (auto Attacked : b)
		{
			sts = a[11]->Collision(*Attacked);
			if (sts)break;
		}

		if (sts)
		{
			//ImGui::Text("%s", Attacker->GetName().c_str());
			//Attacker->SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.3f));
			return true;
		}
		else
		{
			//Attacker->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f));
			return false;
		}
	}
}

bool Enemy::CHit(XMFLOAT3& mypos, XMFLOAT3 targetpos, float myR, float tR)
{
	float X, Y, Z;
	X = mypos.x - targetpos.x;
	Y = mypos.y - targetpos.y;
	Z = mypos.z - targetpos.z;

	if ((X * X) + (Y * Y) + (Z * Z) <= pow((myR + tR), 2))
	{
		return true;
	}
	if ((X * X) + (Y * Y) + (Z * Z) > pow((myR + tR), 2))
	{
		return false;
	}
	/*(xc1 - xc2) ^ 2 + (yc1 - yc2) ^ 2 ≦(r1 + r2) ^ 2*/
}

bool Enemy::GetGopos(XMFLOAT3 nowpos,float Min,int Max) 
{

	XMFLOAT3 ans;
	static int PlusOrMainus;

	PlusOrMainus = rand() % 10;
	if (PlusOrMainus <= 5) {
		EnemyGoPos = { nowpos.x + GetfloatRand(Min,Max),nowpos.y,nowpos.z + GetfloatRand(Min,Max) };
	}
	if (PlusOrMainus > 5) 
	{
		EnemyGoPos = { nowpos.x - GetfloatRand(Min,Max),nowpos.y,nowpos.z - GetfloatRand(Min,Max) };
	}

	bool sts = SearchAllSurface(EnemyGoPos, ans);
	
	if (sts)
	{
		//GoDown = false;
		//ans.y += 0.1f;			//
		ans.y += 0.0005f;
		EnemyGoPos.y = ans.y;

		return true;
	}
	else
	{
		return false;
	}
	
}

void Enemy::Enemyaround(XMFLOAT3 nowpos, float Min, int Max) {

	switch (E_around)
	{
	case FindGo:
		animno = 0;
		movement = 0;

		GetLenToTarget(nowpos, EnemyHomePos);
		if (LenToTarget >= 10.0f) {
			static int cntGo = 0;
			cntGo++;
			if (cntGo > Wait)
			{
				E_around = GoHome;
				break;
			}
		}

		while (GetGopos(nowpos, Min, Max) == false)
		{
			GetGopos(nowpos, Min, Max);
			
		}

		if (GetGopos(nowpos, Min, Max))
		{
			static int cntGo = 0;
			cntGo++;
			if (cntGo > Wait)
			{
				cntGo = 0;
				E_around = ToGo;				
			}
		}
		
		break;

	case ToGo:

		animno = 5;
		GetLenToTarget(nowpos, EnemyHomePos);
		if (LenToTarget >= 10.0f) 
		{
			static int cntGo = 0;
			cntGo++;
			if (cntGo > Wait)
			{
				cntGo = 0;
				E_around = GoHome;
				break;
			}
		}
	
		if (CHit(g_EnemyPos, EnemyGoPos, 0.1f, 0.1f))
		{
			animno = 0;
			static int cntGo = 0;
			cntGo++;
			if (cntGo > Wait)
			{
				cntGo = 0;
				E_around = GoHome;
				break;
			}
		}
		else{ 
			
			angle.y = GetAngleToTarget(g_EnemyPos, EnemyGoPos);
			movement = 0.01f; 
			Move(g_EnemyPos, g_mtxenemy, movement);
		}
		break;

	case GoHome:

		animno = 5;
		

		if (CHit(g_EnemyPos, EnemyHomePos, 1.0f, 1.0f))
		{
			animno = 0;
			static int cntGo = 0;
			cntGo++;
			if (cntGo > Wait)
			{
				cntGo = 0;
				E_around = FindGo;
				break;
			}
		}
		else 
		{
			angle.y = GetAngleToTarget(g_EnemyPos, EnemyHomePos);
			movement = 0.01f;
			Move(g_EnemyPos, g_mtxenemy, movement);
		}
		break;
	}
}


bool Enemy::Die() 
{
	if (EnemyStatus.HP <= 0) 
	{
		if (angle.y <= 2000) 
		{
			angle.y += 20;
		}
		EnemySize -= 0.02;

		if (EnemySize <= 0)
		{
			g_EnemyPos.y = 500.0f;
			ReBorn = true;
			return true;
		}

		else
		{
			return false;
		}
	}

	else 
	{ 
		return false; 
	}
	
}

float Enemy::GetfloatRand(float Min,int Max) 
{
	return Min + ((float)rand() / ((float)RAND_MAX / (Max - Min)));
}

void Enemy::ImGuiRender() 
{
	
	ImGui::Begin(u8"Enemy");

	std::string s;

	s = u8"OBBは";

	s = u8"現在のアニメーションnumber";
	ImGui::Text("%s %d", s.c_str(),animno);

	ImGui::Text("X:%.2f,Y:%.2f,Z:%.2f", g_EnemyPos.x, g_EnemyPos.y, g_EnemyPos.z);
	ImGui::Text("FX:%.2f,FY:%.2f,FZ:%.2f", EnemyPosFront.x, EnemyPosFront.y, EnemyPosFront.z);
	ImGui::Text("Upangle:%.2f", UpAngle);

	ImGui::Text("angleX:%.2f,angleY:%.2f,angleZ:%.2f", seangle.x, seangle.y, seangle.z);
	ImGui::Text("Rturn:%.2f", Rturn);

	//if (Hit(g_enemyobblist, Player::GetInstance().GetPlayerObblist())) 
	if(CHit(g_EnemyPos, Player::GetInstance().GetPlayerPos(),8,25))
	{
		ImGui::Text("Hit:true");
	}
	else {
		ImGui::Text("Hit:false");
	}

	s= u8"プレイヤーとの距離";
	ImGui::Text("%s:%.2f", s.c_str(), LenToTarget);

	//FollowOnOff
	static ImGuiComboFlags Followflags = 0;
	ImGui::CheckboxFlags("Follow Player", (unsigned int*)&Followflags, ImGuiComboFlags_PopupAlignLeft);
	if (Followflags == 1) 
	{
		EnemyActive = FollowE;
	}
	else 
	{
		EnemyActive = StandE;
	}//FollowOnOff END

	if (E_around == FindGo) {
		ImGui::Text("Mode:FinDGo");
	}

	if (E_around == ToGo) {
		ImGui::Text("Mode:ToGo");
	}

	if (E_around == GoHome) {
		ImGui::Text("Mode:GoHome");
	}

	ImGui::Text("GoX:%.2f  GoY:%.2f  GoZ:%.2f", EnemyGoPos.x, EnemyGoPos.y, EnemyGoPos.z);

	ImGui::Text("HomeX:%.2f  HomeY:%.2f  HomeZ:%.2f", EnemyHomePos.x, EnemyHomePos.y, EnemyHomePos.z);

	ImGui::End();
}

void Enemy::EnemyExit() 
{
	g_modelenemy->Uninit(true);
}