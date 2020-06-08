#pragma once
#include	<DirectXMath.h>

// ���ʂ̕�����
struct	PLANE1{
	float	a,b,c,d;			// ���ʕ������̌W���i ax+by+cz+d=0 )
};

// �v���g�^�C�v�錾
void CreatePlaneInfo(const DirectX::XMFLOAT3& p1,const DirectX::XMFLOAT3& p2,const DirectX::XMFLOAT3& p3 ,PLANE1& plane);

bool LinetoPlaneCross(	const PLANE1& plane,		//���ʂ̕�����
						const DirectX::XMFLOAT3& line_o,	//�������ʂ�_
						const DirectX::XMFLOAT3& line_v,	//�����̕����x�N�g��
							  DirectX::XMFLOAT3& ans);		//��_���W

bool CheckInTriangle(	const DirectX::XMFLOAT3& a,
						const DirectX::XMFLOAT3& b,
						const DirectX::XMFLOAT3& c,
						const DirectX::XMFLOAT3& p);

float LengthPointToPlane(const DirectX::XMFLOAT3& p, const PLANE1& plane, DirectX::XMFLOAT3& crosspoint);
