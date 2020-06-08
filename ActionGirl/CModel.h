#pragma once
#include	<d3d11.h>
#include	<vector>
#include	"ModelData.h"
#include	"animationdataassimp.h"
#include    "shaderhashmap.h"
class CModel {
private:
	enum FILETYPE{
		eDATFILE,
		eASSIMPFILE
	};
	FILETYPE				m_filetype = eASSIMPFILE;		// �t�@�C���^�C�v
	ModelData				m_assimpfile;				   // assimpfile
	std::vector<AnimationDataAssimp*> m_animationcontainer;
	ID3D11VertexShader*     m_pVertexShader = nullptr;		// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// ���_�t�H�[�}�b�g��`

	ID3D11ShaderResourceView*	g_toonsrv = nullptr;	// TOON�p�V�F�[�_�[���\�[�X�r���[
	ShaderHashmap           g_shaderhashmap;

	ID3D11RasterizerState*	g_rasterStateFrontCulling = nullptr;
	ID3D11RasterizerState*	g_rasterStateBackCulling = nullptr;
	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// �e�N�X�`���r�q�u
	unsigned int	m_AnimFileIdx = 0;
public:
	
	void setframe();
	bool Init(const char* filename,const char* vsfile,const char* psfile, std::string texfolder);
	bool InitToon(const char* filename, const char* vsfilename[], const char* psfilename[], std::string texfolder);
	void Uninit(bool ani);
	void Update(
		unsigned int animno,
		const XMFLOAT4X4& mtxworld,
		const int INTERPOLATENUMz);
	void UpdateOBB(const XMFLOAT4X4& mtxworld);
	void Draw(XMFLOAT4X4& mtxworld, const char* vsfile, const char* psfile, bool LineOn);
	void DrawToon(XMFLOAT4X4& mtxworld, const char* vsfilename[], const char* psfilename[], bool LineOn);
	unsigned int GetAnimationNum() {	// �A�j���[�V�����������擾

		return m_animationcontainer[m_AnimFileIdx]->GetScene()->mNumAnimations;
	}

	void ChangeAnimFileIdx() {
		m_AnimFileIdx++;
		if (m_AnimFileIdx >= m_animationcontainer.size()) {
			m_AnimFileIdx = 0;
		}
		
	}

	// �A�j���[�V���������[�h����
	bool LoadAnimation(const char* filename) {
		AnimationDataAssimp* animdata;

		animdata = new AnimationDataAssimp();
		bool sts = animdata->Init(filename);
		if (sts) {
			m_animationcontainer.push_back(animdata);
			return true;
		}
		else {
			MessageBox(nullptr, "Load Animation error", "error", MB_OK);
			return false;
		}
	}

	const ModelData& GetModelData() {
		return m_assimpfile;
	}
	ID3D11ShaderResourceView* GetSrv() {
		return m_texSRV;
	}

	void DrawOBB() {
		m_assimpfile.DrawOBB();
	}

	void GetOBBList(std::vector<COBB*>& obblist) {

		// ���b�V���f�[�^�擾
		const std::vector<Mesh>& meshes = m_assimpfile.GetMeshes();
		
		// OBB�S���擾
		for (int i = 0; i < meshes.size(); i++) {
			const Mesh& m = meshes[i];
			for (auto data : m.m_obbvectorcontainer) {
				obblist.push_back(data);
			}
		}
	}
};
