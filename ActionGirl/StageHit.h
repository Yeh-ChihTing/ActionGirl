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

// �n�`�f�[�^�̖ʂ̕��������
struct PLANEINFO{
	PLANE1		plane;				//���ʂ̕�����
	DirectX::XMFLOAT3	p0, p1, p2;		//���_���W
};

enum UPorDown {
	GOUP,
	GODOWN,
	FLAT,
};

//struct navigation 
//{
//	XMFLOAT3 pos;       //�I��vertex
//	XMFLOAT3 P0, P1, P2;//�O�p�̒��_���W
//};

void StageHitInit(CModel*	cmodel);
void StageHitExit();
void CreateStagePlaneInfoAll(CModel*	comdel);
bool SearchAllSurface(XMFLOAT3 pos, DirectX::XMFLOAT3& ans);
