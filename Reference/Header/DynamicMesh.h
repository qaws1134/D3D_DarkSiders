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

	void			Set_AnimationIndex(const _uint& iIndex, _bool bBlend = true);
	void			Play_Animation(const _float& fTimeDelta);
	const			D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	_bool			Is_AnimationsetFinish(void);
	_bool			Is_Animationset(_double dRadius);
	_uint			GetMaxNumAnimationSet() { if (!m_pAniCtrl)return 0;  return m_pAniCtrl->GetMaxNumAnimationSets(); }

	_vec3			GetBonePos(const char* pFrameName);

	map<const char*, map<_ulong, const char*>> GetBoneNameList() { return m_mapBoneName; }

	D3DXFRAME*		GetRootFrame() { return m_pRootFrame; }

	//list<D3DXMESHCONTAINER_DERIVED*> GetMeshContainerList() { return m_MeshContainerList; }
	//컨테이너 리스트 받아서 포지션값들을 그냥 저장해서 넘겨줒 ㅏ
public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

private:
	// 모든 뼈들을 재귀적으로 순회하면서 부모와 자식, 또는 형제 관계를 맺을 수 있도록 행렬들을 곱하여 완성된 월드 행렬 상태를 만들어주는 함수
	void							Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void							SetUp_FrameMatrices(D3DXFRAME_DERIVED* pFrame);
	



private:
	D3DXFRAME*									m_pRootFrame;
	CHierarchyLoader*							m_pLoader;
	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;
	map<const char*, map<_ulong, const char*>>			m_mapBoneName;
	
	
	CAniCtrl*									m_pAniCtrl;
public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*			Clone(void);

private:
	virtual void	Free(void);
};

END
#endif // DynamicMesh_h__

// 뼈 정보를 저장하기 위한 구조체
//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;						// 뼈의 이름을 저장하기 위한 변수(유니코드 아닌 아스키 상태의 문자열)
//	D3DXMATRIX              TransformationMatrix;		// 뼈의 최초 상태 값을 보관하고 있는 행렬
//
//	LPD3DXMESHCONTAINER     pMeshContainer;				// 메쉬 컨테이너(스태틱 메쉬)
//
//	struct _D3DXFRAME       *pFrameSibling;				// 형제 뼈의 주소
//	struct _D3DXFRAME       *pFrameFirstChild;			// 자식 뼈의 주소
//
//} D3DXFRAME, *LPD3DXFRAME;

// 뼈를 제외한 메쉬의 실제적인 정보를 저장하는 구조체
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;				// 메쉬 컨테이너의 이름(유니코드 아닌 아스키 상태의 문자열), 하지만 없어도 그만, 거의 사용하지 않는다.
//
//	D3DXMESHDATA            MeshData;			// 메쉬의 종류를 선택하고, 해당하는 메쉬 컴객체를 보관하기 위한 구조체
//
//	LPD3DXMATERIAL          pMaterials;			// 메쉬의 재질 정보와 텍스처 이름을 보관하기 위한 구조체
//	LPD3DXEFFECTINSTANCE    pEffects;			// 메쉬가 지닌 고유의 이펙트 정보를 보관하기 위한 컴객체(우리는 사용할 수 없다)
//	DWORD                   NumMaterials;		// 재질의 개수 == 서브셋의 개수 == 텍스쳐의 개수
//	DWORD                  *pAdjacency;			// 인접한 폴리곤 또는 정점의 정보를 동적 배열의 형태로 보관한 뒤, 첫 번째 주소만 저장
//
//	LPD3DXSKININFO          pSkinInfo;			// 스키닝을 수행하기 위한 각종 함수를 제공하는 컴객체
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// 다음 메쉬컨테이너의 주소를 저장하기 위한 포인터
//
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;