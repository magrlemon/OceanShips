#ifndef __METHOD_ID_DEFINE__H__
#define __METHOD_ID_DEFINE__H__

union RetVar
{
	int			 iRet;
	short	 int sRet;
	unsigned int uRet;
	double		 dRet;
	float		 fRet;
	long long    llRet;
	bool	     bRet;
	char		 cRet;
	char *		 pRet;
};
#define  GET_ARRAY_LEN(arr,len){ (len) = sizeof((arr))/sizeof((arr)[0]);}
enum ENUM_METHOD_ID
{
	ENUM_METHOD_MIN = 0,
	ENUM_METHOD_START,
	ENUM_METHOD_END,
	ENUM_METHOD_ENTITY_MOVE,
	ENUM_METHOD_ENTITY_MOVE_BACK,
	ENUM_METHOD_ENTITY_FIRE,
	ENUM_METHOD_MAX 
};

#endif // !__METHOD_ID_DEFINE__H__

