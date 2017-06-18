/*!
 * \file Engine_Enum.h
 * \date 2015/04/05 8:42
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum ANGLE { ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END };
	enum BUFFERTYPE { BUFFER_RCTEX, BUFFER_CUBECOL, BUFFER_SPHERECOL };
	enum MESHTYPE { MESH_STATIC, MESH_DYNAMIC };
	enum RENDERID { RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
	enum TEXTURETYPE { TEXTURE_NORMAL, TEXTURE_CUBE };	
}

namespace COMPONENTID
{
	enum COMPONENTID 
	{ 
		// 이동 관련
		TRANSFORM, 

		// 충돌 관련
		COLLISION_RAY,

		// 버퍼 관련
		TEXBUFFER_RECTANGLE,
		COLBUFFER_CUBE,
		COLBUFFER_SPHERE,

		// 텍스처 관련
		TEXTURE_RECTANGLE,
		
		// 메시 관련
		MESH_STATIC,
		MESH_DYNAMIC,

		COMPONENTID_END
	};
}

#endif // Engine_Enum_h__