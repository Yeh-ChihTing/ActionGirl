//*****************************************************************************
//!	@file	StageHit.cpp
//!	@brief	
//!	@note	ステージとの当たり判定
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<DirectXMath.h>
#include	"StageHit.h"
#include	"collision.h"
#include	"DatFileLoader.h"
#include    "CModel.h"


//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
PLANEINFO*		g_PlaneInfo;// = nullptr;
std::vector<int>g_NumFace;
std::vector<PLANEINFO*> PlaneInfo;
int MeshNum;

UPorDown UPDOWN;

//==============================================================================
//!	@fn		StageHitInit
//!	@brief　地形との当たり判定初期化処理
//!	@param	メッシュ
//!	@param	デバイスオブジェクト
//!	@retval	なし
//==============================================================================
void StageHitInit(CModel*	cmodel){
	CreateStagePlaneInfoAll(cmodel);	
}

//==============================================================================
//!	@fn		StageHitExit
//!	@brief　地形との当たり判定終了処理
//!	@retval	なし
//==============================================================================
void StageHitExit(){
	if(g_PlaneInfo != nullptr){
		delete[] g_PlaneInfo;
	}	
}

//==============================================================================
//!	@fn		CreateStagePlaneInfoAll
//!	@brief　指定されたメッシュのポリゴンについて当たり判定を行うための平面方程式情報を生成する
//!	@param	メッシュ
//!	@param	デバイスオブジェクト
//!	@retval	なし
//==============================================================================
void CreateStagePlaneInfoAll(CModel*	cmodel)
{
	PlaneInfo.resize(cmodel->GetModelData().GetMeshes().size());
	g_NumFace.resize(cmodel->GetModelData().GetMeshes().size());
	MeshNum = cmodel->GetModelData().GetMeshes().size();
	for (int NowMeshNum = 0; NowMeshNum < cmodel->GetModelData().GetMeshes().size();NowMeshNum++) 
	{
		// 頂点情報格納用
		std::vector<Vertex>	pVertices = cmodel->GetModelData().GetMeshes()[NowMeshNum].m_vertices;

		// インデックス番号
		unsigned int*	pIndices = (unsigned int*)cmodel->GetModelData().GetMeshes()[NowMeshNum].m_indices.data();

		int	 idx0, idx1, idx2;				// ３角形ポリゴンの頂点インデックス格納用


		//pIndices = (unsigned int*)cmodel->GetModelData().GetMeshes()[0].m_indices.data();

		//pVertices = cmodel->GetModelData().GetMeshes()[0].m_vertices;

		// ＸＹＺの座標だけが含まれるメッシュクローンを作成

		//g_NumFaces = cmodel->GetModelData().GetMeshes()[NowMeshNum].m_indices.size() / 3;		// ３角形数取得
		
		g_NumFace[NowMeshNum] = cmodel->GetModelData().GetMeshes()[NowMeshNum].m_indices.size() / 3;
		//g_PlaneInfo = new  PLANEINFO[g_NumFaces];		// 面数分確保

		PlaneInfo[NowMeshNum] = new PLANEINFO[g_NumFace[NowMeshNum]];

		// ポリゴン数分ループ
		for (int i = 0; i < g_NumFace[NowMeshNum]; i++) {

			idx0 = *pIndices++;							// ３角形ポリゴンの頂点インデックス取得（０番目）
			idx1 = *pIndices++;							// ３角形ポリゴンの頂点インデックス取得（１番目）
			idx2 = *pIndices++;							// ３角形ポリゴンの頂点インデックス取得（２番目）


			//g_PlaneInfo[i].p0.x = pVertices[0 + idx0].m_Pos.x;
			//g_PlaneInfo[i].p0.y = pVertices[0 + idx0].m_Pos.y;
			//g_PlaneInfo[i].p0.z = pVertices[0 + idx0].m_Pos.z;
			//		  
			//g_PlaneInfo[i].p1.x = pVertices[0 + idx1].m_Pos.x;
			//g_PlaneInfo[i].p1.y = pVertices[0 + idx1].m_Pos.y;
			//g_PlaneInfo[i].p1.z = pVertices[0 + idx1].m_Pos.z;
			//		   
			//g_PlaneInfo[i].p2.x = pVertices[0 + idx2].m_Pos.x;
			//g_PlaneInfo[i].p2.y = pVertices[0 + idx2].m_Pos.y;
			//g_PlaneInfo[i].p2.z = pVertices[0 + idx2].m_Pos.z;

			////平面の方程式を作成する
			//CreatePlaneInfo(g_PlaneInfo[i].p0, g_PlaneInfo[i].p1, g_PlaneInfo[i].p2,
			//				g_PlaneInfo[i].plane);

			PlaneInfo[NowMeshNum][i].p0.x = pVertices[0 + idx0].m_Pos.x;
			PlaneInfo[NowMeshNum][i].p0.y = pVertices[0 + idx0].m_Pos.y;
			PlaneInfo[NowMeshNum][i].p0.z = pVertices[0 + idx0].m_Pos.z;

			PlaneInfo[NowMeshNum][i].p1.x = pVertices[0 + idx1].m_Pos.x;
			PlaneInfo[NowMeshNum][i].p1.y = pVertices[0 + idx1].m_Pos.y;
			PlaneInfo[NowMeshNum][i].p1.z = pVertices[0 + idx1].m_Pos.z;

			PlaneInfo[NowMeshNum][i].p2.x = pVertices[0 + idx2].m_Pos.x;
			PlaneInfo[NowMeshNum][i].p2.y = pVertices[0 + idx2].m_Pos.y;
			PlaneInfo[NowMeshNum][i].p2.z = pVertices[0 + idx2].m_Pos.z;

			//平面の方程式を作成する
			CreatePlaneInfo(PlaneInfo[NowMeshNum][i].p0, PlaneInfo[NowMeshNum][i].p1, PlaneInfo[NowMeshNum][i].p2,
				PlaneInfo[NowMeshNum][i].plane);

			//PlaneInfo[NowMeshNum].plane = g_PlaneInfo[i].plane;
		}

	}

}

//==============================================================================
//!	@fn		SearchAllSurface
//!	@brief　指定されたメッシュすべての３角形ポリゴンについての交点を求める
//!	@param	Ｘ座標
//!	@param	Ｚ座標
//!	@param	交点
//!	@retval	true 交点あり false 交点なし
//==============================================================================
bool SearchAllSurface(XMFLOAT3 pos, XMFLOAT3& ans){

	XMFLOAT3		DownVector(0, 1, 0);				// 直線の方向ベクトル
	XMFLOAT3		CrossPoint;							// 交点
	XMFLOAT3		CurrentPosition(pos.x, pos.y, pos.z);// 現在位置

	bool FindFlag[3] = { false,false,false };								// ポリゴンが見つかったかどうか
	bool status;										
	//bool cango;

	// 全ポリゴン数分ループ
	for (int NowMeshNum = 0; NowMeshNum < MeshNum; NowMeshNum++)
	{

		for (int i = 0; i < g_NumFace[NowMeshNum]; i++)
		{

			 //平面と直線の交点を求める
			status = LinetoPlaneCross(PlaneInfo[NowMeshNum][i].plane,	//平面の方程式
				CurrentPosition,							//直線の起点
				DownVector,									//直線の方向ベクトル
				CrossPoint);								//交点座標

			if (status) {
				// 交点が３角形の内部にあるか？
				if (CheckInTriangle(PlaneInfo[NowMeshNum][i].p0, PlaneInfo[NowMeshNum][i].p1, PlaneInfo[NowMeshNum][i].p2, CrossPoint))
				{
					if (CurrentPosition.y + 3.5f < CrossPoint.y) { /*cango = false;*/ return false; break; }
					if (CurrentPosition.y - 3.5f > CrossPoint.y) { /*cango = false;*/ return false; break; }
					if (CrossPoint.y - CurrentPosition.y > 0) { UPDOWN = GOUP; }
					if (CrossPoint.y - CurrentPosition.y < 0) { UPDOWN = GODOWN; }
					if (CrossPoint.y - CurrentPosition.y == 0) { UPDOWN = FLAT; }
					CurrentPosition.y = CrossPoint.y;
					FindFlag[0] = true;
					//cango = true;
					break;
				}

			}
		}
	}

		if (FindFlag[0]/*|| FindFlag[1]|| FindFlag[2]*/) 
		{
			ans = CurrentPosition;
			return true;
			
		}

		else/* if(FindFlag[0]==false&&FindFlag[1]==false&&FindFlag[2]==false)*/
		{
			return false;			
		}

}

//******************************************************************************
//	End of file.
//******************************************************************************