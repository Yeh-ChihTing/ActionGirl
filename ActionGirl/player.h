#pragma once
#include "object.h"
#include "CBillBoard.h"

enum PlayerMoveMode {
	StandP,
	MoveToFront,
	MoveBack,
	MoveLeft,
	MoveRight,
	MoveFL,
	MoveFR,
	MoveBL,
	MoveBR,
	Attack,
	DieP,
	clear,
};

class Player 
{
private:
	Player() = default;
	~Player() = default;
	
	CBillBoard      g_pMiniMapPos;

	float movement;
	//bool attack = false;
	bool lockinput = false;
	bool Nowmove;
	bool dead = false;
	bool Attacknow = false;
	bool Attackpartime = false;
	bool Damageto;
	int TargetNum;
	bool HitEnemy;
	bool ReStart = false;
	bool gobackTitle;
	bool HitBoss = false;
	struct PLAYERSTATUS
	{
		int Level;
		float HP;
		float MAXHP;
		int Attack;
		int Defense;
		float EXP;
		float MAXEXP;


	}PStatus;

	CModel			g_model;			// 主人公モデル
	XMFLOAT4X4		g_mtxplayer;		// 主人公の行列
	XMFLOAT4X4		g_mtxplayerDie;		// 主人公の行列
	XMFLOAT3        g_PlayerPos;        //主人公の位置
	XMFLOAT3        g_pFrontPos;
	XMFLOAT3        g_pBackPos;

	bool JumpNow;
	float YUp= 0.0025f;
	XMFLOAT3        pangle;

	std::vector<COBB*> g_playerobblist;
	PlayerMoveMode MoveMode;

	int animespeed = 15;
	float Atcount;

	bool mouseget = false;

	void operating();
	void Move(XMFLOAT3& pos, XMFLOAT4X4& mat, float move);
	bool GetPlayerGround();
	bool GetPlayerFGround();
	bool GetPlayerBGround();
	bool Hit(std::vector<COBB*> a, std::vector<COBB*> b);
	bool HitMonster(std::vector<COBB*> a, std::vector<COBB*> b);
	bool CHit(XMFLOAT3& mypos, XMFLOAT3 targetpos, float myR, float tR);
	void LevelUP();
	void HitByEnemy();
	void AttackMath();
	void MoveF();
	void Movemode();
	
	/*void MoveB();*/
	float ChageSpeed();
	XMFLOAT3 neerpoint;
	XMFLOAT3 goal = { 700.0f,0.0f,100.0f };
	XMFLOAT3 neergoal;
	XMFLOAT4X4 Triangle;
	XMFLOAT3 TrianglePos;

	XMFLOAT3 angle;
	XMFLOAT3 angleCamera;

	void GetplayerFPos()
	{
		g_pFrontPos.x = g_mtxplayer._41 - g_mtxplayer._31;
		g_pFrontPos.z = g_mtxplayer._43 - g_mtxplayer._33;
	}

	void GetplayerBPos()
	{
		g_pBackPos.x = g_mtxplayer._41 + g_mtxplayer._31;
		g_pBackPos.z = g_mtxplayer._43 + g_mtxplayer._33;
	}

	void SetPos(const XMFLOAT3& pos)
	{
		g_PlayerPos = pos;
	}

	void SetPosPFront(const XMFLOAT3& pos)
	{
		g_pFrontPos = pos;
	}

	void SetPosPBack(const XMFLOAT3& pos)
	{
		g_pBackPos = pos;
	}


public:

	unsigned int animno; 
	void InitPlayerStatus();


	bool Init();
	void TitleUpdata();
	void Updata();
	void Render(bool Obbon, bool LineOn);	
	void RenderInMiniMap();
	void ImGuiRender();	
	void PlayerExit();

	static Player& GetInstance() {
		static Player instance;
		return instance;
	}

	void SetMiniMapPlayerSize(bool minimapBig);

	const XMFLOAT4X4& GetPlayerMat() const
	{
		/*if (MoveMode == DieP)
		{*/
			return g_mtxplayerDie;
		/*}
		else 
		{
			return g_mtxplayer;
		}*/
	}

	const bool& GetHItEnemy()const 
	{
		return HitEnemy;
	}

	const XMFLOAT3& GetPlayerPos() const
	{
		return g_PlayerPos;
	}

	const std::vector<COBB*>& GetPlayerObblist()const 
	{
		return g_playerobblist;
	}

	const int& GetPlayerLevel()const 
	{
		return PStatus.Level;

	}

	const float& GetPlayerHP()const 
	{
		return PStatus.HP;

	}

	const float& GetPlayerMAXHP()const 
	{
		return PStatus.MAXHP;
	}

	const float& GetPlayerEXP()const
	{
		return PStatus.EXP;

	}

	const float& GetPlayerMAXEXP()const
	{
		return PStatus.MAXEXP;
	}

	const int& GetTargetNum() const
	{
		return TargetNum;
	}
	const bool& GetAttackFlag()const {

		return Damageto;
	}

	const bool& GetPlayerLiveOrDead() const 
	{
		return dead;
	}

	const bool& GetReStart()const 
	{
		return ReStart;
	}

	void SetReStart() 
	{
		ReStart = false;
	}
	float getmovement() {
		return movement;
	}

	const bool& GetHitBoss() const 
	{
		return HitBoss;
	}

	//const float& GetPlayerY() const
	//{
	//	return g_PlayerPos.y;
	//}

};
