#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Engine_Define.h"
#include "Component.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CComponent
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:	
	void			Set_AnimationIndex(const _uint& iIndex);
	void			Play_Animation(const _float& fTimeDelta);
	const			D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	_bool			Is_AnimationsetFinish(void);
public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);

private:
	// ��� ������ ��������� ��ȸ�ϸ鼭 �θ�� �ڽ�, �Ǵ� ���� ���踦 ���� �� �ֵ��� ��ĵ��� ���Ͽ� �ϼ��� ���� ��� ���¸� ������ִ� �Լ�
	void					Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void					SetUp_FrameMatrices(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*									m_pRootFrame;
	CHierarchyLoader*							m_pLoader;
	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;
	CAniCtrl*									m_pAniCtrl;

public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*			Clone(void);

private:
	virtual void	Free(void);
};

END
#endif // DynamicMesh_h__

// �� ������ �����ϱ� ���� ����ü
//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;						// ���� �̸��� �����ϱ� ���� ����(�����ڵ� �ƴ� �ƽ�Ű ������ ���ڿ�)
//	D3DXMATRIX              TransformationMatrix;		// ���� ���� ���� ���� �����ϰ� �ִ� ���
//
//	LPD3DXMESHCONTAINER     pMeshContainer;				// �޽� �����̳�(����ƽ �޽�)
//
//	struct _D3DXFRAME       *pFrameSibling;				// ���� ���� �ּ�
//	struct _D3DXFRAME       *pFrameFirstChild;			// �ڽ� ���� �ּ�
//
//} D3DXFRAME, *LPD3DXFRAME;

// ���� ������ �޽��� �������� ������ �����ϴ� ����ü
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;				// �޽� �����̳��� �̸�(�����ڵ� �ƴ� �ƽ�Ű ������ ���ڿ�), ������ ��� �׸�, ���� ������� �ʴ´�.
//
//	D3DXMESHDATA            MeshData;			// �޽��� ������ �����ϰ�, �ش��ϴ� �޽� �İ�ü�� �����ϱ� ���� ����ü
//
//	LPD3DXMATERIAL          pMaterials;			// �޽��� ���� ������ �ؽ�ó �̸��� �����ϱ� ���� ����ü
//	LPD3DXEFFECTINSTANCE    pEffects;			// �޽��� ���� ������ ����Ʈ ������ �����ϱ� ���� �İ�ü(�츮�� ����� �� ����)
//	DWORD                   NumMaterials;		// ������ ���� == ������� ���� == �ؽ����� ����
//	DWORD                  *pAdjacency;			// ������ ������ �Ǵ� ������ ������ ���� �迭�� ���·� ������ ��, ù ��° �ּҸ� ����
//
//	LPD3DXSKININFO          pSkinInfo;			// ��Ű���� �����ϱ� ���� ���� �Լ��� �����ϴ� �İ�ü
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// ���� �޽������̳��� �ּҸ� �����ϱ� ���� ������
//
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;