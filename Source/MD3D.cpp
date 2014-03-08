#include "stdafx.h"
#include "MD3D.h"

MD3D::MD3D()
{
}

MD3D::~MD3D()
{
}

bool MD3D::Initialize( WNDPROC wndProc, UINT nWidth, UINT nHeight, bool bFullscreen )
{
	WNDCLASSEX wnd;
	ZeroMemory( &wnd, sizeof(WNDCLASSEX) );
	
	wnd.cbClsExtra			= NULL;
	wnd.cbSize				= sizeof( WNDCLASSEX );
	wnd.cbWndExtra			= NULL;
	wnd.hbrBackground		= (HBRUSH)(COLOR_WINDOW);
	wnd.hCursor				= LoadCursor( NULL, IDC_ARROW );
	wnd.hIcon				= NULL;
	wnd.hIconSm				= NULL;
	wnd.lpfnWndProc			= wndProc;
	wnd.hInstance			= GetModuleHandle( NULL );
	wnd.lpszClassName		= "MD2Model";
	wnd.lpszMenuName		= NULL;
	wnd.style				= 0;

	if( !RegisterClassEx( &wnd ) )
		return -1;

	m_hWnd = CreateWindow( "MD2Model", "MD2", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL,
		GetModuleHandle(NULL), NULL );

	if( !m_hWnd )
		return -1;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	m_pD3DObject = Direct3DCreate9( D3D_SDK_VERSION );
	if( !m_pD3DObject )
		return -1;

	D3DDISPLAYMODE d3dDisplay;
	m_pD3DObject->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3dDisplay );

	ZeroMemory( &m_d3dParams, sizeof(D3DPRESENT_PARAMETERS) );

	m_d3dParams.BackBufferWidth			= 800;
	m_d3dParams.BackBufferHeight		= 600;
	m_d3dParams.Windowed				= !bFullscreen;
	m_d3dParams.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dParams.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dParams.BackBufferFormat		= d3dDisplay.Format;
	m_d3dParams.BackBufferCount			= 1;
	m_d3dParams.AutoDepthStencilFormat	= D3DFMT_D16;
	m_d3dParams.EnableAutoDepthStencil	= TRUE;

	if( FAILED( m_pD3DObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dParams, &m_pD3DDevice ) ) )
	{
		return -1;
	}

	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
//	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );


	return true;
}

void MD3D::SetupCamera()
{
	D3DXVECTOR3 vecEye( 0.0f, 200.0f, -150.0f );
	D3DXVECTOR3 vecAt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vecUp( 0.0f, 1.0f, 0.0f );

	D3DXMatrixLookAtLH( &m_matView, &vecEye, &vecAt, &vecUp );
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );

	D3DXMatrixPerspectiveFovLH( &m_matProjection, D3DX_PI / 4, 1.0f, 1.0f, 500.0f );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProjection );
}

bool MD3D::SetupVertices( MD2Model* pModel )
{
	if( FAILED( D3DXCreateTextureFromFile( m_pD3DDevice, "guillotine.png", &pModel->m_pTexture ) ) )
		return false;

	m_pD3DDevice->CreateVertexBuffer( pModel->m_Header.nNumTriangles * 3 * 
		sizeof(MD2VertexFormat), 0, MD2VERTEXFVF, D3DPOOL_MANAGED, &pModel->m_pVertexBuffer, NULL );

	VOID* pBuffer;

	if( SUCCEEDED( pModel->m_pVertexBuffer->Lock( 0, 0, &pBuffer, 0 ) ) )
	{
		MD2VertexFormat* pFormat = (MD2VertexFormat*)pBuffer;
		MD2Vertex* pFrame = pModel->m_pFrames[0].pVerts;

		for( int i = 0; i < pModel->m_Header.nNumTriangles; i++ )
		{
			for( int j = 0; j < 3; j++ )
			{
				int nTexIndex = pModel->m_pTriangles[i].tu[j];
				int nIndex = pModel->m_pTriangles[i].nVertex[j];

				pFormat[i * 3 + j].x = pFrame[nIndex].fVertex[0];
				pFormat[i * 3 + j].y = pFrame[nIndex].fVertex[1];
				pFormat[i * 3 + j].z = pFrame[nIndex].fVertex[2];
				pFormat[i * 3 + j].t = pModel->m_pTexCoords[nTexIndex].t / 256.0f;
				pFormat[i * 3 + j].u = pModel->m_pTexCoords[nTexIndex].u / 256.0f;
			}
		}
	}

	pModel->m_pVertexBuffer->Unlock();

	return true;
}

bool MD3D::RenderMD2( MCamera* pCamera, MD2Model* pModel )
{
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, NULL );
	if( SUCCEEDED( m_pD3DDevice->BeginScene() ) )
	{
		m_pD3DDevice->SetTransform( D3DTS_VIEW, pCamera->GetViewMatrixPtr() );
		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, pCamera->GetProjectionMatrixPtr() );
		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

		m_pD3DDevice->SetStreamSource( 0, pModel->m_pVertexBuffer, 0, sizeof(MD2VertexFormat) );
		m_pD3DDevice->SetFVF( MD2VERTEXFVF );
		m_pD3DDevice->SetTexture( 0, pModel->m_pTexture );

		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, pModel->m_Header.nNumTriangles );
	}

	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );

	return true;
}