/******************************************************
* Easy2D Game Core
* 
* Website: https://www.easy2d.cn
* Source Code: https://github.com/Nomango/Easy2D
******************************************************/


#pragma once

#ifndef __cplusplus
	#error ������ C++ ������ʹ�� Easy2D
#endif

#if _MSC_VER < 1600
	#error Easy2D ��֧�� Visual Studio 2010 ���°汾
#endif


#include "emacros.h"
#include "ecommon.h"
#include "ebase.h"
#include "emanagers.h"
#include "enodes.h"
#include "elisteners.h"
#include "etools.h"
#include "eactions.h"
#include "etransitions.h"
#include "egeometry.h"


#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "Easy2Ddw.lib")
#else
	#pragma comment(lib, "Easy2Dw.lib")
#endif


using namespace e2d;