//*****************************************************************************
//!	@file	StageHit.cpp
//!	@brief	
//!	@note	�X�e�[�W�Ƃ̓����蔻��
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
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
PLANEINFO*		g_PlaneInfo;// = nullptr;
std::vector<int>g_NumFace;
std::vector<PLANEINFO*> PlaneInfo;
int MeshNum;

UPorDown UPDOWN;

//==============================================================================
//!	@fn		StageHitInit
//!	@brief�@�n�`�Ƃ̓����蔻�菉��������
//!	@param	���b�V��
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
//==============================================================================
void StageHitInit(CModel*	cmodel){
	CreateStagePlaneInfoAll(cmodel);	
}

//==============================================================================
//!	@fn		StageHitExit
//!	@brief�@�n�`�Ƃ̓����蔻��I������
//!	@retval	�Ȃ�
//==============================================================================
void StageHitExit(){
	if(g_PlaneInfo != nullptr){
		delete[] g_PlaneInfo;
	}	
}

//==============================================================================
//!	@fn		CreateStagePlaneInfoAll
//!	@brief�@�w�肳�ꂽ���b�V���̃|���S���ɂ��ē����蔻����s�����߂̕��ʕ��������𐶐�����
//!	@param	���b�V��
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
//==============================================================================
void CreateStagePlaneInfoAll(CModel*	cmodel)
{
	PlaneInfo.resize(cmodel->GetModelData().GetMeshes().size());
	g_NumFace.resize(cmodel->GetModelData().GetMeshes().size());
	MeshNum = cmodel->GetModelData().GetMeshes().size();
	for (int NowMeshNum = 0; NowMeshNum < cmodel->GetModelData().GetMeshes().size();NowMeshNum++) 
	{
		// ���_���i�[�p
		std::vector<Vertex>	pVertices = cmodel->GetModelData().GetMeshes()[NowMeshNum].m_vertices;

		// �C���f�b�N�X�ԍ�
		unsigned int*	pIndices = (unsigned int*)cmodel->GetModelData().GetMeshes()[NowMeshNum].m_indices.data();

		int	 idx0, idx1, idx2;				// �R�p�`�|���S���̒��_�C���f�b�N�X�i�[�p


		//pIndices = (unsigned int*)cmodel->GetModelData().GetMeshes()[0].m_indices.data();

		//pVertices = cmodel->GetModelData().GetMeshes()[0].m_vertices;

		// �w�x�y�̍��W�������܂܂�郁�b�V���N���[�����쐬

		//g_NumFaces = cmodel->GetModelData().GetMeshes()[NowMeshNum].m_indices.size() / 3;		// �R�p�`���擾
		
		g_NumFace[NowMeshNum] = cmodel->GetModelData().GetMeshes()[NowMeshNum].m_indices.size() / 3;
		//g_PlaneInfo = new  PLANEINFO[g_NumFaces];		// �ʐ����m��

		PlaneInfo[NowMeshNum] = new PLANEINFO[g_NumFace[NowMeshNum]];

		// �|���S���������[�v
		for (int i = 0; i < g_NumFace[NowMeshNum]; i++) {

			idx0 = *pIndices++;							// �R�p�`�|���S���̒��_�C���f�b�N�X�擾�i�O�Ԗځj
			idx1 = *pIndices++;							// �R�p�`�|���S���̒��_�C���f�b�N�X�擾�i�P�Ԗځj
			idx2 = *pIndices++;							// �R�p�`�|���S���̒��_�C���f�b�N�X�擾�i�Q�Ԗځj


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

			////���ʂ̕��������쐬����
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

			//���ʂ̕��������쐬����
			CreatePlaneInfo(PlaneInfo[NowMeshNum][i].p0, PlaneInfo[NowMeshNum][i].p1, PlaneInfo[NowMeshNum][i].p2,
				PlaneInfo[NowMeshNum][i].plane);

			//PlaneInfo[NowMeshNum].plane = g_PlaneInfo[i].plane;
		}

	}

}

//==============================================================================
//!	@fn		SearchAllSurface
//!	@brief�@�w�肳�ꂽ���b�V�����ׂĂ̂R�p�`�|���S���ɂ��Ă̌�_�����߂�
//!	@param	�w���W
//!	@param	�y���W
//!	@param	��_
//!	@retval	true ��_���� false ��_�Ȃ�
//==============================================================================
bool SearchAllSurface(XMFLOAT3 pos, XMFLOAT3& ans){

	XMFLOAT3		DownVector(0, 1, 0);				// �����̕����x�N�g��
	XMFLOAT3		CrossPoint;							// ��_
	XMFLOAT3		CurrentPosition(pos.x, pos.y, pos.z);// ���݈ʒu

	bool FindFlag[3] = { false,false,false };								// �|���S���������������ǂ���
	bool status;										
	//bool cango;

	// �S�|���S���������[�v
	for (int NowMeshNum = 0; NowMeshNum < MeshNum; NowMeshNum++)
	{

		for (int i = 0; i < g_NumFace[NowMeshNum]; i++)
		{

			 //���ʂƒ����̌�_�����߂�
			status = LinetoPlaneCross(PlaneInfo[NowMeshNum][i].plane,	//���ʂ̕�����
				CurrentPosition,							//�����̋N�_
				DownVector,									//�����̕����x�N�g��
				CrossPoint);								//��_���W

			if (status) {
				// ��_���R�p�`�̓����ɂ��邩�H
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