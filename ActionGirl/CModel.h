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
	FILETYPE				m_filetype = eASSIMPFILE;		// ファイルタイプ
	ModelData				m_assimpfile;				   // assimpfile
	std::vector<AnimationDataAssimp*> m_animationcontainer;
	ID3D11VertexShader*     m_pVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// 頂点フォーマット定義

	ID3D11ShaderResourceView*	g_toonsrv = nullptr;	// TOON用シェーダーリソースビュー
	ShaderHashmap           g_shaderhashmap;

	ID3D11RasterizerState*	g_rasterStateFrontCulling = nullptr;
	ID3D11RasterizerState*	g_rasterStateBackCulling = nullptr;
	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// テクスチャＳＲＶ
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
	unsigned int GetAnimationNum() {	// アニメーション総数を取得

		return m_animationcontainer[m_AnimFileIdx]->GetScene()->mNumAnimations;
	}

	void ChangeAnimFileIdx() {
		m_AnimFileIdx++;
		if (m_AnimFileIdx >= m_animationcontainer.size()) {
			m_AnimFileIdx = 0;
		}
		
	}

	// アニメーションをロードする
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

		// メッシュデータ取得
		const std::vector<Mesh>& meshes = m_assimpfile.GetMeshes();
		
		// OBB全件取得
		for (int i = 0; i < meshes.size(); i++) {
			const Mesh& m = meshes[i];
			for (auto data : m.m_obbvectorcontainer) {
				obblist.push_back(data);
			}
		}
	}
};
