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

	// CGameObject��(��) ���� ��ӵ�
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
	_vec3					m_vOriginPos;		//��ƼŬ ������ ��ġ
	_float                  m_fEmitRate;       // �ʴ� ��ƼŬ ���� �� 
	_float                  m_fParticlesize;   // ��� ��ƼŬ�� ������

	IDirect3DTexture9*      _tex;
	IDirect3DVertexBuffer9* _vb;				
	list<PATTRIBUTE>		m_listParticles;	//��ƼŬ�� �����ǰ� �����Ǵ� �� 
	_int                    m_iMaxParticles;	//�־��� �ð����� �ý����� ���� �� �ִ� �ִ� ��ƼŬ �� 
	D3DXCOLOR					m_vColor;

	ShapeType				m_eShape;			//��� Ÿ��
	BOX						_ShapeBox;			//�ڽ� Ÿ��
	SPHERE					_ShapeSphere;		//��ü Ÿ��

	_float					m_fRadius;
	_float					m_fAngle;
	_float					m_fDuration;
	_float					m_fSpeed;
	_float					m_fGravity;
	_float					m_fArc;


	_float					m_fEndTime = 0.f;
	_float					m_fEndSpeed = 0.f;

	POption m_tOption;

	DWORD m_vbSize;      // ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ�� ��. �� ���� ���� ��ƼŬ �ý��� ���� ��ƼŬ �� �ʹ� �������̴� 
	DWORD m_vbOffset;    // ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ, �� ���� �ܰ���� ������ �������, �ܰ�1�� 0���� 499���� �׸��̶�� �ܰ�2���� �������� 500�� �ȴ�
	DWORD m_vbBatchSize; // �ϳ��� �ܰ迡 ���ǵ� ��ƼŬ�� ��

	_bool m_bReset=false;
	_bool m_bStart = false;
	_bool m_bHalf = false;
	class CTransform*	m_pTrasform = nullptr;
};




END
#define __PARTICLESYSTEM_H__
#endif
