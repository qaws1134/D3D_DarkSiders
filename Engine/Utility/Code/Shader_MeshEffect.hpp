matrix		g_matWorld, g_matView, g_matProj;		// 상수 테이블
float		g_fUVTime, g_fUVSpeed, g_DissolveAmount;
bool g_bEmissiveExist;
vector g_vColor;


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

texture		g_DissolveTexture;
sampler DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

};


struct VS_IN_GENEREL
{
	vector		vPosition : POSITION;			// 시멘틱 : 정점의 구성 옵션(쉽게 말해 FVF)
	float2		vTexUV : TEXCOORD0;
	float3		vNormal : NORMAL;

};
struct VS_IN
{
	vector		vPosition : POSITION;			// 시멘틱 : 정점의 구성 옵션(쉽게 말해 FVF)
	float2		vTexUV : TEXCOORD0;
	float3		mNormal : NORMAL;
	float3		mTangent : TANGENT;
};
struct VS_OUT_GENEREL
{
	vector		vPosition : POSITION;
	//vector		vNormal		: NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vNormal : NORMAL;
	vector		vProjPos : TEXCOORD1;
};
struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vProjPos : COLOR0;
	float2		vTexUV : TEXCOORD0;
	float3		T : TEXCOORD1;
	float3		B : TEXCOORD2;
	float3		N : TEXCOORD3;
};

struct PS_IN_GENEREL
{
	vector		vPosition : POSITION;
	//vector		vNormal		: NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vNormal : NORMAL;
	vector		vProjPos : TEXCOORD1;
};

struct PS_OUT_GENEREL
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;

};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
	//vector			vNormal		: NORMAL;
	float3			T : TEXCOORD1;
	float3 B : TEXCOORD2;
	float3 N : TEXCOORD3;
	vector			vProjPos : COLOR0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;

};

// 버텍스 쉐이더
VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	//Out.vNormal = mul(In.vNormal, g_matWorld);

	Out.N = normalize(mul(In.mNormal, (float3x3)g_matWorld));
	Out.T = normalize(mul(In.mTangent, (float3x3)g_matWorld));
	Out.B = vector(cross(Out.N.xyz, Out.T.xyz), 0.f);

	//Out.B = normalize(mul(In.mBinormal, (float3x3)g_matWorld));
	Out.vProjPos = Out.vPosition;

	Out.vTexUV = In.vTexUV;

	return Out;
}
VS_OUT_GENEREL			VS_MAIN2(VS_IN_GENEREL In)
{
	VS_OUT_GENEREL			Out = (VS_OUT_GENEREL)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = mul(In.vNormal, g_matWorld);

	Out.vProjPos = Out.vPosition;

	Out.vTexUV = In.vTexUV;

	return Out;
}
PS_OUT		PS_MAIN3(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	//vector vNormal = tex2D(NormalSampler, In.vTexUV);

	float3 tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1); // -1 ~ 1 사이로 변환.

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal); // 이 값을 0 ~ 1사이로 다시 변환해야 한다.

	worldNormal = worldNormal * 0.5f + 0.5f;

	Out.vNormal = float4(worldNormal, 0.f);

	Out.vColor = vColor;


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w * 0.001f,
		0.f,
		0.f);


	return Out;
}

PS_OUT		PS_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float vDissolve = tex2D(DissolveSampler, In.vTexUV).x;

	float ClipAmount = vDissolve - g_DissolveAmount;


	vector vColor = tex2D(BaseSampler, In.vTexUV);

	//vector vNormal = tex2D(NormalSampler, In.vTexUV);

	float3 tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1); // -1 ~ 1 사이로 변환.

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal); // 이 값을 0 ~ 1사이로 다시 변환해야 한다.

	worldNormal = worldNormal * 0.5f + 0.5f;

	Out.vNormal = float4(worldNormal, 0.f);

	Out.vColor = vColor;


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w * 0.001f,
		0.f,
		0.f);

	if (ClipAmount < 0.2f)
	{
		Out.vColor = 1.f;
	}

	if (ClipAmount < 0.f)
	{
		Out.vColor = ClipAmount;
	}
	return Out;
}


PS_OUT		PS_DISSOLVE_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector vColor = tex2D(BaseSampler, In.vTexUV);

	//vector vNormal = tex2D(NormalSampler, In.vTexUV);

	float3 tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1); // -1 ~ 1 사이로 변환.

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal); // 이 값을 0 ~ 1사이로 다시 변환해야 한다.

	worldNormal = worldNormal * 0.5f + 0.5f;

	Out.vNormal = float4(worldNormal, 0.f);

	Out.vColor = vColor;


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w * 0.001f,
		0.f,
		0.f);

	Out.vColor.w = Out.vColor.w - g_DissolveAmount;

	return Out;
}

PS_OUT_GENEREL		PS_GENEREL(PS_IN_GENEREL In)
{
	PS_OUT_GENEREL		Out = (PS_OUT_GENEREL)0;

	float2 vTexUV = In.vTexUV;
	vTexUV.x = (In.vTexUV.x - g_fUVTime* g_fUVSpeed);

	vector vColor = tex2D(BaseSampler, vTexUV);
	Out.vColor.xyz = g_vColor.xyz;
	Out.vColor.w = vColor.r;



	//Out.vColor = tex2D(BaseSampler, vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	return Out;
}


technique Default_Technique
{
	pass AlphaBlend
	{
				LIGHTING = FALSE;
		//CULLMODE = NONE;		 // <- ㅡㅡ
		ALPHABLENDENABLE = TRUE;

		DESTBLEND = ONE/*INVSRCALPHA*/;		//<- 사용했던거 
		SRCBLEND = SRCALPHA;

		//SRCBLEND = SRCALPHA;				// <- 이게 디폴트라고 함
		//DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN3();
	}

	pass AlphaTest
	{
			lighting = false;
		alphatestenable = true;
		alphafunc = greater;
		ZWRITEENABLE = true;
		alpharef = 1;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN3();

	}
	pass
	{
			lighting = false;
		alphatestenable = false;
		ZWRITEENABLE = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN3();

	}



	pass dissolve
	{
			lighting = false;
		alphatestenable = true;
		alphafunc = greater;
		ZWRITEENABLE = true;
		alpharef = 1;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE();

	}
	pass PS_DISSOLVE_ALPHA
	{
			lighting = false;
		alphatestenable = true;
		alphafunc = greater;
		ZWRITEENABLE = true;
		alpharef = 1;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE_ALPHA();

	}
	pass Water
	{
		lighting = false;
	ZWRITEENABLE = false;
	Alphablendenable = true;
	alphatestenable = false;
	cullmode = none;

	vertexshader = compile vs_3_0 VS_MAIN2();
	pixelshader = compile ps_3_0 PS_GENEREL();

	}
};
