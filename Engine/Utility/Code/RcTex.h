#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }

public:
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

private:
	_vec3*					m_pPos = nullptr;


public:
	static CRcTex*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};

END
#endif // RcTex_h__
