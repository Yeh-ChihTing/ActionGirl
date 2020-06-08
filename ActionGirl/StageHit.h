#pragma once
#include	<DirectXMath.h>
#include    <vector>
#include    <d3d11.h>
#include	<DirectXMath.h>
#include	<assimp\scene.h>
#include	<map>
#include	<assimp\Importer.hpp>
#include	"dx11mathutil.h"
#include	"Mesh.h"
#include	"CDirectxGraphics.h"
#include	"collision.h"
#include    "CModel.h"

using namespace DirectX;

// 地形データの面の方程式情報
struct PLANEINFO{
	PLANE1		plane;				//平面の方程式
	DirectX::XMFLOAT3	p0, p1, p2;		//頂点座標
};

enum UPorDown {
	GOUP,
	GODOWN,
	FLAT,
};

//struct navigation 
//{
//	XMFLOAT3 pos;       //選んだvertex
//	XMFLOAT3 P0, P1, P2;//三角の頂点座標
//};

void StageHitInit(CModel*	cmodel);
void StageHitExit();
void CreateStagePlaneInfoAll(CModel*	comdel);
bool SearchAllSurface(XMFLOAT3 pos, DirectX::XMFLOAT3& ans);
