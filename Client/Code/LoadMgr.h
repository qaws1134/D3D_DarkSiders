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

	//������ ������Ʈ�� ã�ư��� ���� objŰ , �±�->���̾ ���� �̸�,  
	map<wstring, map<wstring, LOAD_DATA_COL>> m_mapMeshCollider;

	map<_uint, map<_uint, MESH>> m_mapNaviData;
	//objŰ , ��ǻ� ����, �޽� ����
	//obj �����ϸ� �Ž� �ö��̴��� ���� Ű�� �ֳ� �˻�
	//������ Ÿ���������ش�-> �������̸� �Ѱ�����ϳ�?
	//1.������Ʈ ũ������Ʈ -> 2. �������̸� �ö��̴� ������ �Ѱ��� Ÿ�� ���� �� ���̾� ���
	// 3. �������� ���̾� ���
	//
	map<wstring, map<wstring, MESH>> m_mapStaticData;
	map<wstring, map<wstring, MESH>> m_mapDynamicData;

	map<wstring ,CGameObject*> m_mapHead;

};

#endif // LoadMgr_h__
