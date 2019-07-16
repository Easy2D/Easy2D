/******************************************************
* Easy2D Game Framework
* 
* Website: https://www.easy2d.cn
* Source Code: https://github.com/Nomango/Easy2D
******************************************************/


#pragma once

#ifndef __cplusplus
#	error ������ C++ ������ʹ�� Easy2D
#endif

#if _MSC_VER < 1700
#	error Easy2D ��֧�� Visual Studio 2012 ���°汾
#endif


#include <e2dmacros.h>
#include <e2dmath.h>
#include <e2dcommon.h>
#include <e2dbase.h>
#include <e2dmanager.h>
#include <e2dnode.h>
#include <e2dshape.h>
#include <e2dtool.h>
#include <e2daction.h>
#include <e2dtransition.h>


#if defined(DEBUG) || defined(_DEBUG)
#	pragma comment(lib, "libeasy2dd.lib")
#else
#	pragma comment(lib, "libeasy2d.lib")
#endif
