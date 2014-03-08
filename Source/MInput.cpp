#include "stdafx.h"
#include "MInput.h"

MInput::MInput()
{
}

MInput::~MInput()
{
	Destroy();
}

bool MInput::Initialize( HWND hWnd )
{
	HRESULT hRet = DirectInput8Create( GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pKeyboardObject, NULL );
	if( FAILED( hRet ) )
	{
		//TODO
		return false;
	}

	hRet = m_pKeyboardObject->CreateDevice( GUID_SysKeyboard, &m_pKeyboardDevice, NULL );
	if( FAILED( hRet ) )
	{
		//TODO
		return false;
	}

	m_pKeyboardDevice->SetDataFormat( &c_dfDIKeyboard );
	m_pKeyboardDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	hRet = m_pKeyboardDevice->Acquire();
	if( FAILED( hRet ) ) //TODO : Acquire can fail for fuckton of reasons
	{
		//TODO
	}

	hRet = DirectInput8Create( GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pMouseObject, NULL );
	if( FAILED( hRet ) )
	{
		//TODO
		return false;
	}

	hRet = m_pMouseObject->CreateDevice( GUID_SysMouse, &m_pMouseDevice, NULL );
	if( FAILED( hRet ) )
	{
		//TODO
		return false;
	}

	m_pMouseDevice->SetDataFormat( &c_dfDIMouse );
	m_pMouseDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	hRet = m_pMouseDevice->Acquire();
	if( FAILED( hRet ) ) //TODO : Acquire can fail for fuckton of reasons
	{
		//TODO
	}

	return true;
}

void MInput::Destroy()
{
	if( m_pKeyboardDevice != NULL )
	{
		m_pKeyboardDevice->Release();
		m_pKeyboardDevice = NULL;
	}
	if( m_pMouseDevice != NULL )
	{
		m_pMouseDevice->Release();
		m_pMouseDevice = NULL;
	}
	if( m_pKeyboardObject != NULL )
	{
		m_pKeyboardObject->Release();
		m_pKeyboardObject = NULL;
	}
	if( m_pMouseObject != NULL )
	{
		m_pMouseObject->Release();
		m_pMouseObject = NULL;
	}
}

void MInput::Read()
{
	if( FAILED( m_pKeyboardDevice->GetDeviceState( sizeof(m_szKeysDown), (LPVOID)m_szKeysDown ) ) )
		m_pKeyboardDevice->Acquire();

	if( FAILED( m_pMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState ) ) )
		m_pMouseDevice->Acquire();
}

bool MInput::IsKeyDown( UINT nKey )
{
	if( m_szKeysDown[nKey] & 0x80 )
		return true;

	return false;
}

bool MInput::IsMouseLeftButtonDown()
{
	return (m_mouseState.rgbButtons[0] > 0);
}
bool MInput::IsMouseMiddleButtonDown()
{
	return (m_mouseState.rgbButtons[1] > 0);
}
bool MInput::IsMouseRightButtonDown()
{
	return (m_mouseState.rgbButtons[2] > 0);
}
float MInput::GetMousePositionX()
{
	return (float)m_mouseState.lX;
}
float MInput::GetMousePositionY()
{
	return (float)m_mouseState.lY;
}
UINT MInput::GetMouseScrollWheelValue()
{
	return m_mouseState.lZ;
}