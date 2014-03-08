#include "stdafx.h"
#include "MCamera.h"

MCamera::MCamera()
{
}

MCamera::~MCamera()
{
}

void MCamera::Setup()
{
	m_fMaxPitch		= D3DXToRadian( 89.0f );
	m_fMaxVelocity	= 1.0f;
	m_vecPosition = m_vecVelocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vecLook = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
}

void MCamera::CreateProjectionMatrix( float fFOV, float fAspect, float fNear, float fFar )
{
	m_fFOV			= fFOV;
	m_fAspect		= fAspect;
	m_fNearPlane	= fNear;
	m_fFarPlane		= fFar;

	D3DXMatrixPerspectiveFovLH( &m_matProjection, m_fFOV, m_fAspect,
		m_fNearPlane, m_fFarPlane );
}

void MCamera::Move( D3DXVECTOR3 vecMove )
{
	if( vecMove.x != 0.0f )
		m_vecVelocity += m_vecRight * vecMove.x;
	if( vecMove.z != 0.0f )
	{
		m_vecVelocity += m_vecLook * vecMove.z;
	}
}

void MCamera::Look( float x, float y )
{
	if( x != 0.0f )
	{
		D3DXMATRIX matRot;

		D3DXMatrixRotationAxis( &matRot, &m_vecUp, x );
		D3DXVec3TransformNormal( &m_vecRight, &m_vecRight, &matRot );
		D3DXVec3TransformNormal( &m_vecLook, &m_vecLook, &matRot );
	}

	if( y != 0.0f )
	{
		m_fPitch -= y;

		if( m_fPitch > m_fMaxPitch )
			y += m_fPitch - m_fMaxPitch;
		else if( m_fPitch < -m_fMaxPitch )
			y += m_fPitch + m_fMaxPitch;

		D3DXMATRIX matRot;

		D3DXMatrixRotationAxis( &matRot, &m_vecRight, y );
		D3DXVec3TransformNormal( &m_vecUp, &m_vecUp, &matRot );
		D3DXVec3TransformNormal( &m_vecLook, &m_vecLook, &matRot );
	}
}

void MCamera::Update()
{
	if( D3DXVec3Length( &m_vecVelocity ) > m_fMaxVelocity )
		m_vecVelocity = *(D3DXVec3Normalize(&m_vecVelocity, &m_vecVelocity)) * m_fMaxVelocity;

	m_vecPosition += m_vecVelocity;

	// TODO : Decelerate
	m_vecVelocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vecLookAt = m_vecPosition + m_vecLook;

	D3DXVECTOR3 vecUp( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &m_matView, &m_vecPosition, &m_vecLookAt, &vecUp );

	m_vecRight.x	= m_matView._11; 
	m_vecRight.y	= m_matView._21; 
	m_vecRight.z	= m_matView._31; 
	m_vecUp.x		= m_matView._12;
	m_vecUp.y		= m_matView._22;
	m_vecUp.z		= m_matView._32;
	m_vecLook.x		= m_matView._13;
	m_vecLook.y		= m_matView._23;
	m_vecLook.z		= m_matView._33;

	float fLook = sqrtf( (m_vecLook.z * m_vecLook.z) + (m_vecLook.x * m_vecLook.x) );
	m_fPitch	= atan2f( m_vecLook.y, fLook );
	m_fYaw		= atan2f( m_vecLook.x, m_vecLook.z );
}

void MCamera::SetPosition( D3DXVECTOR3 vecPosition )
{
	m_vecPosition = vecPosition;
}

void MCamera::SetPosition( float x, float y, float z )
{
	m_vecPosition = D3DXVECTOR3( x, y, z );
}

void MCamera::SetLookAt( D3DXVECTOR3 vecLookAt )
{
	m_vecLookAt = vecLookAt;
	D3DXVec3Normalize( &m_vecLook, &(m_vecLookAt - m_vecPosition) );
}

void MCamera::SetLookAt( float x, float y, float z )
{
	m_vecLookAt = D3DXVECTOR3( x, y, z );
	D3DXVec3Normalize( &m_vecLook, &(m_vecLookAt - m_vecPosition) );
}