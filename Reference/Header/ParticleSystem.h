#pragma once
#ifndef __PARTICLESYSTEM_H__

#include "GameObject.h"

BEGIN(Engine)

typedef struct Particle
{
	D3DXVECTOR3 _position;
	D3DCOLOR    _color;

	static const DWORD FVF;
}PARTICLE;

typedef struct Box
{
	Box();
	_bool isPointInside(_vec3& vPoint);

	_vec3 _min;
	_vec3 _max;
}BOX;

typedef struct Sphere
{
	_vec3 _center;
	_bool	_bHalf();
}SPHERE;

typedef struct Cone
{
	_vec3 _Angle;
	_float  _radius;
}CONE;

enum class ShapeType
{
	CONE,
	SPHERE,
	SPHERE_HALF,
	BOX,
};

typedef struct POption
{
	_float fDuration;
	_bool bLifeTime; _float fStartLifeTime; _float fEndLifeTime;
	_bool bSpeed; _float fStartSpeed; _float fEndSpeed;
	_float fSize;
	_float fGravity;
	_bool bColor; D3DXCOLOR vStartColor; D3DXCOLOR vEndColor;
	ShapeType eType;
	_float fAngle;
	_float fRadius;
	_float fArc;
	_int   iBatchSize;
	_int	iAmount;


	_float fStartTime;
	_bool	bStartDraw;
	_bool	bLoop;
	_float fSizeSpeed;

	_vec3 vMinSize;
	_vec3 vMaxSize;
	_float fEndTimer;

}POPTION;

typedef struct PAttribute
{
	PAttribute()
	{ 
		_lifeTime = 0.0f;
		_age = 0.0f;
		_isAlive = true;
	}

	_vec3 _position;
	_vec3 _velocity;
	_vec3 _acceleration;
	_float       _lifeTime;     // how long the particle lives for before dying  
	_float       _age;          // current age of the particle  
	D3DXCOLOR   _color;        // current color of the particle   
	D3DXCOLOR   _colorFade;    // how the color fades with respect to time
	_float		_fSize;
	_bool        _isAlive;
	_bool	_isStart;
}PATTRIBUTE;


class ENGINE_DLL CParticleSystem :public CGameObject
{
public:
	explicit CParticleSystem(LPDIRECT3DDEVICE9 pDevice);
	explicit CParticleSystem(const CParticleSystem& other);
	virtual ~CParticleSystem() = default;


	static CGameObject* Create(LPDIRECT3DDEVICE9 pDevice);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fDeltaTime);

	virtual void preRender();
	virtual void postRender();

	virtual void Render_Object();

	virtual void Free();

	bool isEmpty();
	bool isDead();

public:
	_float GetRandomFloat(float lowBound, float highBound);
	void GetRandomVector(_vec3* out, _vec3* min, _vec3* max);
	void resetParticle(PAttribute* attribute);
	DWORD FtoDw(_float f);
	void addParticle();
	void SetEndTimer(_float fEndTime) { m_fEndSpeed = fEndTime; }
public:
	void SetParticle(const TCHAR* szFilePath, const POption& option);
	void SetReset(_bool bReset) { m_bReset = bReset; }
	void SetColor(D3DXCOLOR vColor) { m_tOption.vStartColor = vColor; }

public:
	void SetPosition(_vec3 _Position);
public:
	bool Down_Object(list<_vec3>& _Object);
public :
	const POption GetOption() { return m_tOption; };

public:
	void Set_Start(_bool START);

protected:
	_vec3					m_vOriginPos;		//파티클 생성기 위치
	_float                  m_fEmitRate;       // 초당 파티클 생성 수 
	_float                  m_fParticlesize;   // 모든 파티클의 사이즈

	IDirect3DTexture9*      _tex;
	IDirect3DVertexBuffer9* _vb;				
	list<PATTRIBUTE>		m_listParticles;	//파티클이 생성되고 삭제되는 곳 
	_int                    m_iMaxParticles;	//주어진 시간동안 시스템이 가질 수 있는 최대 파티클 수 
	D3DXCOLOR					m_vColor;

	ShapeType				m_eShape;			//모양 타입
	BOX						_ShapeBox;			//박스 타입
	SPHERE					_ShapeSphere;		//구체 타입

	_float					m_fRadius;
	_float					m_fAngle;
	_float					m_fDuration;
	_float					m_fSpeed;
	_float					m_fGravity;
	_float					m_fArc;


	_float					m_fEndTime = 0.f;
	_float					m_fEndSpeed = 0.f;

	POption m_tOption;

	DWORD m_vbSize;      // 버텍스 버퍼가 보관할 수 있는 파티클의 수. 이 값은 실제 파티클 시스템 내의 파티클 수 와는 독립적이다 
	DWORD m_vbOffset;    // 버텍스 버퍼에서 복사를 시작할 파티클, 내 다음 단계로의 오프셋 예를들어, 단계1이 0부터 499까지 항목이라면 단계2로의 오프셋은 500이 된다
	DWORD m_vbBatchSize; // 하나의 단계에 정의된 파티클의 수

	_bool m_bReset=false;
	_bool m_bStart = false;
	_bool m_bHalf = false;
	class CTransform*	m_pTrasform = nullptr;
};




END
#define __PARTICLESYSTEM_H__
#endif
