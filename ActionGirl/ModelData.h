#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assimp\scene.h>
#include <map>
#include <assimp\Importer.hpp>
#include "dx11mathutil.h"
#include "Mesh.h"
#include "CDirectxGraphics.h"
#include "animationdataassimp.h"

using namespace DirectX;

class ModelData
{
public:
	ModelData();
	~ModelData();

	bool Load(std::string resourcefolder, std::string filename);
	
	void Update(
		unsigned int animno,
		unsigned int animfileno,
		std::vector<AnimationDataAssimp*>& animationcontainer,
		const int INTERPOLATENUM);
	void setframe();


	void Draw(ID3D11DeviceContext* devcon, XMFLOAT4X4& mtxworld , bool LineOn);

	void DrawOBB();

	void Exit();

	const std::vector<Mesh>& GetMeshes()const {
		return m_meshes;
	}

	void UpdateOBB(const XMFLOAT4X4& mtxworld);

private:

	int m_Frame=0;								// フレーム番号
	AssimpScene m_assimpscene;					// assimp scene
	
	std::vector<const aiScene*> m_sceneAnimContainer;	// アニメーションコンテナ

	int m_cnt = 0;								// キーフレーム補間用
	int m_preFrame=0;							// キーフレーム補間用
	float m_factor = 0.0f;						// キーフレーム補間用

	std::map<std::string, BONE> m_Bone;			//ボーンデータ（ノードの名前で参照）
    std::vector<Mesh> m_meshes;					// メッシュの集合がモデル

	std::string m_directory;					// テクスチャファイルの位置
	ID3D11Buffer* m_constantbufferbonematrix;	// ボーン行列格納用定数バッファ	

	std::vector<Texture> m_texturesloaded;		// 既にロードされているテクスチャ

	void processNode(aiNode* node, const aiScene* scene);				// ノードを解析
	Mesh processMesh(aiMesh* mesh, const aiScene* scene,int meshidx);	// メッシュを解析
																																				// マテリアルに対応したテクスチャを取得する
	std::vector<Texture> loadMaterialTextures(
		aiMaterial * mat, 
		aiTextureType type, 
		std::string typeName, 
		const aiScene * scene);

	void CreateBone(aiNode* node);			// ボーン生成
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);	// ボーン行列更新
	void UpdateBoneMatrixConstantBuffer();	// ボーン行列を定数バッファに反映させる

	//std::vector<Mesh>GetMesh() {};
};
