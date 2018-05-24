#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{


template<class Interface>
inline void SafeRelease(Interface*& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}


class Music;

// ��Դ�ص�
class VoiceCallback 
	: public IXAudio2VoiceCallback
{
public:
	VoiceCallback(Music * music);
	~VoiceCallback();

	void __stdcall OnStreamEnd();

	void __stdcall OnBufferEnd(
		void * pBufferContext
	);

	void __stdcall OnBufferStart(
		void * pBufferContext
	);

	void __stdcall OnLoopEnd(
		void * pBufferContext
	);

	void __stdcall OnVoiceProcessingPassEnd();

	void __stdcall OnVoiceProcessingPassStart(
		UINT32 SamplesRequired
	);

	void __stdcall OnVoiceError(
		void * pBufferContext,
		HRESULT Error
	);

	void SetFuncOnStreamEnd(
		const Function& func
	);

	void SetFuncOnLoopEnd(
		const Function& func
	);

protected:
	Music * _music;
	Function _loopEndFunc;
	Function _streamEndFunc;
};


// ������Ⱦ��
class TextRenderer 
	: public IDWriteTextRenderer
{
private:
	TextRenderer();

	~TextRenderer();

public:
	static TextRenderer * Create(
		ID2D1Factory* pD2DFactory,
		ID2D1HwndRenderTarget* pRT,
		ID2D1SolidColorBrush* pBrush
	);

	STDMETHOD_(void, SetTextStyle)(
		CONST D2D1_COLOR_F &fillColor,
		BOOL hasOutline,
		CONST D2D1_COLOR_F &outlineColor,
		FLOAT outlineWidth,
		D2D1_LINE_JOIN outlineJoin
	);

	STDMETHOD(DrawGlyphRun)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		__in DWRITE_GLYPH_RUN const* glyphRun,
		__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
	);

	STDMETHOD(DrawUnderline)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
	);

	STDMETHOD(DrawStrikethrough)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
	);

	STDMETHOD(DrawInlineObject)(
		__maybenull void* clientDrawingContext,
		FLOAT originX,
		FLOAT originY,
		IDWriteInlineObject* inlineObject,
		BOOL isSideways,
		BOOL isRightToLeft,
		IUnknown* clientDrawingEffect
	);

	STDMETHOD(IsPixelSnappingDisabled)(
		__maybenull void* clientDrawingContext,
		__out BOOL* isDisabled
	);

	STDMETHOD(GetCurrentTransform)(
		__maybenull void* clientDrawingContext,
		__out DWRITE_MATRIX* transform
	);

	STDMETHOD(GetPixelsPerDip)(
		__maybenull void* clientDrawingContext,
		__out FLOAT* pixelsPerDip
	);

public:
	unsigned long STDMETHODCALLTYPE AddRef();
	unsigned long STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(
		IID const& riid,
		void** ppvObject
	);

private:
	unsigned long cRefCount_;
	D2D1_COLOR_F sFillColor_;
	D2D1_COLOR_F sOutlineColor_;
	FLOAT fOutlineWidth;
	BOOL bShowOutline_;
	ID2D1Factory* pD2DFactory_;
	ID2D1HwndRenderTarget* pRT_;
	ID2D1SolidColorBrush* pBrush_;
	ID2D1StrokeStyle * pCurrStrokeStyle_;
};


// �쳣
class Exception
{
public:
	Exception() throw();

	explicit Exception(const String& message) throw();

	Exception(Exception const& other) throw();

	virtual ~Exception() throw();

	Exception& operator=(Exception const& other) throw();

	// ��ȡ�쳣��Ϣ
	virtual String msg() const;

private:
	String _message;
};


// ϵͳ�쳣
class SystemException
	: public Exception
{
public:
	SystemException() throw();

	explicit SystemException(const String& message) throw();
};

}