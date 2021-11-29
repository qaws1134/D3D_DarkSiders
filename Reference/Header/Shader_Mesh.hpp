matrix		g_matWorld, g_matView, g_matProj;		// ��� ���̺�
float		g_fUVTime, g_fUVSpeed;
bool g_bEmissiveExist;


texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

MIPFILTER = NONE;
MINFILTER = POINT;
MAGFILTER = POINT;
}; 

texture		g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

};

texture g_EmissiveTexture;

sampler EmissiveSampler = sampler_state
{
	texture = g_EmissiveTexture;
};


struct VS_IN
{
	vector		vPosition	: POSITION;			// �ø�ƽ : ������ ���� �ɼ�(���� ���� FVF)
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;	
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
};

// ���ؽ� ���̴�
VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV	= mul(g_matWorld, g_matView);
	matWVP	= mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = mul(In.vNormal, g_matWorld);
	Out.vProjPos = Out.vPosition;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN				
{
	float2			vTexUV		: TEXCOORD0;	
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal: COLOR1;
	vector			vDepth : COLOR2;

};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	// ���� ���ͷ� ����ȭ�� ���� ���� �������� �븻 ������ ��ǥ ���� -1~1�̶� ������ ���´�.
	// �̷� ��ǥ�� �ؽ��� uv��ǥ�� 0~1�� �����Ͽ� ����ؾ� �Ѵ�.
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	// r : z�����Ⱑ ���� ���� ������ z�� ����
	// g : �� �����̽� ������ z���� ����
	// b, a �ƹ� ���� �������� ���� ����(far������ ���� ��)
	
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, 
						In.vProjPos.w * 0.001f,
						0.f, 
						0.f);


	return Out;
}

PS_OUT		PS_MAIN2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	vector vNormal = tex2D(NormalSampler, In.vTexUV);

	Out.vColor = vColor;
	Out.vNormal = vNormal;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w * 0.001f,
		0.f,
		0.f);

	//if (g_bEmissiveExist)
	//{
	//	// EmissiveTexture
	//	float4 emissive = tex2D(EmissiveSampler, In.vTexUV);
	//	Out.vColor.a = emissive.r;
	//}


	return Out;
}

PS_OUT		PS_UVMOVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 vTexUV = In.vTexUV;
	vTexUV.y = (In.vTexUV.y-g_fUVTime* g_fUVSpeed);

	vector vColor = tex2D(BaseSampler, vTexUV);

	Out.vColor = vector(0.f, 0.5f, 0.7f, vColor.r);


	//Out.vColor = tex2D(BaseSampler, vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	return Out;
}


technique Default_Device
{
	pass AlphaTest
	{
		lighting = false;
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 1;
	
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
		
	}
	pass 
	{ 
		lighting = false;
		alphatestenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();	

	}


	pass UVmove
	{ 
		lighting = false;
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		//cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_UVMOVE();

	}
};