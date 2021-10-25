#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Component.h"

#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CComponent
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void			Set_CellIndex(const _ulong& dwIndex) { m_dwIndex = dwIndex; }
	void			Set_CellUpdate(_ulong dwIndex,_vec3 vPosA, _vec3 vPosB, _vec3 vPosC);
public:
	HRESULT			Ready_NaviMesh(void);
	void			Render_NaviMesh(void);
	_vec3			MoveOn_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, const _float& fSpeed, const _float& fTimeDelta);
	

private:
	HRESULT			Link_Cell(void);

private:
	vector<CCell*>			m_vecCell;
	_ulong					m_dwIndex;

public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};

END
#endif // NaviMesh_h__
