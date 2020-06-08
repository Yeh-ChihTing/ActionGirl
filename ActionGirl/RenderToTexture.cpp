#include "RenderToTexture.h"
#include "CDirectxGraphics.h"
#include "object.h"
#include"CCamera.h"
#include "DX11Settransform.h"

using namespace Microsoft::WRL;


HRESULT RenderTexture::InitRTT(/*ID3D11Device* device, */int texWidth, int texHeight, bool generateMips)
{
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();
	// 
	m_pOutputTextureSRV.Reset();
	m_pOutputTextureRTV.Reset();
	m_pOutputTextureDSV.Reset();
	m_pCacheRTV.Reset();
	m_pCacheDSV.Reset();

	m_GenerateMips = generateMips;
	HRESULT hr;
	// 
	// 1. 
	//

	ComPtr<ID3D11Texture2D> texture;
	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = (m_GenerateMips ? 0 : 1);	// 
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// 
	hr = device->CreateTexture2D(&texDesc, nullptr, texture.GetAddressOf());
	if (FAILED(hr))
		return hr;
	// 
	// 2. 
	//

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(texture.Get(), &rtvDesc, m_pOutputTextureRTV.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// 
	// 3. 
	//

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;	// 

	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc,
		m_pOutputTextureSRV.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// 
	// 4. 
	//

	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthTex;
	hr = device->CreateTexture2D(&texDesc, nullptr, depthTex.GetAddressOf());
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthTex.Get(), &dsvDesc,
		m_pOutputTextureDSV.GetAddressOf());
	if (FAILED(hr))
		return hr;

	//
	// 5. 
	//
	m_OutputViewPort.TopLeftX = 0.0f;
	m_OutputViewPort.TopLeftY = 0.0f;
	m_OutputViewPort.Width = static_cast<float>(texWidth);
	m_OutputViewPort.Height = static_cast<float>(texHeight);
	m_OutputViewPort.MinDepth = 0.0f;
	m_OutputViewPort.MaxDepth = 1.0f;

	return S_OK;
}

void RenderTexture::BeginRTT(ID3D11DeviceContext* deviceContext, const FLOAT backgroundColor[4])
{
	
	deviceContext->OMGetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.GetAddressOf());
	// 
	UINT num_Viewports = 1;
	deviceContext->RSGetViewports(&num_Viewports, &m_CacheViewPort);

	// 
	deviceContext->ClearRenderTargetView(m_pOutputTextureRTV.Get(), backgroundColor);
	deviceContext->ClearDepthStencilView(m_pOutputTextureDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// 
	deviceContext->OMSetRenderTargets(1, m_pOutputTextureRTV.GetAddressOf(), m_pOutputTextureDSV.Get());
	// 
	deviceContext->RSSetViewports(1, &m_OutputViewPort);
	
}

void RenderTexture::EndRTT(ID3D11DeviceContext* deviceContext)
{

	deviceContext->RSSetViewports(1, &m_CacheViewPort);
	deviceContext->OMSetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.Get());

	// 
	if (m_GenerateMips)
	{
		deviceContext->GenerateMips(m_pOutputTextureSRV.Get());
	}

	// 
	m_pCacheDSV.Reset();
	m_pCacheRTV.Reset();

}

ID3D11ShaderResourceView* RenderTexture::GetOutputTexture()
{
	return m_pOutputTextureSRV.Get();
}

void RenderTexture::RenderMiniMap(ID3D11DeviceContext* deviceContext,float X,float Y,bool minimapBig) {
	

	if (!minimapBig)
	{
		
		CCamera::GetInstance()->Lookthatdown(Player::GetInstance().GetPlayerMat());
	}
	else 
	{
		CCamera::GetInstance()->CMiniMap(Player::GetInstance().GetPlayerMat());
	}
	XMFLOAT4X4 mtx;
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// プロジェクション変換行列セット
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);
	//minimap

	
	float clear[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	BeginRTT(deviceContext, clear);

	Object::GetInstance()->MiniMapRender();

	EndRTT(deviceContext);
	
}

void RenderTexture::UnInit() {

	if (m_pOutputTextureSRV) {
		m_pOutputTextureSRV->Release();
		m_pOutputTextureSRV = nullptr;
	}
}



