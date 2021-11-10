#ifndef KeyMgr_h__
#define KeyMgr_h__

#define KEY_LBUTTON 0x0000000000000001
#define KEY_RBUTTON 0x0000000000000002
#define KEY_W		0x0000000000000004
#define KEY_A		0x0000000000000008
#define KEY_S		0x0000000000000010
#define KEY_D		0x0000000000000020
#define KEY_SPACE	0x0000000000000040
#define KEY_Q		0x0000000000000080
#define KEY_E		0x0000000000000100
#define KEY_ADD		0x0000000000000200
#define KEY_SUBJECT	0x0000000000000400
#define KEY_NUM8	0x0000000000000800
#define KEY_NUM2	0x0000000000001000
#define KEY_1		0x0000000000002000
#define KEY_2		0x0000000000004000
#define KEY_3		0x0000000000008000
#define KEY_4		0x0000000000010000
#define KEY_5		0x0000000000020000
#define KEY_6		0x0000000000040000
#define KEY_7		0x0000000000080000
#define KEY_8		0x0000000000100000
#define KEY_9		0x0000000000200000
#define KEY_0		0x0000000000400000
#define KEY_I		0x0000000000800000
#define KEY_M		0x0000000001000000
#define KEY_MINUS	0x0000000002000000
#define KEY_PLUS	0x0000000004000000
#define KEY_J		0x0000000008000000
#define KEY_K		0x0000000010000000
#define KEY_L		0x0000000020000000
#define KEY_H		0x0000000040000000
#define KEY_TAB		0x0000000080000000
#define KEY_F		0x0000000100000000
#define KEY_G		0x0000000200000000
#define KEY_P		0x0000000400000000
#define KEY_LEFT	0x0000000800000000
#define KEY_RIGHT	0x0000001000000000
#define KEY_NUM9	0x0000002000000000
#define KEY_NUM6	0x0000004000000000
#define KEY_V		0x0000008000000000
#define KEY_O		0x0000010000000000
#define KEY_NUM0	0x0000020000000000
#define KEY_NUM5	0x0000040000000000
#define KEY_NUM7	0x0000080000000000
#define KEY_NUM4	0x0000100000000000
#define KEY_NUM3	0x0000200000000000
#define KEY_NUM1	0x0000400000000000
#define KEY_F1		0x0000800000000000
#define KEY_F2		0x0001000000000000
#define KEY_MBUTTON	0x0002000000000000
#define KEY_SHIFT	0x0004000000000000
#define KEY_X		0x0008000000000000

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

public:
	explicit CKeyMgr(void);
	virtual ~CKeyMgr(void);

public:
	void Key_Update();
	bool Key_Up(DWORD64 dwKey);
	bool Key_Down(DWORD64 dwKey);
	bool Key_Pressing(DWORD64 dwKey);

private:
	DWORD64 m_dwKey;
	DWORD64 m_dwKeyDown;
	DWORD64 m_dwKeyUp;

public:
	virtual void	Free(void);

};
END
#endif // KeyMgr_h__
