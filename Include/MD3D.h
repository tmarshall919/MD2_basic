#pragma once

#include "MD2.h"
#include "MCamera.h"

class MD3D
{
public:
	MD3D();
	~MD3D();

public:
	bool Initialize( WNDPROC wndProc, UINT nWidth, UINT nHeight, bool bFullscreen );
	void SetupCamera();

	bool SetupVertices( MD2Model* pModel );

	bool RenderMD2( MCamera* pCamera, MD2Model* pModel );

public:
	HWND	GetHWND()		{ return m_hWnd; }

	LPDIRECT3D9				GetD3DObject()		{ return m_pD3DObject; }
	LPDIRECT3DDEVICE9		GetD3DDevice()		{ return m_pD3DDevice; }
	D3DPRESENT_PARAMETERS&	GetD3DParams()		{ return m_d3dParams; }

private:
	HWND		m_hWnd;

	LPDIRECT3D9				m_pD3DObject;
	LPDIRECT3DDEVICE9		m_pD3DDevice;
	D3DPRESENT_PARAMETERS	m_d3dParams;

	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProjection;
};