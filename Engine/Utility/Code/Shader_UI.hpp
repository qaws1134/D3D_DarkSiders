matrix		g_matWorld, /*g_matView, */g_matProj;		// 상수 테이블
texture		g_BaseTexture;
texture		g_SubTexture1;
texture		g_SubTexture2;

float g_SizeX;
float g_SizeY;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

sampler SubSampler1 = sampler_state
{
	texture = g_SubTexture1;

	minfilter = linear;
	magfilter = linear;
};


sampler SubSampler2 = sampler_state
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

PS_OUT		PS_SubTex1_MAIN(PS_IN In)
{

	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = Color;
	vector Color1 = tex2D(SubSampler1, In.vTexUV);
	Out.vColor += Color1;

	return Out;
};

PS_OUT		PS_SubTex2_MAIN(PS_IN In)
{

	PS_OUT		Out = (PS_OUT)0;

	vector Color = tex2D(BaseSampler, In.vTexUV);

	vector Color1 = tex2D(SubSampler1, In.vTexUV);

	vector Color2 = tex2D(SubSampler2, In.vTexUV);

	Out.vColor = Color + Color1 + Color2;

	return Out;
};

technique Default_Device
{
	pass Texture1
	{
		//ZEnable = false;
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
		
	}

	pass Texture2
	{
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;


		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_SubTex1_MAIN();
	}

	pass Texture3
	{
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;


		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_SubTex2_MAIN();
	}
};