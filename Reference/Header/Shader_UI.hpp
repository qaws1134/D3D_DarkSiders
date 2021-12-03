matrix		g_matWorld, /*g_matView, */g_matProj;		// 상수 테이블


float g_fAlpha;
float g_fUVx = 0.f;
float g_fUVy = 0.f;

bool  g_bStoneRare;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture		g_StoneTexture;
sampler StoneSampler = sampler_state
{
	texture = g_StoneTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_StoneBaseTexture;
sampler StoneBaseSampler = sampler_state
{
	texture = g_StoneBaseTexture;

minfilter = linear;
magfilter = linear;
};


texture		g_StoneEffTexture;
sampler StoneEffSampler = sampler_state
{
	texture = g_StoneEffTexture;

minfilter = linear;
magfilter = linear;
};


texture		g_SubTexture1;
sampler SubTexSampler1 = sampler_state
{
	texture = g_SubTexture1;

minfilter = linear;
magfilter = linear;
};


texture		g_SubTexture2;
sampler SubTexSampler2 = sampler_state
{
	texture = g_SubTexture2;

minfilter = linear;
magfilter = linear;
};






struct VS_IN
{
	vector		vPosition	: POSITION;			// 시멘틱 : 정점의 구성 옵션(쉽게 말해 FVF)
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;			
	float2		vTexUV		: TEXCOORD0;
};

// 버텍스 쉐이더
VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	//matWV	= mul(g_matWorld, g_matView);
	matWVP	= mul(g_matWorld, g_matProj);
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
};

VS_OUT			VS_SELLFILL(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	//matWV	= mul(g_matWorld, g_matView);
	matWVP = mul(g_matWorld, g_matProj);
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
};
struct PS_IN				// 픽셀쉐이더에서는 POSITION이란 시멘틱을 사용할 수 없다(현재 쉐이더 버전에서는 사용가능하도록 업그레이드 됨)
{
	float2			vTexUV		: TEXCOORD0;
	//vector			vPlayerPos	: TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = vColor;
	return Out;
};

PS_OUT		PS_LIST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vColor;
	return Out;
};

PS_OUT		PS_SELFILL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(BaseSampler, In.vTexUV);
	if (In.vTexUV.x >= g_fUVx)
	{
		vColor = 0.f;
	}

	Out.vColor = vColor;
	return Out;
};

PS_OUT		PS_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(BaseSampler, In.vTexUV);

	vColor.w = 0.8f;

	Out.vColor = vColor;
	return Out;
};
PS_OUT		PS_COREELEMENT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(BaseSampler, In.vTexUV);
	vector vSub1 = tex2D(SubTexSampler1, In.vTexUV);
	vector vSub2 = tex2D(SubTexSampler2, In.vTexUV);

	float2 vTexUV = In.vTexUV;
	vTexUV.x = (In.vTexUV.x + g_fUVx);
	vTexUV.y = (In.vTexUV.y + g_fUVy);
	vector vStoneEff = tex2D(StoneEffSampler, vTexUV);

	float fEffX = vStoneEff.x;
	if (vStoneEff.r > 0.2f)
	{
		if (g_bStoneRare)
		{
			vStoneEff.x = 0.5f;
			vStoneEff.y = 0.1f;
			vStoneEff.z = 0.0f;
		}
		else
		{
			vStoneEff.x = 0.2f;
			vStoneEff.y = 0.f;
			vStoneEff.z = 0.5f;
		}
		vStoneEff.xyz = vStoneEff.xyz*fEffX;
	}

	if (g_bStoneRare)
	{
		vSub2.x = 0.5f;
		vSub2.y = 0.1f;
		vSub2.z = 0.0f;
	}
	else
	{
		vSub2.x = 0.2f;
		vSub2.y = 0.f;
		vSub2.z = 0.6f;
	}





	if (vColor.w!= 0.f)
	{
		vColor.xyz = vColor.xyz + vStoneEff.xyz+ vSub2.xyz;
		if (vSub1.w > 0.f)
		{
			vColor.xyz = vColor.xyz + vSub1.xyz;
		}

	}
	//vColor.xyz = vColor.xyz +vSub2.xyz+vStoneEff.xyz;
	

	Out.vColor = vColor;
	return Out;
};

PS_OUT		PS_STONEBASE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(BaseSampler, In.vTexUV);

	vector vStoneBase = tex2D(StoneSampler, In.vTexUV);
	vector vStoneBase2 = tex2D(StoneBaseSampler, In.vTexUV);
	if (g_bStoneRare)
	{
		vStoneBase2.x = 0.5f;
		vStoneBase2.y = 0.1f;
		vStoneBase2.z = 0.0f;
	}
	else
	{
		vStoneBase2.x = 0.2f;
		vStoneBase2.y = 0.f;
		vStoneBase2.z = 0.6f;
	}


	float2 vTexUV = In.vTexUV;
	vTexUV.x = (In.vTexUV.x + g_fUVx);
	vTexUV.y = (In.vTexUV.y + g_fUVy);
	vector vStoneEff = tex2D(StoneEffSampler, vTexUV);
	float fEffX = vStoneEff.x;
	if (vStoneEff.r > 0.2f)
	{
		if (g_bStoneRare)
		{
			vStoneEff.x = 0.5f;
			vStoneEff.y = 0.1f;
			vStoneEff.z = 0.0f;
		}
		else
		{
			vStoneEff.x = 0.2f;
			vStoneEff.y = 0.f;
			vStoneEff.z = 0.5f;
		}
		vStoneEff.xyz = vStoneEff.xyz*fEffX;
	}

	vStoneBase.xyz = vStoneBase.xyz*0.1f;
	Out.vColor = vStoneBase;

	if (vColor.w > 0.f)
	{
		Out.vColor = Out.vColor + vColor;
	}
	if (vStoneBase.x > 0.f)
	{
		Out.vColor.xyz = Out.vColor.xyz + vStoneEff.xyz+vStoneBase2.w;
		
		if (vStoneBase2.w > 0.f)
		{
			Out.vColor.xyz = Out.vColor.xyz + vStoneBase2.xyz;
		}
	}


//	Out.vColor.xyz = vColor.xyz;
	//vColor.w = vColor.w- (vStoneBase.w*0.1f);

	//Out.vColor.xyz = vStoneBase.xyz;
	/*if (vColor.w < 0.9f)
	{ 
		Out.vColor.xyz = Out.vColor.xyz  + vStoneBase.xyz;
		vColor.xyz =vStoneBase.xyz;
		Out.vColor.w = vStoneBase.w;
	}
	else
	{
		Out.vColor.xyz =  vStoneBase.xyz;
		Out.vColor.w = vColor.w;
	}*/
	return Out;
};

PS_OUT		PS_STONEEFF(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vTexUV = In.vTexUV;
	vTexUV.x = (In.vTexUV.x+g_fUVx);
	vTexUV.y = (In.vTexUV.y + g_fUVy);
	vector vStoneEff = tex2D(StoneEffSampler, vTexUV);

	if (g_bStoneRare)
	{
		vStoneEff.x = 0.7f;
		vStoneEff.y = 0.3f;
		vStoneEff.z = 0.0f;
		//vStoneEff.w = 0.1f;
	}
	else
	{
		vStoneEff.x = 0.5f;
		vStoneEff.y = 0.f;
		vStoneEff.z = 0.9f;
		//vStoneEff.w = 0.1f;
	}
	Out.vColor = vStoneEff;
	return Out;
};


technique Default_Device
{
	pass Texture0
	{
		//ZEnable = false;
		Alphablendenable = true;
		alphatestenable = false;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
		
	}
	pass LIST1
	{
		//ZEnable = false;
		Alphablendenable = true;
		alphatestenable = false;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		SCISSORTESTENABLE = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_LIST();
	}
	pass SELFILL2
	{
		//ZEnable = false;
		alphatestenable = false;
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		SCISSORTESTENABLE = true;
		vertexshader = compile vs_3_0 VS_SELLFILL();
		pixelshader = compile ps_3_0 PS_SELFILL();
	}
	pass PS_STONEEFF3
	{
		//ZEnable = false;
		ALPHATESTENABLE = TRUE;
		alphafunc = greater;
		alpharef = 0x00000008f;

		Alphablendenable = false;
		SCISSORTESTENABLE = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_STONEEFF();
	}
	pass PS_STONEBASE4
	{
		//ZEnable = false;
		ALPHATESTENABLE = false;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		Alphablendenable = true;
		SCISSORTESTENABLE = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_STONEBASE();
	}

	pass PS_COREBASE5
	{
		//ZEnable = false;
		ALPHATESTENABLE = false;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		Alphablendenable = true;
		SCISSORTESTENABLE = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_COREELEMENT();
	}


	pass BLEND6
	{
		//ZEnable = false;
		ALPHATESTENABLE = false;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		Alphablendenable = true;
		SCISSORTESTENABLE = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_BLEND();
	}


};