#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CComponent
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	HRESULT	Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void	Render_Meshes(void);

private:
	LPD3DXMESH				m_pMesh;		// �븻������ ������ �޽� ������ ��� �ִ� ��ü
	LPD3DXMESH				m_pOriMesh;		// ���� �ε� ������ �޽� ������ ��� �ִ� ��ü

	LPD3DXBUFFER			m_pAdjacency;	// �޽��� ���� ������ �ٰŷ� �̷��� ������ ������ ���� ������ �����ϱ� ���� ��
	LPD3DXBUFFER			m_pSubset;		// �޽��� �����ϴ� �����ݿ� �ش��ϴ� ����
											// ������� ���� == ������ ���� == �ؽ�ó�� ����
	D3DXMATERIAL*			m_pMtrl;		// �ؽ�ó�� �̸��� ������ ���� ������ �����ϱ� ���� ����ü
	_ulong					m_dwSubsetCnt;	// ������� ����
	LPDIRECT3DTEXTURE9*		m_ppTexture;	// �������� �޽� �ؽ�ó�� ���� �迭�� ���·� �����ϱ� ���� ������

public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _tchar* pFilePath, 
								const _tchar* pFileName);

	virtual CComponent*	Clone(void);

private:
	virtual void	Free(void);

};
END
#endif // StaticMesh_h__
