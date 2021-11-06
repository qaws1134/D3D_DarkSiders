#include "KeyMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CKeyMgr)

Engine::CKeyMgr::CKeyMgr(void)
{

}

Engine::CKeyMgr::~CKeyMgr(void)
{
	Free();
}

void Engine::CKeyMgr::Key_Update()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	if (GetAsyncKeyState('O') & 0x8000)
		m_dwKey |= KEY_O;
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		m_dwKey |= KEY_NUM8;
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		m_dwKey |= KEY_NUM2;
	if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
		m_dwKey |= KEY_NUM9;
	if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
		m_dwKey |= KEY_NUM6;
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		m_dwKey |= KEY_NUM1;
	if (GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;
	if (GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3;
	if (GetAsyncKeyState('4') & 0x8000)
		m_dwKey |= KEY_4;
	if (GetAsyncKeyState('5') & 0x8000)
		m_dwKey |= KEY_5;
	if (GetAsyncKeyState('6') & 0x8000)
		m_dwKey |= KEY_6;
	if (GetAsyncKeyState('7') & 0x8000)
		m_dwKey |= KEY_7;
	if (GetAsyncKeyState('8') & 0x8000)
		m_dwKey |= KEY_8;
	if (GetAsyncKeyState('9') & 0x8000)
		m_dwKey |= KEY_9;
	if (GetAsyncKeyState('0') & 0x8000)
		m_dwKey |= KEY_0;
	if (GetAsyncKeyState('V') & 0x8000)
		m_dwKey |= KEY_V;
	if (GetAsyncKeyState(VK_ADD) & 0x8000)
		m_dwKey |= KEY_ADD;
	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
		m_dwKey |= KEY_SUBJECT;
	if (GetAsyncKeyState(VK_OEM_MINUS) & 0x8000)
		m_dwKey |= KEY_MINUS;
	if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000)
		m_dwKey |= KEY_PLUS;
	if (GetAsyncKeyState('I') & 0x8000)
		m_dwKey |= KEY_I;
	if (GetAsyncKeyState('M') & 0x8000)
		m_dwKey |= KEY_M;
	if (GetAsyncKeyState('H') & 0x8000)
		m_dwKey |= KEY_H;
	if (GetAsyncKeyState('J') & 0x8000)
		m_dwKey |= KEY_J;
	if (GetAsyncKeyState('K') & 0x8000)
		m_dwKey |= KEY_K;
	if (GetAsyncKeyState('L') & 0x8000)
		m_dwKey |= KEY_L;
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_dwKey |= KEY_TAB;
	if (GetAsyncKeyState('F') & 0x8000)
		m_dwKey |= KEY_F;
	if (GetAsyncKeyState('G') & 0x8000)
		m_dwKey |= KEY_G;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
		m_dwKey |= KEY_NUM0;
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
		m_dwKey |= KEY_NUM5;
	if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
		m_dwKey |= KEY_NUM7;
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		m_dwKey |= KEY_NUM4;
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
		m_dwKey |= KEY_NUM3;
	if (GetAsyncKeyState(VK_F1) & 0x8000)
		m_dwKey |= KEY_F1;
	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_dwKey |= KEY_F2;

}

bool CKeyMgr::Key_Up(DWORD64 dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

bool CKeyMgr::Key_Down(DWORD64 dwKey)
{
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

bool CKeyMgr::Key_Pressing(DWORD64 dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}



void Engine::CKeyMgr::Free(void)
{

}

