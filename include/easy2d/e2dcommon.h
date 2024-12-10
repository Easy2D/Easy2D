#pragma once

#ifndef WINVER
#	define WINVER 0x0700       // Allow use of features specific to Windows 7 or later
#endif

#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0700 // Allow use of features specific to Windows 7 or later
#endif

#ifndef NTDDI_VERSION
#	define NTDDI_VERSION NTDDI_WIN7
#endif

#ifndef UNICODE
#	define UNICODE
#endif

// Exclude rarely-used items from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECTINPUT_VERSION
#	define DIRECTINPUT_VERSION 0x0800
#endif

#define INITGUID

// Windows Header Files
#include <windows.h>
#include <wincodec.h>
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d2d1helper.h>

// Import Libraries
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "winmm.lib")


#ifndef HINST_THISCOMPONENT
	EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#	define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#if !defined(E2D_DEBUG) && defined(_DEBUG)
#	define E2D_DEBUG
#endif


#include <easy2d/e2dmath.h>
#include <functional> // std::function, std::hash
#include <ostream> // std::basic_ostream
#include <string>

namespace easy2d
{

// 方向
enum class Direction : int
{
	Up,			/* 上 */
	Down,		/* 下 */
	Left,		/* 左 */
	Right		/* 右 */
};


// 窄字符串
using ByteString = std::string;

// 宽字符串
using WideString = std::wstring;

// 字符串
using String = ByteString;


// 函数对象
template <typename _Fty>
using Function = std::function<_Fty>;


template<class Interface>
inline void SafeRelease(Interface*& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}


// 格式化字符串
ByteString FormatString(const char* format, ...);

// 格式化字符串
WideString FormatString(const wchar_t* format, ...);

// 宽字符串转窄字符串
ByteString WideToNarrow(const WideString& str);

// 窄字符串转宽字符串
WideString NarrowToWide(const ByteString& str);


// 基础对象
class Object
{
public:
	Object();

	virtual ~Object();

	// 自动释放
	void autorelease();

	// 引用计数加一
	void retain();

	// 引用计数减一
	void release();

	// 获取引用计数
	int getRefCount() const;

	// 获取名称
	String getName() const;

	// 设置名称
	void setName(
		const String& name
	);

	// 名称是否相同
	bool isName(
		const String& name,
		size_t hashName = 0
	) const;

private:
	int _refCount;
	String* _name;
	size_t _hashName;
};

}
