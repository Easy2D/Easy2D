/******************************************************
* Easy2D Game Framework
* 
* Website: https://www.easy2d.cn
* Source Code: https://github.com/Nomango/Easy2D
******************************************************/


#pragma once

#ifndef __cplusplus
#	error 仅能在 C++ 环境下使用 Easy2D
#endif

#if _MSC_VER < 1700
#	error Easy2D 不支持 Visual Studio 2012 以下版本
#endif


#include <easy2d/e2dmacros.h>
#include <easy2d/e2dmath.h>
#include <easy2d/e2dcommon.h>
#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>
#include <easy2d/e2dnode.h>
#include <easy2d/e2dshape.h>
#include <easy2d/e2dtool.h>
#include <easy2d/e2daction.h>
#include <easy2d/e2dtransition.h>


#if defined(DEBUG) || defined(_DEBUG)
#	pragma comment(lib, "libeasy2dd.lib")
#else
#	pragma comment(lib, "libeasy2d.lib")
#endif
