matrix		g_matWorld, g_matView, g_matProj;		// ��� ���̺�
vector g_vColor = {1.f,1.f,1.f,1.f};
float g_fLifeSpeed = 0.f;
float g_fLifeTime= 0.f;



texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};


texture		g_BackTexture;
sampler BackSampler = sampler_state
{
	texture = g_BackTexture;

minfilter = linear;
magfilter = linear;
};


struct VS_IN
{
	vector		vPosition	: POSITION;			// �ø�ƽ : ������ ���� �ɼ�(���� ���� FVF)
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;			
	float2		vTexUV		: TEXCOORD0;
};

// ���ؽ� ���̴�
VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV	= mul(g_matWorld, g_matView);
	matWVP	= mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN				// �ȼ����̴������� POSITION�̶� �ø�ƽ�� ����� �� ����(���� ���̴� ���������� ��밡���ϵ��� ���׷��̵� ��)
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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó���� uv��ǥ�� �ش��ϴ� �ȼ��� ������ �����ϴ� �Լ�, ��ȯ Ÿ���� vector Ÿ��
	
	//Out.vColor.rb = 0.5f;
	Out.vColor = Out.vColor* g_vColor;
	return Out;
}

PS_OUT		PS_MAIN_TEMP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó���� uv��ǥ�� �ش��ϴ� �ȼ��� ������ �����ϴ� �Լ�, ��ȯ Ÿ���� vector Ÿ��
	//Out.vColor= Out.vColor* g_vColor;

	return Out;
}

PS_OUT		PS_MAIN_BLACKOUT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó���� uv��ǥ�� �ش��ϴ� �ȼ��� ������ �����ϴ� �Լ�, ��ȯ Ÿ���� vector Ÿ��
	
	if (vColor.x > 0.1f)
	{
		Out.vColor = vColor*g_vColor;
	}
	

	return Out;
}


PS_OUT		PS_MAIN_BLACKOUT_BackGround(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó���� uv��ǥ�� �ش��ϴ� �ȼ��� ������ �����ϴ� �Լ�, ��ȯ Ÿ���� vector Ÿ��
	vector vBack = tex2D(BackSampler, In.vTexUV);


	if (vBack.x > 0.2f)
	{
		vBack = vBack* g_vColor;
	}
	if (vColor.z > 0.2f)
	{
		vColor = vColor*g_vColor;
	}
	Out.vColor = vBack + vColor;
	if (Out.vColor.x < 0.1f)
		Out.vColor = 0.f;
	else
		Out.vColor.w = 0.9f;
	return Out;
}

technique Default_Device
{
	// pass : ����� ĸ��ȭ�� ���� ���� �� ������ �� ����, ������ �Լ� ȣ���� ����
	// �н��� �̸��� ������ ������ �������. pass�� ���ʿ� ����� �༮���� 0�� �ε����� �����ȴ�.

	pass Alphablend
	{
		alphatestenable = false;
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
		
	}

	pass alphatest
	{
		Alphablendenable = false;

		ALPHATESTENABLE = TRUE;
		alphafunc = greater;
		alpharef = 254;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_TEMP();

	}

	pass Black3
	{

		//Alphablendenable = false;
		//ALPHATESTENABLE = TRUE;
		//alphafunc = greater;
		//alpharef = 0x00000008f;
		zwriteenable = false;
		alphatestenable = false;
		Alphablendenable = true;
		cullmode = none;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_BLACKOUT();

	}
		pass Black3
	{

		zwriteenable = false;
		alphatestenable = false;
		Alphablendenable = true;
		cullmode = none;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_BLACKOUT_BackGround();

	}
};