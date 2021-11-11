#ifndef ClientStruct_h__
#define ClientStruct_h__

#include "Engine_Define.h"
#include "Enum.h"
typedef struct UISet
{
	_vec2 vPos;
	_vec2 vSize;
	_uint iTextureNum;
	wstring wstrTexture;	//텍스쳐 키값 저장
	wstring	wstrObjTag;		//직접 지정해준 ObjTag값 저장 
}UISET;

typedef struct Stone
{
	UI::STONE eCreature;
	wstring wstrName;
	wstring wstrInfo;
	_uint	iElementType;
	_bool	bRare;
}STONE;

typedef struct Font
{
	wstring wstrFont;
	wstring wstrText;
	_vec2 vPos;
	_vec4 vColor;

}UIFONT;



#endif 
