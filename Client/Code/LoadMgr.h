#ifndef LoadMgr_h__
#define LoadMgr_h__

#include "Base.h"
#include "Define.h"
#include "Struct.h"
#include "GameObject.h"

class CLoadMgr : public CBase
{
	DECLARE_SINGLETON(CLoadMgr)
private:
	explicit CLoadMgr();
	virtual ~CLoadMgr();

public:
	HRESULT LoadData(wstring szFilePath);
	HRESULT LoadColTool(wstring szFilePath);
	HRESULT LoadMeshTool(wstring szFilePath);

	map<wstring, CGameObject*> SpawnData();
	void SetTransform(CTransform* pTransform, MESH tMesh);
		
private:
	virtual void Free(void) override;
//	map<wstring , list<>>

	//생성된 오브젝트를 찾아가기 위한 obj키 , 태그->레이어에 넣을 이름,  
	map<wstring, map<wstring, LOAD_DATA_COL>> m_mapMeshCollider;

	map<_uint, map<_uint, MESH>> m_mapNaviData;
	//obj키 , 사실상 갯수, 메시 정보
	//obj 생성하며 매시 컬라이더에 같은 키가 있나 검색
	//있으면 타겟지정해준다-> 오비제이를 넘겨줘야하나?
	//1.오브젝트 크리에이트 -> 2. 오비제이를 컬라이더 스폰에 넘겨줌 타겟 지정 후 레이어 등록
	// 3. 오비제이 레이어 등록
	//
	map<wstring, map<wstring, MESH>> m_mapStaticData;
	map<wstring, map<wstring, MESH>> m_mapDynamicData;

	map<wstring ,CGameObject*> m_mapHead;

};

#endif // LoadMgr_h__
