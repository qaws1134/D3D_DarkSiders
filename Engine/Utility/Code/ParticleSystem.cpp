#include "ParticleSystem.h"
#include "Management.h"
#include "Transform.h"
#include "Texture.h"
#include "Export_Utility.h"

USING(Engine)


const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem & other)
	: CGameObject(other)
	, m_eShape(other.m_eShape)
	, m_vOriginPos(other.m_vOriginPos)
	, m_fRadius(other.m_fRadius)
	, m_fAngle(other.m_fAngle)
{
}

CGameObject * CParticleSystem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CParticleSystem* pInstance = new CParticleSystem(pDevice);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}



HRESULT CParticleSystem::Ready_Object()
{
	CGameObject::Ready_Object();




	CComponent*  pCom= m_pTrasform = CTransform::Create(m_pGraphicDev);

	Set_Component(L"Com_Transform", pCom, ID_STATIC);


	srand((unsigned int)time(0));
	Sphere _Sphere;
	Box _Box;

	m_bStart = true;

	m_vbSize = 4000;
	m_vbOffset = 0;


	HRESULT hr = 0;
	
	hr = m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
		&_vb,
		0);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexBuffer() - FAILED", L"PSystem", 0);
		return false;
	}

	return S_OK;
}
_int CParticleSystem::Update_Object(const _float& fDeltaTime)
{
	if (!m_bActive)
		return 0;
	if (m_fEndSpeed != 0)
	{
		m_fEndTime += fDeltaTime;

		if (m_fEndTime > m_fEndSpeed)
		{
			m_bActive = false;
			m_fEndSpeed = 0.f;
		}
	}

	CGameObject::Update_Object(fDeltaTime);
	m_pTrasform->Update_Component(fDeltaTime);
	//_matrix matWorld;
	//m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	//_vec3 vPos;
	//m_pTrasform->Get_INFO(INFO_POS, &vPos);
	//_vec3 vWorldPos;
	//D3DXVec3TransformCoord(&vWorldPos, &vPos,&matWorld);

	//matWorld._41 = vWorldPos.x;
	//matWorld._42 = vWorldPos.y;
	//matWorld._43 = vWorldPos.z;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);


	if (m_bReset)
	{
		m_listParticles.swap(list<PATTRIBUTE>());
		m_listParticles.clear();
		for (int i = 0; i < m_tOption.iAmount; i++)
		{
			addParticle();
		}
		m_bReset = false;
	}

	list<PAttribute>::iterator i;
	for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
	{
		i->_velocity += _vec3{ 0.f,-m_tOption.fGravity,0.f }*fDeltaTime*3.f;

		if (m_eShape == ShapeType::BOX)
		{
			i->_position += i->_velocity * fDeltaTime;

			if (_ShapeBox.isPointInside(i->_position) == false)
			{

				resetParticle(&(*i));
			}
		}
		else
		{
			if (i->_isAlive)
			{
				i->_position += i->_velocity * fDeltaTime;

				i->_age += fDeltaTime;

				if (i->_age > i->_lifeTime) // kill  
				{
					i->_isAlive = false;
					if (m_tOption.bLoop)
						resetParticle(&(*i));
				}
			}
		}
	}

	Add_RenderGroup(RENDER_ALPHA, this);
	return _uint();
}




void CParticleSystem::preRender()
{


	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_tOption.fSize));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));


	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// control the size of the particle relative to distance
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// use alpha from texture
	//m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	///////////////// 알파테스팅 ////////////////////////////////////////////////

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	//m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	///////////////////////////////////////////////////////
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CParticleSystem::postRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CParticleSystem::Render_Object()
{

	if (!m_listParticles.empty())
	{
		CGameObject::Render_Object();

		auto iter_find = 	find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Com_Transform"));
		m_pGraphicDev->SetTransform(D3DTS_WORLD, dynamic_cast<CTransform*>(iter_find->second)->Get_WorldMatrix());


		//랜더 상태 저장
		preRender();

		m_pGraphicDev->SetTexture(0, _tex);

		m_pGraphicDev->SetFVF(PARTICLE::FVF);
		m_pGraphicDev->SetStreamSource(0, _vb, 0, sizeof(Particle));


		// render batches one by one
		//

		//버텍스 버퍼를 벗어날 경우 처음부터 시작
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		PARTICLE* tParticle = 0;

		_vb->Lock(
			m_vbOffset * sizeof(Particle),
			m_tOption.iBatchSize * sizeof(Particle),
			(void**)&tParticle,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//
		// 모든 파티클이 랜더링될 때까지
		//
		list<PATTRIBUTE>::iterator iter;
		for (iter = m_listParticles.begin(); iter != m_listParticles.end(); iter++)
		{

			if (iter->_isAlive)
			{
				//
				// 한 단계의 생존한 파티클을
				// 다음 버텍스 버퍼 세그먼트로 복사한다
				//
				tParticle->_position = iter->_position;
				tParticle->_color = (D3DCOLOR)iter->_color;
				tParticle++; // next element;
				
				numParticlesInBatch++;	//단계 카운터를 증가시킨다

										//현재 파티클이 가득 찼는가?
				if (numParticlesInBatch == m_tOption.iBatchSize)
				{
					//
					// 버텍스 버퍼로 복사된 마지막 
					// 마지막 단계의 파티클을 그린다
					//
					_vb->Unlock();

					
					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						m_vbOffset,
						m_tOption.iBatchSize);

					//
					// 단계가 그려지는 동안 
					// 다음 단계를 파티클로 채운다
					//

					// 다음 단계의 처음 오프셋으로  이동한다
					m_vbOffset += m_tOption.iBatchSize;

					//버텍스 버퍼의 경계를 넘는 메모리로 오프셋을 설정하지 않는다
					//경계를 넘을 경우 처음부터 시작한다
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					_vb->Lock(
						m_vbOffset * sizeof(Particle),
						m_tOption.iBatchSize * sizeof(Particle),
						(void**)&tParticle,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // 다음 단계를 위한 리셋
				}
			}
		}

		_vb->Unlock();

		// 	if (numParticlesInBatch == _vbBatchSize)조건이 
		// 만족되지 못하여 마지막 단계가 랜더링 되지 않는 경우가 발생할 수 있다
		// 일부만 채워진 단계는 바로 이곳에서 랜더링 된다

		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(
				D3DPT_POINTLIST,
				m_vbOffset,
				numParticlesInBatch);
		}

		// next block
		m_vbOffset += m_tOption.iBatchSize;

		postRender();
	}

}



void CParticleSystem::Free()
{
	Safe_Release(_tex);
	Safe_Release(_vb);
	CGameObject::Free();
}

bool CParticleSystem::isEmpty()
{
	return m_listParticles.empty();
}

bool CParticleSystem::isDead()
{
	list<PATTRIBUTE>::iterator i;
	for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if (i->_isAlive)
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}


_float CParticleSystem::GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CParticleSystem::GetRandomVector(_vec3 * out, _vec3 * min, _vec3 * max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

void CParticleSystem::resetParticle(PAttribute * attribute)
{
	m_eShape = m_tOption.eType;
	Box _Box;
	Sphere _Sphere;
	switch (m_eShape)
	{
	case ShapeType::CONE:

		break;
	case ShapeType::SPHERE:
		//_Sphere._center = m_vOriginPos;
		//_ShapeSphere = _Sphere;
		m_bHalf = false;
		break;
	case ShapeType::SPHERE_HALF:
		m_bHalf = true;
		break;
	case ShapeType::BOX:
		_Box._min = m_tOption.vMinSize;
		_Box._max = m_tOption.vMaxSize;
		_ShapeBox = _Box;

		break;
	default:
		break;
	}


	switch (m_eShape)
	{
	case ShapeType::CONE:
	{

		attribute->_isAlive = true;
		_float fRandomAngle = GetRandomFloat(0.f, m_tOption.fArc);
		fRandomAngle = D3DXToRadian(fRandomAngle);
		m_fAngle = D3DXToRadian(m_tOption.fAngle);
		attribute->_position.x = m_tOption.fRadius*cosf(fRandomAngle);
		attribute->_position.z = m_tOption.fRadius*sinf(fRandomAngle);
		attribute->_position.y = m_tOption.fRadius* atanf(m_fAngle);

		attribute->_velocity.x = m_tOption.fRadius*cosf(fRandomAngle) - m_tOption.fRadius*sinf(fRandomAngle);
		attribute->_velocity.z = m_tOption.fRadius*sinf(fRandomAngle) + m_tOption.fRadius*cosf(fRandomAngle);
		attribute->_velocity.y = m_tOption.fRadius*atanf(m_fAngle);


		_float fRandomSizeSpeed = GetRandomFloat(0.f,m_tOption.fSizeSpeed);

		if (m_tOption.bSpeed)
		{
			attribute->_velocity.y *= GetRandomFloat(m_tOption.fStartSpeed, m_tOption.fEndSpeed);
		}
		else
		{
			attribute->_velocity.y *= m_tOption.fStartSpeed;
		}
		if (m_tOption.bColor)
		{

			attribute->_color = D3DXCOLOR(
				GetRandomFloat(m_tOption.vStartColor.a, m_tOption.vEndColor.a),
				GetRandomFloat(m_tOption.vStartColor.r, m_tOption.vEndColor.r),
				GetRandomFloat(m_tOption.vStartColor.g, m_tOption.vEndColor.g),
				GetRandomFloat(m_tOption.vStartColor.b, m_tOption.vEndColor.b));
		}
		else
		{
			attribute->_color = D3DXCOLOR(
				m_tOption.vStartColor.r,
				m_tOption.vStartColor.g,
				m_tOption.vStartColor.b,
				m_tOption.vStartColor.a);
		}

		attribute->_age = 0.f;
		if (m_tOption.bLifeTime)
		{
			attribute->_lifeTime = GetRandomFloat(m_tOption.fStartLifeTime, m_tOption.fEndLifeTime);
		}
		else
		{
			attribute->_lifeTime = m_tOption.fStartLifeTime;
		}


		if (m_tOption.bStartDraw)
		{
			_float fDeltaTime = 0.01f;
			_float fTime = 0.f;
			//해당 시간이 지난만큼 m_listparticle을 옮겨줌 
			while (fTime < m_tOption.fStartTime)
			{
				fTime += fDeltaTime;

				attribute->_velocity += _vec3{ 0.f,-m_tOption.fGravity,0.f }*fDeltaTime*10.f;


				attribute->_position += attribute->_velocity*100.f * fDeltaTime;	//가속해서 셋팅
				attribute->_age += fDeltaTime;
			}	
		}


		break;
	}
	case ShapeType::SPHERE_HALF:
	{
		_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	}
	case ShapeType::SPHERE:
	{
		_vec3 min;
		_vec3 max;
		attribute->_isAlive = true;
		if (!m_bHalf)
		{
			min = _vec3(-1.0f, -1.0f, -1.0f);
			max = _vec3(1.0f, 1.0f, 1.0f);
		}
		attribute->_position = _vec3{ 0.f,0.1f,0.f };
		GetRandomVector(
			&attribute->_velocity,
			&min,
			&max);

		// normalize to make spherical
		D3DXVec3Normalize(
			&attribute->_velocity,
			&attribute->_velocity);

		if (m_tOption.bSpeed)
		{
			attribute->_velocity *= GetRandomFloat(m_tOption.fStartSpeed, m_tOption.fEndSpeed);
		}
		else
		{
			attribute->_velocity *= m_tOption.fStartSpeed;
		}
		if (m_tOption.bColor)
		{

			attribute->_color = D3DXCOLOR(
				GetRandomFloat(m_tOption.vStartColor.a, m_tOption.vEndColor.a),
				GetRandomFloat(m_tOption.vStartColor.r, m_tOption.vEndColor.r),
				GetRandomFloat(m_tOption.vStartColor.g, m_tOption.vEndColor.g),
				GetRandomFloat(m_tOption.vStartColor.b, m_tOption.vEndColor.b));
		}
		else
		{
			attribute->_color = D3DXCOLOR(
				m_tOption.vStartColor.a,
				m_tOption.vStartColor.r,
				m_tOption.vStartColor.g,
				m_tOption.vStartColor.b);
		}

		attribute->_age = m_tOption.fDuration;
		if (m_tOption.bLifeTime)
		{
			attribute->_lifeTime = GetRandomFloat(m_tOption.fStartLifeTime, m_tOption.fEndLifeTime);
		}
		else
		{
			attribute->_lifeTime = m_tOption.fStartLifeTime;
		}
	}
	break;

	case ShapeType::BOX:
	{
		attribute->_isAlive = true;

		// get random x, z coordinate for the position of the snow flake.
		GetRandomVector(
			&attribute->_position,
			&_ShapeBox._min,
			&_ShapeBox._max);

		// no randomness for height (y-coordinate).  Snow flake
		// always starts at the top of bounding box.
		attribute->_position.y = _ShapeBox._max.y;
		//내려오는 방향
		// snow flakes fall downwards and slightly to the left
		attribute->_velocity.x = GetRandomFloat(0.0f, 1.0f) * m_tOption.fStartSpeed*-0.3f;
		attribute->_velocity.y = GetRandomFloat(0.0f, 1.0f) *m_tOption.fStartSpeed*-1.0f;
		attribute->_velocity.z = 0.0f;

		// white snow flake
		if (m_tOption.bColor)
		{

			attribute->_color = D3DXCOLOR(
				GetRandomFloat(m_tOption.vStartColor.a, m_tOption.vEndColor.a),
				GetRandomFloat(m_tOption.vStartColor.r, m_tOption.vEndColor.r),
				GetRandomFloat(m_tOption.vStartColor.g, m_tOption.vEndColor.g),
				GetRandomFloat(m_tOption.vStartColor.b, m_tOption.vEndColor.b));
		}
		else
		{
			attribute->_color = D3DXCOLOR(
				m_tOption.vStartColor.a,
				m_tOption.vStartColor.r,
				m_tOption.vStartColor.g,
				m_tOption.vStartColor.b);
		}
		break;
	}
	default:
		break;
	}
}

void CParticleSystem::addParticle()
{
	PATTRIBUTE attribute;

	resetParticle(&attribute);
	if (m_tOption.bStartDraw)
		m_tOption.bStartDraw = false;
	m_listParticles.push_back(attribute);
}


void CParticleSystem::SetParticle(const TCHAR * szFilePath, const POption& option)
{
	m_bReset = true;
	Safe_Release(_tex);
	D3DXCreateTextureFromFile(
		m_pGraphicDev,
		szFilePath,
		&_tex);


	m_tOption.fDuration = option.fDuration;
	m_tOption.bLifeTime = option.bLifeTime;
	m_tOption.bSpeed = option.bSpeed;

	m_tOption.fStartLifeTime = option.fStartLifeTime;
	m_tOption.fEndLifeTime = option.fEndLifeTime;

	m_tOption.fStartSpeed = option.fStartSpeed;
	m_tOption.fEndSpeed = option.fEndSpeed;


	m_tOption.fSize = option.fSize;

	m_tOption.fGravity = option.fGravity;

	if (option.bColor)
	{
		m_tOption.vStartColor = option.vStartColor;
		m_tOption.vEndColor = option.vEndColor;
	}
	else
	{
		m_tOption.vStartColor = option.vStartColor;
	}

	m_tOption.eType = option.eType;
	m_tOption.fAngle = option.fAngle;
	m_tOption.fRadius = option.fRadius;
	m_tOption.fArc = option.fArc;
	m_tOption.iBatchSize = option.iBatchSize;
	m_tOption.iAmount = option.iAmount;
	m_tOption.bLoop = option.bLoop;

	m_tOption.vMinSize = option.vMinSize;
	m_tOption.vMaxSize = option.vMaxSize;


	m_tOption.bStartDraw = option.bStartDraw;
	m_tOption.fStartTime = option.fStartSpeed;
	m_tOption.fSizeSpeed = option.fSizeSpeed;

	m_fEndSpeed = option.fEndTimer;
}

void CParticleSystem::SetPosition(_vec3 _Position)
{

	for (auto& pParticle : m_listParticles)
	{
		pParticle._position = _Position;
	}
}

bool CParticleSystem::Down_Object(list<_vec3>& _Object)
{
	for (auto& pParticle : m_listParticles)
	{
		if (pParticle._position.y < -0.1f && pParticle._isAlive)
		{
			pParticle._isAlive = false;
			_Object.emplace_back(_vec3(pParticle._position.x, pParticle._position.y, pParticle._position.z));
			return true;
		}
	}
	return false;
}


void CParticleSystem::Set_Start(_bool START)
{
	m_bStart = START;
}



DWORD CParticleSystem::FtoDw(_float f)
{
	return *((DWORD*)&f);
}

Box::Box()
{
	_min.x = FLT_MAX;
	_min.y = FLT_MAX;
	_min.z = FLT_MAX;

	_max.x = -FLT_MAX;
	_max.y = -FLT_MAX;
	_max.z = -FLT_MAX;
}

_bool Box::isPointInside(_vec3& vPoint)
{
	if (vPoint.x >= _min.x && vPoint.y >= _min.y && vPoint.z >= _min.z &&
		vPoint.x <= _max.x && vPoint.y <= _max.y && vPoint.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

_bool Sphere::_bHalf()
{
	return _bool();
}