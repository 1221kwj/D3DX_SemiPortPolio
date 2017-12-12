#pragma once

struct PositionSample
{
	int				n;
	D3DXVECTOR3		v;
};

struct RotationSample
{
	int				n;
	D3DXQUATERNION	q;
};

struct ASEScene
{
	int firstFrame;
	int lastFrame;
	int frameSpeed;
	int ticksPerFrame;
};

struct SphereInfo
{
	bool		isInFrustum;
	bool		isPicked;
	float		fRadius;
	D3DXVECTOR3 vCenter;
	SphereInfo()
		: isInFrustum( false )
		, isPicked( false )
		, fRadius( 0.0f )
		, vCenter( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) )
	{}
};

#define SAFE_RELEASE(p)			if(p) {(p)->Release(); (p) = NULL;}
#define SAFE_DELETE(p)			if(p) {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p)	if(p) {delete[] (p); (p) = NULL;}
#define SAFE_ADDREF(p)			if(p) {(p)->AddRef();}

#define FLOAT_EPSILON 0.001f
#define FLOAT_TO_INT(f1) static_cast<int>(f1)
#define FLOAT_EQUAL(f1, f2) (fabs(f1, f2) <= FLOAT_EPSILON)

#define GETSET(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define GETSET_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define GETSET_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
		SAFE_RELEASE(varName);\
		SAFE_ADDREF(var);\
		varName = var;\
	}\
}