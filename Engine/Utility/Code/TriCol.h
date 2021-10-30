#ifndef TriCol_h__
#define TriCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol : public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol(void);

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual HRESULT Ready_Buffer(_vec3 *pArryPos);
	virtual void Render_Buffer(void) override;
	void Update_Buffer(_vec3* pArryPos);
public:
	static CTriCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CTriCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pArryPos);

	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

};

END
#endif // TriCol_h__
