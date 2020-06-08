#include	<directxmath.h>
#include   "dx11mathutil.h"
#include	"Memory.h"

using namespace DirectX;

class CCamera{
	/*CCamera() {
	}*/
public:
	CCamera()=default;
	CCamera(const CCamera&) = delete;
	CCamera& operator=(const CCamera&) = delete;
	CCamera(CCamera&&) = delete;
	CCamera& operator=(CCamera&&) = delete;

	static CCamera* GetInstance(){
		static CCamera instance;
		return &instance;
	}

	void Init(float nearclip, float farclip, float fov, 
		float width, float height,
		XMFLOAT3 eye, XMFLOAT3 lookat, XMFLOAT3 up){

		SetProjection(nearclip,farclip,fov,width,height);
		SetCamera(eye,lookat,up);
	}

	void SetNear(float nearclip){
		m_near = nearclip;
	}

	void SetFar(float farclip){
		m_far = farclip;
	}

	void SetFov(float fov){
		m_Fov = fov;
	}

	void SetAspect(float width,float height){
		m_Aspect = width/height;
	}

	void SetProjection(float nearclip,float farclip,float fov,float width,float height){
		SetNear(nearclip);
		SetFar(farclip);
		SetFov(fov);
		SetAspect(width,height);
		CreateProjectionMatrix();
	}
		
	void SetCamera(const XMFLOAT3& eye, const XMFLOAT3& lookat, const XMFLOAT3& up){

		SetEye(eye);
		SetLookat(lookat);
		SetUp(up);
		CreateCameraMatrix();
	}

	void SetEye(const XMFLOAT3& eye){
		m_Eye = eye;
	}

	void SetLookat(const XMFLOAT3& lookat){
		m_Lookat = lookat;
	}

	void SetUp(const XMFLOAT3& up){
		m_Up = up;
	}

	void CreateCameraMatrix(){
		ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x, m_Eye.y, m_Eye.z, 0.0f);
		ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x, m_Lookat.y, m_Lookat.z, 0.0f);
		ALIGN16 XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

		ALIGN16 XMMATRIX camera;
		camera = XMMatrixLookAtLH(Eye, At, Up);

		XMStoreFloat4x4(&m_Camera, camera);
	}

	void CreateProjectionMatrix(){

		ALIGN16 XMMATRIX projection;

		projection = XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

		XMStoreFloat4x4(&m_Projection, projection);

	}

	const XMFLOAT4X4& GetCameraMatrix(){
		return m_Camera;
	}

	const XMFLOAT4X4& GetProjectionMatrix(){
		return m_Projection;
	}

	float GetFov() const {
		return m_Fov;
	}

	const XMFLOAT3& GetEye() const {
		return m_Eye;
	}

	const XMFLOAT3& GetLookat() const {
		return m_Lookat;
	}

	const XMFLOAT3& GetUp() const {
		return m_Up;
	}

	void Lookthat(const XMFLOAT4X4 mat, float f,float h)
	{
		//man
	/*	m_Up.x = mat._31;
		m_Up.y = mat._32;
		m_Up.z = mat._33;

		m_Eye.x = mat._41 - mat._21 *  f;
		m_Eye.y = (mat._42+250.0f) - mat._22 *  f;
		m_Eye.z = mat._43 - mat._23 *  f;*/

		//
		m_Up.x = mat._21;
		m_Up.y = mat._22;
		m_Up.z = mat._23;

	
		m_Eye.x = mat._41 - mat._31 *  -f;
		//m_Eye.y = (mat._42 + 25.0f) - mat._32 *  f;
		//m_Eye.y = mat._42+3.0f;// (mat._42 + 5.0f) - mat._32 *  f;
		//m_Eye.y = mat._42 + 2.5f;
		m_Eye.y = mat._42 + h;
		m_Eye.z = mat._43 - mat._33 *  -f;

		//
		m_Lookat.x = mat._41;
		m_Lookat.y = mat._42;
		m_Lookat.z = mat._43;

		SetCamera(m_Eye, m_Lookat, m_Up);

	}

	void Lookthatdown(const XMFLOAT4X4 mat)
	{
		m_Eye.x = mat._41;
		m_Eye.y = 40.0f;
		m_Eye.z = mat._43;

		m_Lookat.x = mat._41;
		m_Lookat.y = mat._42;
		m_Lookat.z = mat._43;

		m_Up.x = 0.0f;
		m_Up.y = 0.0f;
		m_Up.z = 1.0f;

		SetCamera(m_Eye, m_Lookat, m_Up);
	}

	void CMiniMap(const XMFLOAT4X4 mat) {
		
		m_Eye.x = 0.1f;
		m_Eye.y = 200.0f;
		m_Eye.z = 7.0f;

		m_Lookat.x = 0.1f;
		m_Lookat.y = 0;
		m_Lookat.z = 7.0f;

		m_Up.x = 0.0f;
		m_Up.y = 0.0f;
		m_Up.z = 1.0f;

		SetCamera(m_Eye, m_Lookat, m_Up);
	}

	void TitleCamera() {
		m_Eye = { 0.0f,0.0f,-10.0f };
		m_Lookat = { 0.0f,0.0f,0.0f };
		m_Up = { 0.0f,1.0f,0.0f };

		SetCamera(m_Eye, m_Lookat, m_Up);
	}

	void OriginCamera() 
	{
		m_Eye = { 0.0f,0.0f,-10.0f };
		m_Lookat = { 0.0f,0.0f,0.0f };
		m_Up = { 0.0f,1.0f,0.0f };
	}

    

	

private:
	XMFLOAT4X4		m_Projection;

	XMFLOAT4X4		m_Camera;

	XMFLOAT3		m_Eye;				// カメラ位置
	XMFLOAT3		m_Lookat;			// 注視点
	XMFLOAT3		m_Up;				// 上向きベクトル

	float			m_near;
	float			m_Aspect;
	float			m_Fov;
	float			m_far;

};