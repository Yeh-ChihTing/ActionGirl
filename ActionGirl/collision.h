#pragma once
#include	<DirectXMath.h>

// 平面の方程式
struct	PLANE1{
	float	a,b,c,d;			// 平面方程式の係数（ ax+by+cz+d=0 )
};

// プロトタイプ宣言
void CreatePlaneInfo(const DirectX::XMFLOAT3& p1,const DirectX::XMFLOAT3& p2,const DirectX::XMFLOAT3& p3 ,PLANE1& plane);

bool LinetoPlaneCross(	const PLANE1& plane,		//平面の方程式
						const DirectX::XMFLOAT3& line_o,	//直線が通る点
						const DirectX::XMFLOAT3& line_v,	//直線の方向ベクトル
							  DirectX::XMFLOAT3& ans);		//交点座標

bool CheckInTriangle(	const DirectX::XMFLOAT3& a,
						const DirectX::XMFLOAT3& b,
						const DirectX::XMFLOAT3& c,
						const DirectX::XMFLOAT3& p);

float LengthPointToPlane(const DirectX::XMFLOAT3& p, const PLANE1& plane, DirectX::XMFLOAT3& crosspoint);
