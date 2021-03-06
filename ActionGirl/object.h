#pragma once
#include "CModel.h"
#include"player.h"
#include"enemy.h"
#include "StageHit.h"
#define PI (3.1415926)



class Object 
{
private:

	void ImGuiRender();
	bool LineOn=false;
	bool ImGuiOn = false;
	bool OBBOn=false;
	void HitTurnRed(std::vector<COBB*> a, std::vector<COBB*> b);
	
	Object() = default;
	~Object() = default;
	
public:
		
	CModel			g_Skydome;		    // skydomeモデル
	XMFLOAT4X4		g_mtxskydome;	    // skydomeの行列


	CModel          g_ground;
	XMFLOAT4X4      g_mtxground;

	bool Init(bool sts);
	void Updata();
	void Render();
	void MiniMapRender();
	void ObjectExit();
	

	static Object* GetInstance() {
		static Object instance;
		return &instance;
	}


};