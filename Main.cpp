#include "stdafx.h"
#include "Main.h"
#include "MD2.h"
#include "MD3D.h"
#include "MCamera.h"
#include "MInput.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

MD3D*		g_pD3D;
MCamera*	g_pCamera;
MD2Model*	g_pModel;
MInput*		g_pInput;

LRESULT CALLBACK WndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	switch( nMsg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		default:
			break;
	}

	return DefWindowProc( hWnd, nMsg, wParam, lParam );
}


int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	g_pModel = new MD2Model();
	if( !g_pModel->Load( "guillotine.md2" ) )
		return -1;

	g_pD3D = new MD3D();
	if( !g_pD3D->Initialize( WndProc, 800, 600, false ) )
		return -1;

	g_pCamera = new MCamera();
	g_pCamera->Setup();
	g_pCamera->CreateProjectionMatrix( D3DX_PI / 4, 800 / 600, 0.1f, 1000.0f );
	g_pCamera->SetPosition( 0.0f, 0.0f, -150.0f );
	g_pCamera->Update();

	if( !g_pD3D->SetupVertices( g_pModel ) )
		return -1;
	g_pInput = new MInput();
	if( !g_pInput->Initialize( g_pD3D->GetHWND() ) )
		return -1;

	MSG msg;
	while( true )
	{
		if( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			g_pInput->Read();
			D3DXVECTOR3 vecMove = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			if( g_pInput->IsKeyDown( DIK_W ) )
				vecMove.z += 0.5f;
			if( g_pInput->IsKeyDown( DIK_S ) )
				vecMove.z -= 0.5f;
			if( g_pInput->IsKeyDown( DIK_A ) )
				vecMove.x -= 0.5f;
			if( g_pInput->IsKeyDown( DIK_D ) )
				vecMove.x += 0.5f;
			
			g_pCamera->Move( vecMove );

			float x = g_pInput->GetMousePositionX() * 0.03f;
			float y = g_pInput->GetMousePositionY() * 0.03f;
			if( x != 0.0f || y != 0.0f )
				g_pCamera->Look( x, y );

			g_pCamera->Update();
			g_pD3D->RenderMD2( g_pCamera, g_pModel );
		}
	}

	return 0;
}