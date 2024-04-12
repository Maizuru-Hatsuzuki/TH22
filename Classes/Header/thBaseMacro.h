/********************************************************
* Filename		: KBaseMacro.h
* Creator		: lidiankai
* Date time		: 2023.11.14
* Description	:
********************************************************/

#pragma warning(disable : 26438)
#pragma warning(disable : 26448)
#pragma warning(disable : 26485)

#ifndef __KBASEMACRO_H__
#define __KBASEMACRO_H__

#define ASSERT(Condition)	if (!(Condition)) __debugbreak()
#define TH_MALLOC(ptrType)	(ptrType*)malloc(sizeof(ptrType))
#define TH_RELEASE(p)		{ if (p) { free(p); (p) = NULL; } }

#define TH_PROCESS_SUCCESS(Condition)		\
do											\
{											\
	if (Condition)							\
	{										\
		printf("[TH_PROCESS_SUCCESS] ERR '%s' at line %d in %s::%s.\n", #Condition, __LINE__, __FILE__, __FUNCTION__);	\
		goto Exit0;							\
	}										\
} while (0);

#define TH_PROCESS_ERROR(Condition)			\
do											\
{											\
	if (!(Condition))						\
	{										\
		printf("[TH_PROCESS_ERROR] ERR '%s' at line %d in %s::%s.\n", #Condition, __LINE__, __FILE__, __FUNCTION__);	\
		goto Exit0;							\
	}										\
} while (0);



#endif // !__KBASEMACRO_H__
