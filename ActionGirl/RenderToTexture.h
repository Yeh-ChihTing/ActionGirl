#pragma once
#include <d3d11_1.h>
#include <wrl/client.h>
#include <string>


class RenderTexture
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	RenderTexture() = default;
	~RenderTexture() = default;
	//
	RenderTexture(const RenderTexture&) = delete;
	RenderTexture& operator=(const RenderTexture&) = delete;
	RenderTexture(RenderTexture&&) = default;
	RenderTexture& operator=(RenderTexture&&) = default;

	static RenderTexture* GetInstance() {
		static RenderTexture instance;
		return &instance;
	}

	HRESULT InitRTT(/*ID3D11Device* device,*/
		int texWidth,
		int texHeight,
		bool generateMips = false);

	// 
	void BeginRTT(ID3D11DeviceContext* deviceContext, const FLOAT backgroundColor[4]);
	// 
	void EndRTT(ID3D11DeviceContext * deviceContext);
	// 
	// 
	ID3D11ShaderResourceView* GetOutputTexture();

	void UnInit();


	void RenderMiniMap(ID3D11DeviceContext* deviceContext, float X, float Y, bool minimapBig);
	//
	//void SetDebugObjectName(const std::string& name);

private:
	
	
	ComPtr<ID3D11ShaderResourceView>	m_pOutputTextureSRV;	// 
	ComPtr<ID3D11RenderTargetView>		m_pOutputTextureRTV;	// 
	ComPtr<ID3D11DepthStencilView>		m_pOutputTextureDSV;	// 
	D3D11_VIEWPORT						m_OutputViewPort = {};	// 

	ID3D11VertexShader*     m_pVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// 頂点フォーマット定義


	ComPtr<ID3D11RenderTargetView>		m_pCacheRTV;		    // 
	ComPtr<ID3D11DepthStencilView>		m_pCacheDSV;		    // 
	D3D11_VIEWPORT						m_CacheViewPort = {};	// 

	bool								m_GenerateMips = false;	// 
};
