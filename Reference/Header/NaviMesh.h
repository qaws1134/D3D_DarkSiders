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
	_uint			Get_CellIndex() { return  m_dwIndex; }

	void			Add_Cell( _vec3 vPosA, _vec3 vPosB, _vec3 vPosC);
public:
	HRESULT			Ready_NaviMesh();
	HRESULT			Ready_NaviMesh(map<_uint, map<_uint, MESH>> mapNavi);
	void			Render_NaviMesh(void);
	_vec3			MoveOn_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, const _float& fSpeed, const _float& fTimeDelta, class CCalculator* pCalcul);
	//_bool			PickOn_NaviMesh(const _vec2  vMousPos, const _vec2  vWindowSize);
	HRESULT			Link_Cell(void);

private:

private:
	vector<CCell*>			m_vecCell;
	_ulong					m_dwIndex;

public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static  CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, map<_uint, map<_uint, MESH>> mapNavi);
	//static  CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, map<_ulong, map<_ulong, CCell*>> mapNavi);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};

END
#endif // NaviMesh_h__
