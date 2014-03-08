#pragma once

class MCamera
{
public:
	MCamera();
	~MCamera();

public:
	void Setup();
	void CreateProjectionMatrix( float fFOV, float fAspect, float fNear, float fFar );

	void Move( D3DXVECTOR3 vecMove );
	void Look( float x, float y );
	void Update();

	void SetPosition( D3DXVECTOR3 vecPosition );
	void SetPosition( float x, float y, float z );
	void SetLookAt( D3DXVECTOR3 vecLookAt );
	void SetLookAt( float x, float y, float z );

	void SetFOV( float fFOV )
	{	CreateProjectionMatrix( fFOV, m_fAspect, m_fNearPlane, m_fFarPlane ); }

	void SetAspect( float fAspect )
	{	CreateProjectionMatrix( m_fFOV, fAspect, m_fNearPlane, m_fFarPlane ); }

	void SetPlanes( float fNear, float fFar )
	{	CreateProjectionMatrix( m_fFOV, m_fAspect, fNear, fFar ); }

	void SetMaxVelocity( float fVelocity )
	{	m_fMaxVelocity = fVelocity; }

	void SetMaxPitch( float fPitch )
	{	m_fMaxPitch = fPitch; }

	D3DXMATRIX		GetViewMatrix()				{ return m_matView; }
	D3DXMATRIX*		GetViewMatrixPtr()			{ return &m_matView; }
	D3DXMATRIX		GetProjectionMatrix()		{ return m_matProjection; }
	D3DXMATRIX*		GetProjectionMatrixPtr()	{ return &m_matProjection; }

	float	GetFOV()			{ return m_fFOV; }
	float	GetAspectRatio()	{ return m_fAspect; }
	float	GetNearPlane()		{ return m_fNearPlane; }
	float	GetFarPlane()		{ return m_fFarPlane; }
	float	GetMaxVelocity()	{ return m_fMaxVelocity; }
	float	GetMaxPitch()		{ return m_fMaxPitch; }

private:
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProjection;

	D3DXVECTOR3		m_vecUp;
	D3DXVECTOR3		m_vecRight;
	D3DXVECTOR3		m_vecLook;
	D3DXVECTOR3		m_vecLookAt;
	D3DXVECTOR3		m_vecPosition;
	D3DXVECTOR3		m_vecVelocity;

	float		m_fFOV;
	float		m_fAspect;
	float		m_fNearPlane;
	float		m_fFarPlane;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fMaxPitch;
	float		m_fMaxVelocity;
	
};