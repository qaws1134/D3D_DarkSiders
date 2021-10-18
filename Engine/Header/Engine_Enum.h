#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };
	
	// DYNAMIC�� ��� �� �����Ӹ��� ������ �ʿ��� ������Ʈ
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };
	
	enum BUFFERID{ BUFFER_TRICOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_END	};
	
	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };
		
	enum TEXTURETYPE { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END};

	enum MOUSEKEYSTATE	{ DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum MESHTYPE { MESH_STATIC, MESH_DYNAMIC, MESH_NAVI, MESH_END };

	enum COLTYPE { COL_FALSE, COL_TRUE, COL_END };
	
	enum POINT_ENGINE { POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE { COMPARE_LEFT, COMPARE_RIGHT }; // �÷��̾� ��ġ�� �������� ���ʿ� ������ ������ �̵��� ��

}

#endif // Engine_Enum_h__