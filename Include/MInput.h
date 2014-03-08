#pragma once

class MInput
{
public:
	MInput();
	~MInput();

public:
	bool Initialize( HWND hWnd );
	void Destroy();
	void Read();

	bool IsKeyDown( UINT nKey );

	bool IsMouseLeftButtonDown();
	bool IsMouseMiddleButtonDown();
	bool IsMouseRightButtonDown();
	float GetMousePositionX();
	float GetMousePositionY();
	UINT GetMouseScrollWheelValue();

private:
	HWND	m_hWindowHandle;

	LPDIRECTINPUT8			m_pKeyboardObject;
	LPDIRECTINPUT8			m_pMouseObject;
	LPDIRECTINPUTDEVICE8	m_pKeyboardDevice;
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;

	char			m_szKeysDown[256];
	DIMOUSESTATE	m_mouseState;

};