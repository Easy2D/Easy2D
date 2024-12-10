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

// ����
enum class Direction : int
{
	Up,			/* �� */
	Down,		/* �� */
	Left,		/* �� */
	Right		/* �� */
};


// խ�ַ���
using ByteString = std::string;

// ���ַ���
using WideString = std::wstring;

// �ַ���
using String = ByteString;


// ��������
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


// ��ʽ���ַ���
ByteString FormatString(const char* format, ...);

// ��ʽ���ַ���
WideString FormatString(const wchar_t* format, ...);

// ���ַ���תխ�ַ���
ByteString WideToNarrow(const WideString& str);

// խ�ַ���ת���ַ���
WideString NarrowToWide(const ByteString& str);


// ��������
class Object
{
public:
	Object();

	virtual ~Object();

	// �Զ��ͷ�
	void autorelease();

	// ���ü�����һ
	void retain();

	// ���ü�����һ
	void release();

	// ��ȡ���ü���
	int getRefCount() const;

	// ��ȡ����
	String getName() const;

	// ��������
	void setName(
		const String& name
	);

	// �����Ƿ���ͬ
	bool isName(
		const String& name,
		size_t hashName = 0
	) const;

private:
	int _refCount;
	String* _name;
	size_t _hashName;
};


// ��Դ
class Resource
{
public:
	// ��Դ�Ķ���������
	struct Data
	{
		void* buffer;	// ��Դ����
		int size;	// ��Դ���ݴ�С

		Data();

		bool isValid() const;

		template <typename Elem>
		friend std::basic_ostream<Elem>& operator<<(std::basic_ostream<Elem>& out, const Resource::Data& data)
		{
			using OStreamType = std::basic_ostream<Elem>;

			typename OStreamType::iostate state = OStreamType::goodbit;
			const typename OStreamType::sentry ok(out);
			if (!ok)
			{
				state |= OStreamType::badbit;
			}
			else
			{
				if (data.buffer && data.size)
				{
					out.write(reinterpret_cast<const Elem*>(data.buffer), static_cast<std::streamsize>(data.size));
				}
				else
				{
					state |= OStreamType::badbit;
				}
			}
			out.setstate(state);
			return out;
		}
	};

	Resource(
		int id,				/* ��Դ ID */
		const String& type	/* ��Դ���� */
	);

	// ������Դ�Ķ���������
	Data loadData() const;

	// ��ȡ��Դ ID
	int getId() const;

	// ��ȡ��Դ����
	String getType() const;

	bool operator==(const Resource& other) const { return _id == other._id && _type == other._type; }

	bool operator<(const Resource& other) const { return _id < other._id || _type < other._type; }

private:
	int		_id;
	String	_type;
};

}

namespace std
{
	template<>
	struct hash<easy2d::Resource>
	{
		size_t operator()(const easy2d::Resource& res) const
		{
			return static_cast<size_t>(res.getId());
		}
	};
}
