#pragma once
#include "object.h"
#include "CBillBoard.h"

#define ENEMYMAX 3

enum ENEMYACTIVE 
{
	StandE,
	FollowE,
	AttackTarget,
	StandToAttack,
};

enum EnemyAround 
{
	FindGo,
	ToGo,
	GoHome,
};

// 頂点シェーダー
extern const char* vsfilename[];

// ピクセルシェーダー
extern const char* psfilename[];

class EnemyLoad {
public:

	CModel E_model;

	CModel E_modelsave[ENEMYMAX+1];

	void EnemyModelLoad()
	{
		E_model.InitToon("assets/wolf/Wolf.fbx", vsfilename, psfilename, "assets/wolf/textures/");
	
		for (int i = 0; i < ENEMYMAX + 1; i++)
		{
			E_modelsave[i] = E_model;
		}
		// ASSIMPを使用したアニメーションの読み込み
		//E_model.LoadAnimation("assets/wolf/Wolf.fbx");
	}
};


class Enemy 
{
private:
	
	float movement;
	bool GotTarget;
	XMFLOAT3 seangle;
	
	CModel*			g_modelenemy;		// 敵モデル
	XMFLOAT4X4		g_mtxenemy;			// 敵の行列
	XMFLOAT3        g_EnemyPos;
	XMFLOAT3        EnemyPosFront;
	XMFLOAT3        EnemyHomePos;
	XMFLOAT3        EnemyGoPos;
	CBillBoard      g_eMiniMapPos;
	CBillBoard      BossMiniMapPos;
	float EnemySize;
	int ReBornCnt;
	bool ReBorn;
	XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	struct EnemyStatus
	{
		int HP;
		int Attack;
		int HPMAX;
		//bool Alive;
		bool Mad;
		int EXP;

	}EnemyStatus;

	float UpAngle;
	float Rturn;
	float LenToTarget;

	void Move(XMFLOAT3& mypos, XMFLOAT4X4& mymat, float move);
	void Enemyaround(XMFLOAT3 nowpos, float Min, int Max);
	bool GetLenToTarget(XMFLOAT3& mypos, XMFLOAT3 Targetpos);
	void GetGround();
	void ReBornNow();

	float GetAngleToTarget(XMFLOAT3& mypos, XMFLOAT3 Targetpos);
	float GetUpAngle(XMFLOAT3& mypos, XMFLOAT3& frontpos);	
	float GetfloatRand(float Min,int Max);

	bool GetGopos(XMFLOAT3 nowpos, float Min, int Max);
	bool Die();
	bool Hit(std::vector<COBB*> a, std::vector<COBB*> b);
	bool Hitbysword(std::vector<COBB*> a, std::vector<COBB*> b);
	bool CHit(XMFLOAT3& mypos, XMFLOAT3 targetpos, float myR, float tR);
	
	unsigned int animno;

	EnemyAround E_around;

	std::vector<COBB*> g_enemyobblist;
public:

	Enemy() = default;
	~Enemy() = default;
	
	ENEMYACTIVE EnemyActive;
		
	bool Init(CModel* cmodel);
	void InitEnemyStatus(XMFLOAT3 enemypos, int HP, int Attack, int EXP/*, CModel model*/);
	void UpData();
	void UpDataBoss();
	void Render(bool ObbOn, bool LineOn);	
	void RenderMiniMap();
	void RenderMiniMapBoss();
	void SetMiniMapEnemySize(bool minimapBig);
	void SetMiniMapBossSize(bool minimapBig);
	void InitBossStatus(XMFLOAT3 enemypos, int HP, int Attack, int EXP);
	void ImGuiRender();
	void EnemyExit();


	const std::vector<COBB*>& GetEnemyObblist()const
	{
		return g_enemyobblist;
	}

	const XMFLOAT3& GetEnemyPos() const
	{
		return g_EnemyPos;
	}

	const XMFLOAT4X4& GetEnemyMat()const {
		return g_mtxenemy;
	}

	const float GetEnemyHP() const 
	{
		return EnemyStatus.HP;
	}


	const int& GetEnemyAttack() const
	{
		return EnemyStatus.Attack;
	}

	const int& GetEXP()const
	{
		return EnemyStatus.EXP;
	}

	void DamageToEenmy(int attack)
	{
		EnemyStatus.HP -= attack;
	}

	const XMFLOAT3& GetHomepos()const
	{
		return EnemyHomePos;
	}

	float GetLenToTargetNum() {
		return LenToTarget;
	}

};
