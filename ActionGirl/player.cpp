#include <windows.h>
#include <stdio.h>
#include "CModel.h"
#include "CDirectInput.h"
#include "object.h"
#include "imguiutil.h"
#include "player.h"
#include "game.h"
#include "StageHit.h"
#include "CCamera.h"
#include"UI.h"

extern bool mouseOnoff;
extern Enemy g_enemy[ENEMYMAX];
extern Enemy  Boss;
//extern CameraMode Cmode;
extern SCENE scene;
extern UPorDown UPDOWN;
extern float CHight;

const char* vsfilename[] = {
	"shader/vsoneskin.hlsl",
	"shader/vsoutline.fx",
};

// ピクセルシェーダー
const char* psfilename[] = {
	"shader/pstoon.fx",
	"shader/psoutline.fx",
};

//プレイヤー初期化
bool Player::Init() 
{
	bool sts;

	/*sts = g_model.Init("assets/girl.x", "shader/vsoneskin.hlsl", "shader/psmira.hlsl",
		"");*/
	sts = g_model.InitToon("assets/girl.x", vsfilename, psfilename,"");
	if (!sts)
	{
		MessageBox(NULL, "load enemy model error", "error", MB_OK);
		return false;
	}

	DX11MtxIdentity(g_mtxplayer);

	// ASSIMPを使用したアニメーションの読み込み
	sts = g_model.LoadAnimation("assets/girl.x");
	if (!sts) {
		MessageBox(NULL, "load enemy animation error", "error", MB_OK);
		return false;
	}

	Billboarinit(g_pMiniMapPos, 1.5f, 1.5f, 0, 1, 0, 1, "assets/pic/miniplayer.png");


	g_model.GetOBBList(g_playerobblist);


	InitPlayerStatus();

	return true;
}

//プレイヤーステータス初期化
void Player::InitPlayerStatus() 
{
	CHight = 0.7f;
	MoveMode = MoveToFront;
	PStatus.Level = 1;
	PStatus.HP = 50 + PStatus.Level * 50;
	PStatus.MAXHP = 50 + PStatus.Level * 50;
	PStatus.Attack = 20 + PStatus.Level * 5;
	PStatus.Defense = 15 + PStatus.Level * 5;
	g_PlayerPos = { 17.0f,-1.0f,-37.0f };

	dead = false;
	Nowmove = false;
	Attacknow = false;
	HitEnemy = false;
	HitBoss = false;
	lockinput = false;
	ReStart = false;
	/*XMFLOAT3 */angle = { 0.0f,140.0f,0.0f };
	angleCamera = { 0.0f,140.0f,0.0f };
	DX11MakeWorldMatrix(g_mtxplayer, angle, g_PlayerPos, 0.5f);
	DX11MakeWorldMatrix(g_mtxplayerDie, angle, g_PlayerPos, 0.5f);

	g_model.Update(1,			// アニメーション番号
		g_mtxplayer, 8);				// モデル表示位置と姿勢

	Move(g_PlayerPos, g_mtxplayer, -0.1f);
	gobackTitle = false;
}

//レベルアップ処理
void Player::LevelUP() 
{
	if (PStatus.EXP >= (PStatus.Level * 50)) {
		PStatus.EXP = PStatus.EXP - PStatus.Level * 50;
		PStatus.Level++;
		PStatus.MAXHP = 50 + PStatus.Level * 50;
		PStatus.MAXEXP = PStatus.Level * 50;
		PStatus.HP = PStatus.MAXHP;
		PStatus.Attack = 20 + PStatus.Level * 5;
		PStatus.Defense = 15 + PStatus.Level * 5;
	}	
}

void Player::TitleUpdata() 
{
	/*static XMFLOAT3 */angle = { 0.0f,0.0f,0.0f };
	g_model.Update(animno,			// アニメーション番号
		g_mtxplayer, animespeed);				// モデル表示位置と姿勢
	DX11MakeWorldMatrix(g_mtxplayer, angle, g_PlayerPos, 0.5f);
	
	g_PlayerPos = { -0.7f,2.0f,-1.0f };
	GetPlayerGround();
	animno = 0;
	animespeed = 8;
}

//プレイヤー更新
void Player::Updata() {


	Nowmove = false;

	if (!dead)
	{
		g_model.Update(animno,			// アニメーション番号
			g_mtxplayer, animespeed);				// モデル表示位置と姿勢
	}

	//DX11MakeWorldMatrix(g_mtxplayer, angle, g_PlayerPos, 0.05f);
	DX11MakeWorldMatrix(g_mtxplayer, angle, g_PlayerPos, 0.3f);
	DX11MakeWorldMatrix(g_mtxplayerDie, angleCamera, g_PlayerPos, 0.3f);

	operating();//プレイヤー操作
	GetplayerFPos();
	GetplayerBPos();
	GetPlayerGround();
	Movemode();	
	HitByEnemy();	
	LevelUP();
	
	g_pMiniMapPos.SetPosition(g_mtxplayer._41,15, g_mtxplayer._43);

	Nowmove = false;
}

//描画
void Player::Render(bool Obbon , bool LineOn) {
	
	//g_model.Draw(g_mtxplayer,LineOn);
	g_model.DrawToon(g_mtxplayer, vsfilename, psfilename, LineOn);

	if (Obbon) 
	{
		g_model.DrawOBB();		
	}

	//DrawTriangleByNeer();
}

void Player::RenderInMiniMap()
{	
	XMFLOAT4X4 Minimapmat;
	Minimapmat= CCamera::GetInstance()->GetCameraMatrix();
	
	g_pMiniMapPos.DrawRotateBillBoard(Minimapmat, -angleCamera.y+180.0f);
}

void Player::SetMiniMapPlayerSize(bool minimapBig) 
{
	if (minimapBig) 
	{
		g_pMiniMapPos.Init(0, 0, 0, 4.0f, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		g_pMiniMapPos.Init(0, 0, 0, 1.5f, 1.5f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}


void Player::PlayerExit() 
{
	g_model.Uninit(true);
}

void Player::Movemode() 
{
	static int AttackPartCnt = 0;

	switch (MoveMode)
	{
	case StandP:

		movement = 0.0;
		animno = 0;
		animespeed = 8;
		
		AttackPartCnt++;
		if (AttackPartCnt > 30) {
			Attackpartime = false;
			AttackPartCnt = 0;
		}
		
		break;

	case MoveToFront:

		angle.y = angleCamera.y;
		MoveF();
		Attackpartime = false;
		break;

	case MoveFL:
		angle.y = angleCamera.y - 45.0f;
		MoveF();
		break;
	case MoveFR:
		angle.y = angleCamera.y + 45.0f;

		MoveF();
		Attackpartime = false;
		break;

	case MoveBack:

		MoveF();
		angle.y = angleCamera.y + 180.0f;
		//MoveB();
		Attackpartime = false;
		break;

	case MoveBL:
		angle.y = angleCamera.y - 135.0f;

		MoveF();

		break;
	case MoveBR:
		angle.y = angleCamera.y + 135.0f;

		MoveF();
		Attackpartime = false;
		break;

	case MoveLeft:

		MoveF();
		angle.y = angleCamera.y - 90.0f;
		//GetPlayerGround();
		Attackpartime = false;
		break;

	case MoveRight:

		MoveF();
		angle.y = angleCamera.y + 90.0f;
		Attackpartime = false;
		break;


	case Attack:

		AttackMath();

		break;

	case DieP:

		dead = true;
		lockinput = true;

		angleCamera.y += 0.5f;

		if (angle.x < 85)
		{
			angle.x += 1.0f;
		}
		static int ReCnt;

		ReCnt++;

		if (ReCnt >= 300)
		{
			ReStart = true;

		}

		if (gobackTitle)
		{
			angle = { 0.0f,140.0f,0.0f, };
			angleCamera = { 0.0f,140.0f,0.0f, };
		}

		break;

	case clear:

		lockinput = true;

		movement = 0.0;
		animno = 0;
		animespeed = 8;

		angleCamera.y += 0.5f;

		ReCnt++;

		if (ReCnt >= 300)
		{
			ReStart = true;

		}

		if (gobackTitle)
		{
			angle = { 0.0f,140.0f,0.0f, };
			angleCamera = { 0.0f,140.0f,0.0f, };
		}

	}

	if (!Nowmove && !Attacknow && !dead)
	{		
		if (!lockinput)
		{			
				MoveMode = StandP;		
		}
	}
}

void Player::MoveF() 
{
	animno = 1;

	animespeed = 3;

	if (MoveMode == MoveToFront || MoveMode == MoveBack) 
	{
		movement = -0.1f+ ChageSpeed();
	}
	else if (MoveMode == MoveLeft || MoveMode == MoveRight) {
		movement = -0.08f+ ChageSpeed();
	}


	if (GetPlayerFGround())
	{
		for (int i = 0; i < ENEMYMAX; i++)
		{
			if (!Hit(g_playerobblist, g_enemy[i].GetEnemyObblist())|| !Hit(g_playerobblist, Boss.GetEnemyObblist()))
				//if (!CHit(g_PlayerPos, Enemy::GetInstance().GetEnemyPos(), 8, 8))
			{
				Move(g_PlayerPos, g_mtxplayer, movement);
			}
		}
	}
}

float Player::ChageSpeed() {
	static float obstacle;
	if (UPDOWN == GOUP)obstacle = 0.04f;
	if (UPDOWN == GODOWN)obstacle = -0.001f;
	if (UPDOWN == FLAT)obstacle = 0.0f;

	return obstacle;
}

void Player::AttackMath() 
{
	Attacknow = true;
	if (Attackpartime)
	{
		animno = 4;
		//animno = 5;
		//attack = true;
		animespeed = 4;
	}
	else {
		animno = 5;
		//animno = 5;
		//attack = true;
		animespeed = 4;
	}
	lockinput = true;
	Atcount++;
	if (Atcount > 10)
	{
		if (!Damageto)
		{
			//if (HitMonster(g_playerobblist, Enemy::GetInstance().GetPlayerObblist()))
			for (int i = 0; i < ENEMYMAX; i++)
			{
				if (HitMonster(g_playerobblist, g_enemy[i].GetEnemyObblist()))
				{
					
					TargetNum = i;
					HitEnemy = true;
					HitBoss = false;
					g_enemy[i].DamageToEenmy(PStatus.Attack);

					if (g_enemy[TargetNum].GetEnemyHP() <= 0)
					{
						PStatus.EXP += g_enemy[TargetNum].GetEXP();
						HitEnemy = false;
						//TargetNum = 2;
						//break;
					}

					Damageto = true;
					//Enemy::GetInstance().DamageToEenmy(PStatus.Attack);					
					break;
				}			
			}

			if (HitMonster(g_playerobblist, Boss.GetEnemyObblist()))
			{
				HitBoss = true;
				HitEnemy = false;
				Boss.DamageToEenmy(PStatus.Attack);

				if (Boss.GetEnemyHP() <= 0)
				{
					PStatus.EXP += Boss.GetEXP();
					HitEnemy = false;
					MoveMode = clear;
					//TargetNum = 2;
					//break;
				}

				Damageto = true;
				//Enemy::GetInstance().DamageToEenmy(PStatus.Attack);					
				//break;
			}
		}
	}
	if (Attackpartime) 
	{
		if (Atcount > 40)
		{
			lockinput = false;
			Atcount = 0;
			Attacknow = false;
			Damageto = false;
		}
	}
	else {
		if (Atcount > 50)
		{
			lockinput = false;
			Atcount = 0;
			Attacknow = false;
			Damageto = false;
		}
	}
}

void Player::HitByEnemy() 
{
	for (int i = 0; i < ENEMYMAX; i++)
	{
		if (PStatus.HP > 0)
		{
		/*	if (Hit(g_playerobblist, g_enemy[i].GetEnemyObblist()))
			{*/
			if (CHit(g_PlayerPos, g_enemy[i].GetEnemyPos(), 0.1f, 0.1f))
			{
				if (g_enemy[i].GetEnemyHP() > 0)
				{
					TargetNum = i;
					HitEnemy = true;
					HitBoss = false;
					PStatus.HP -= g_enemy[i].GetEnemyAttack();
					Move(g_PlayerPos, g_mtxplayer, 1.5f);
					//GetPlayerGround();
					if (PStatus.HP <= 0)
					{
						animno = 0;
						g_model.setframe();

						lockinput = true;
						MoveMode = DieP;
					}

				}
			}

			if (CHit(g_PlayerPos, Boss.GetEnemyPos(), 0.1f, 0.5f))
			{
				if (Boss.GetEnemyHP() > 0)
				{
					TargetNum = i;
					HitEnemy = false;
					HitBoss = true;
					PStatus.HP -= Boss.GetEnemyAttack();
					Move(g_PlayerPos, g_mtxplayer, 3.5f);
					//GetPlayerGround();
					if (PStatus.HP <= 0)
					{
						animno = 0;
						g_model.setframe();

						lockinput = true;
						MoveMode = DieP;
					}

				}
			}
		}
	}
}



//地面との当たり判定
bool Player::GetPlayerGround()
{
	XMFLOAT3 ans;
	//int cnt;
	bool sts = SearchAllSurface(g_PlayerPos, ans);
	
	if (sts) 
	{
		//ans.y += 0.5f;	
		ans.y += YUp;
		//neerpoint = FindNearVertex(g_PlayerPos);
		
		SetPos(ans);
	}

	else 
	{
		g_PlayerPos.y-=0.01;
	}

	

	//neergoal = FindNearVertex(goal);
	return sts;
}

bool Player::GetPlayerFGround()
{
	XMFLOAT3 ans;
	//bool godown;g_PlayerPos
	int cnt;
	bool sts = SearchAllSurface(g_pFrontPos, ans);

	if (sts)
	{
		ans.y += YUp;
		SetPosPFront(ans);
	}

	return sts;
}

bool Player::GetPlayerBGround()
{
	XMFLOAT3 ans;
	//bool godown;g_PlayerPos
	int cnt;
	bool sts = SearchAllSurface(g_pBackPos, ans);

	if (sts)
	{
		ans.y += YUp;
		SetPosPFront(ans);
	}

	return sts;
}

//操作
void Player::operating()
{

	if (!lockinput)
	{

		if (CDirectInput::GetInstance().GetMouseMoveX() > 0 && mouseOnoff == true)
		{
			//angle.y += 2.5f;
			angleCamera.y += 2.0f;
		}

		else if (CDirectInput::GetInstance().GetMouseMoveX() < 0 && mouseOnoff == true)
		{
			//angle.y -= 2.5f;
			angleCamera.y -= 2.0f;
		}

		if (CDirectInput::GetInstance().GetMouseMoveY() > 0 && mouseOnoff == true)
		{
			if (CHight <= 5.0f) 
			{
				CHight += 0.03f;
			}
		}

		else if (CDirectInput::GetInstance().GetMouseMoveY() < 0 && mouseOnoff == true)
		{
			if (CHight >= 0.0f)
			{
				CHight -= 0.03f;
			}
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
		{	
			MoveMode = MoveToFront;
			Nowmove = true;		
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {

		
			MoveMode = MoveBack;
			Nowmove = true;
			
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {

			MoveMode = MoveLeft;
			Nowmove = true;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {

			MoveMode = MoveRight;
			Nowmove = true;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) && 
			CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) 
		{
			MoveMode = MoveFL;
			Nowmove = true;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) &&
			CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			MoveMode = MoveFR;
			Nowmove = true;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) &&
			CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
		{
			MoveMode = MoveBL;
			Nowmove = true;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) &&
			CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			MoveMode = MoveBR;
			Nowmove = true;
		}

		if (CDirectInput::GetInstance().GetMouseLButtonCheck()) {
			lockinput = true;
			Attackpartime = !Attackpartime;
			MoveMode = Attack;
			g_model.setframe();
			Nowmove = true;		
		}

	/*	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
		{
			
			MoveMode = Jump;
			Nowmove = true;
		}*/

	}

	if (ReStart) 
	{
		if (CDirectInput::GetInstance().GetMouseLButtonCheck())
		{
			scene = LOAD;
			gobackTitle = true;
			
		}
	}
}

//OBBによるキャラクターの当たり判定
bool Player::Hit(std::vector<COBB*> a, std::vector<COBB*> b) 
{
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

//剣と敵の当たり判定
bool Player::HitMonster(std::vector<COBB*> a, std::vector<COBB*> b)
{
	bool sts = false;

	
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

//移動
void Player::Move(XMFLOAT3& pos, XMFLOAT4X4& mat, float move) 
{
	pos.x += mat._31*move;
	//pos.y += mat._32*move;
	pos.z += mat._33*move;
	mat._41 = pos.x;
	//mat._42 = pos.y;
	mat._43 = pos.z;
}

//円による当たり判定
bool Player::CHit(XMFLOAT3& mypos, XMFLOAT3 targetpos, float myR, float tR)
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
}



//IMGUI
void Player::ImGuiRender() {
	ImGui::Begin(u8"Player");
	std::string s;

	s = u8"現在のアニメーションnumber";
	ImGui::Text("%s %d", s.c_str(), animno);

	
	//ImGui::Text("size%d", g_ground.GetMes().m_meshes.size());

	//ImGui::Text("%.2f", count);

	ImGui::Text("%.2f", movement);

	//ImGui::Text("11:%f,22:%f,33:%f", g_mtxplayer._11, g_mtxplayer._22, g_mtxplayer._33);

	//XMFLOAT3 eye = CCamera::GetInstance()->GetEye();
	//ImGui::Text("EyeX:%f,EyeY:%f,EyeZ:%f", eye.x, eye.y, eye.z);
	
	ImGui::Text("player11:%.2f,player12:%.2f,player13:%.2f,player14:%.2f",
		g_mtxplayer._11, g_mtxplayer._12,
		g_mtxplayer._13, g_mtxplayer._14);

	ImGui::Text("player21:%.2f,player22:%.2f,player23:%.2f,player24:%.2f",
		g_mtxplayer._21, g_mtxplayer._22,
		g_mtxplayer._23, g_mtxplayer._24);

	ImGui::Text("player31:%.2f,player32:%.2f,player33:%.2f,player34:%.2f",
		g_mtxplayer._31, g_mtxplayer._32,
		g_mtxplayer._33, g_mtxplayer._34);

	ImGui::Text("player41:%.2f,player42:%.2f,player43:%.2f,player44:%.2f",
		g_mtxplayer._41, g_mtxplayer._42,
		g_mtxplayer._43, g_mtxplayer._44);

	ImGui::Text("CH:%.2f", CHight);

	/*ImGui::Text("NeerPoint.X:%.2f", neerpoint.x);
	ImGui::Text("NeerPoint.Y:%.2f", neerpoint.y);
	ImGui::Text("NeerPoint.Z:%.2f", neerpoint.z);

	ImGui::Text("");

	ImGui::Text("NeerGoal.X:%.2f", goal.x);
	ImGui::Text("NeerGoal.Y:%.2f", goal.y);
	ImGui::Text("NeerGoal.Z:%.2f", goal.z);
*/
	ImGui::Text("");

	//if (CheckNeedNavi(g_PlayerPos, goal)) {
	//	
	//	ImGui::Text("true");
	//}
	//else {
	//	ImGui::Text("false");
	//}
	/*int i = Geti(g_PlayerPos);
	ImGui::Text("%d", i);
	i = Geti(goal);
	ImGui::Text("%d", i);*/

	//HitTurnRed(Player::GetInstance().GetPlayerObblist(), g_groundobblist);
	//HitTurnRed(Player::GetInstance().GetPlayerObblist(), Enemy::GetInstance().GetPlayerObblist());
	ImGui::Text("angleX%.2f,angleY%.2f,angleZ%.2f", pangle.x,pangle.y,pangle.z);

	ImGui::Text("");

	/*ImGui::Text("PFposX:%.2f  PFposY:%.2f  PFposZ:%.2f", g_pFrontPos.x, g_pFrontPos.y, g_pFrontPos.z);*/

	ImGui::Text("HP:%.2f", PStatus.HP);

	if (MoveMode == StandP) 
	{
		ImGui::Text("MoveMode=StandP");
	}
	if (MoveMode == MoveToFront) 
	{
		ImGui::Text("MoveMode=MoveToFront");
	}
	if (MoveMode == Attack) 
	{
		ImGui::Text("MoveMode=Attack");
	}
	if (MoveMode == DieP) 
	{
		ImGui::Text("MoveMode=DieP");
	}

	ImGui::End();
}