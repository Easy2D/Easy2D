#include <e2dtool.h>

#ifndef E2D_USE_MCI

///////////////////////////////////////////////////////////////////////////////////////////
//
// Music with XAudio2
//
///////////////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "xaudio2.lib")


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif


namespace
{
	IXAudio2* s_pXAudio2 = nullptr;
	IXAudio2MasteringVoice* s_pMasteringVoice = nullptr;
}


inline bool TraceError(wchar_t* sPrompt)
{
	E2D_WARNING(L"MusicInfo error: %s failed!", sPrompt);
	return false;
}

inline bool TraceError(wchar_t* sPrompt, HRESULT hr)
{
	E2D_WARNING(L"MusicInfo error: %s (%#X)", sPrompt, hr);
	return false;
}


easy2d::Music::Music()
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _hmmio(nullptr)
	, _resBuffer(nullptr)
	, _waveData(nullptr)
	, _dwSize(0)
	, _voice(nullptr)
{
}

easy2d::Music::Music(const easy2d::String & filePath)
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _hmmio(nullptr)
	, _resBuffer(nullptr)
	, _waveData(nullptr)
	, _dwSize(0)
	, _voice(nullptr)
{
	this->open(filePath);
}

easy2d::Music::Music(int resNameId, const String & resType)
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _hmmio(nullptr)
	, _resBuffer(nullptr)
	, _waveData(nullptr)
	, _dwSize(0)
	, _voice(nullptr)
{
	this->open(resNameId, resType);
}

easy2d::Music::~Music()
{
	close();
}

bool easy2d::Music::open(const easy2d::String& filePath)
{
	if (_opened)
	{
		E2D_WARNING(L"MusicInfo can be opened only once!");
		return false;
	}

	if (filePath.empty())
	{
		E2D_WARNING(L"MusicInfo::open Invalid file name.");
		return false;
	}

	String actualFilePath = Path::searchForFile(filePath);
	if (actualFilePath.empty())
	{
		E2D_WARNING(L"MusicInfo::open File not found.");
		return false;
	}

	if (!s_pXAudio2)
	{
		E2D_WARNING(L"IXAudio2 nullptr pointer error!");
		return false;
	}

	// 定位 wave 文件
	wchar_t pFilePath[MAX_PATH];
	if (!_findMediaFileCch(pFilePath, MAX_PATH, actualFilePath.c_str()))
	{
		E2D_WARNING(L"Failed to find media file: %s", pFilePath);
		return false;
	}

	_hmmio = mmioOpen(pFilePath, nullptr, MMIO_ALLOCBUF | MMIO_READ);

	if (nullptr == _hmmio)
	{
		return TraceError(L"mmioOpen");
	}

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(_hmmio, 0);
		return TraceError(L"_readMMIO");
	}

	if (!_resetFile())
		return TraceError(L"_resetFile");

	// 重置文件后，wave 文件的大小是 _ck.cksize
	_dwSize = _ck.cksize;

	// 将样本数据读取到内存中
	_waveData = new (std::nothrow) BYTE[_dwSize];

	if (!_read(_waveData, _dwSize))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	// 创建音源
	HRESULT hr;
	if (FAILED(hr = s_pXAudio2->CreateSourceVoice(&_voice, _wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr)))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	_opened = true;
	_playing = false;
	return true;
}

bool easy2d::Music::open(int resNameId, const easy2d::String& resType)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	void* pvRes;

	if (_opened)
	{
		E2D_WARNING(L"MusicInfo can be opened only once!");
		return false;
	}

	if (!s_pXAudio2)
	{
		E2D_WARNING(L"IXAudio2 nullptr pointer error!");
		return false;
	}

	if (nullptr == (hResInfo = FindResourceW(HINST_THISCOMPONENT, MAKEINTRESOURCE(resNameId), resType.c_str())))
		return TraceError(L"FindResource");

	if (nullptr == (hResData = LoadResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"LoadResource");

	if (0 == (dwSize = SizeofResource(HINST_THISCOMPONENT, hResInfo)))
		return TraceError(L"SizeofResource");

	if (nullptr == (pvRes = LockResource(hResData)))
		return TraceError(L"LockResource");

	_resBuffer = new (std::nothrow) CHAR[dwSize];
	memcpy(_resBuffer, pvRes, dwSize);

	MMIOINFO mmioInfo;
	ZeroMemory(&mmioInfo, sizeof(mmioInfo));
	mmioInfo.fccIOProc = FOURCC_MEM;
	mmioInfo.cchBuffer = dwSize;
	mmioInfo.pchBuffer = (CHAR*)_resBuffer;

	_hmmio = mmioOpen(nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);

	if (!_readMMIO())
	{
		// 读取非 wave 文件时 ReadMMIO 调用失败
		mmioClose(_hmmio, 0);
		return TraceError(L"ReadMMIO");
	}

	if (!_resetFile())
		return TraceError(L"ResetFile");

	// 重置文件后，wave 文件的大小是 _ck.cksize
	_dwSize = _ck.cksize;

	// 将样本数据读取到内存中
	_waveData = new (std::nothrow) BYTE[_dwSize];

	if (!_read(_waveData, _dwSize))
	{
		TraceError(L"Failed to read WAV data");
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	// 创建音源
	HRESULT hr;
	if (FAILED(hr = s_pXAudio2->CreateSourceVoice(&_voice, _wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr)))
	{
		TraceError(L"Create source voice error", hr);
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	_opened = true;
	_playing = false;
	return true;
}

bool easy2d::Music::play(int nLoopCount)
{
	if (!_opened)
	{
		E2D_WARNING(L"MusicInfo::play Failed: MusicInfo must be opened first!");
		return false;
	}

	if (_voice == nullptr)
	{
		E2D_WARNING(L"MusicInfo::play Failed: IXAudio2SourceVoice Null pointer exception!");
		return false;
	}

	if (_playing)
	{
		stop();
	}

	nLoopCount = min(nLoopCount, XAUDIO2_LOOP_INFINITE - 1);
	nLoopCount = (nLoopCount < 0) ? XAUDIO2_LOOP_INFINITE : nLoopCount;

	// 提交 wave 样本数据
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = _waveData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = _dwSize;
	buffer.LoopCount = nLoopCount;

	HRESULT hr;
	if (FAILED(hr = _voice->SubmitSourceBuffer(&buffer)))
	{
		TraceError(L"Submitting source buffer error", hr);
		_voice->DestroyVoice();
		SAFE_DELETE_ARRAY(_waveData);
		return false;
	}

	if (SUCCEEDED(hr = _voice->Start(0)))
	{
		// 播放成功，保留这个对象
		_playing = true;
	}

	return SUCCEEDED(hr);
}

void easy2d::Music::pause()
{
	if (_voice)
	{
		if (SUCCEEDED(_voice->Stop()))
		{
			_playing = false;
		}
	}
}

void easy2d::Music::resume()
{
	if (_voice)
	{
		if (SUCCEEDED(_voice->Start()))
		{
			_playing = true;
		}
	}
}

void easy2d::Music::stop()
{
	if (_voice)
	{
		if (SUCCEEDED(_voice->Stop()))
		{
			_voice->ExitLoop();
			_voice->FlushSourceBuffers();
			_playing = false;
		}
	}
}

void easy2d::Music::close()
{
	if (_voice)
	{
		_voice->Stop();
		_voice->FlushSourceBuffers();
		_voice->DestroyVoice();
		_voice = nullptr;
	}

	if (_hmmio != nullptr)
	{
		mmioClose(_hmmio, 0);
		_hmmio = nullptr;
	}

	SAFE_DELETE_ARRAY(_resBuffer);
	SAFE_DELETE_ARRAY(_waveData);
	SAFE_DELETE_ARRAY(_wfx);

	_opened = false;
	_playing = false;
}

bool easy2d::Music::isPlaying() const
{
	if (_opened && _voice)
	{
		XAUDIO2_VOICE_STATE state;
		_voice->GetState(&state);

		if (state.BuffersQueued == 0)
		{
			_playing = false;
		}
		return _playing;
	}
	else
	{
		return false;
	}
}

bool easy2d::Music::setVolume(float volume)
{
	if (_voice)
	{
		return SUCCEEDED(_voice->SetVolume(float(volume)));
	}
	return false;
}

bool easy2d::Music::_readMMIO()
{
	MMCKINFO ckIn;
	PCMWAVEFORMAT pcmWaveFormat;

	memset(&ckIn, 0, sizeof(ckIn));

	_wfx = nullptr;

	if ((0 != mmioDescend(_hmmio, &_ckRiff, nullptr, 0)))
		return TraceError(L"mmioDescend");

	// 确认文件是一个合法的 wave 文件
	if ((_ckRiff.ckid != FOURCC_RIFF) ||
		(_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return TraceError(L"mmioFOURCC");

	// 在输入文件中查找 'fmt' 块
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (0 != mmioDescend(_hmmio, &ckIn, &_ckRiff, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	// 'fmt' 块至少应和 PCMWAVEFORMAT 一样大
	if (ckIn.cksize < (LONG)sizeof(PCMWAVEFORMAT))
		return TraceError(L"sizeof(PCMWAVEFORMAT)");

	// 将 'fmt' 块读取到 pcmWaveFormat 中
	if (mmioRead(_hmmio, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return TraceError(L"mmioRead");

	// 分配 WAVEFORMATEX，但如果它不是 PCM 格式，再读取一个 WORD 大小
	// 的数据，这个数据就是额外分配的大小
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		_wfx = (WAVEFORMATEX*) new (std::nothrow) CHAR[sizeof(WAVEFORMATEX)];

		// 拷贝数据
		memcpy(_wfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		_wfx->cbSize = 0;
	}
	else
	{
		// 读取额外数据的大小
		WORD cbExtraBytes = 0L;
		if (mmioRead(_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return TraceError(L"mmioRead");

		_wfx = (WAVEFORMATEX*) new (std::nothrow) CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];

		// 拷贝数据
		memcpy(_wfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		_wfx->cbSize = cbExtraBytes;

		// 读取额外数据
		if (mmioRead(_hmmio, (CHAR*)(((BYTE*)&(_wfx->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes)
		{
			SAFE_DELETE(_wfx);
			return TraceError(L"mmioRead");
		}
	}

	if (0 != mmioAscend(_hmmio, &ckIn, 0))
	{
		SAFE_DELETE(_wfx);
		return TraceError(L"mmioAscend");
	}

	return true;
}

bool easy2d::Music::_resetFile()
{
	// Seek to the data
	if (-1 == mmioSeek(_hmmio, _ckRiff.dwDataOffset + sizeof(FOURCC),
		SEEK_SET))
		return TraceError(L"mmioSeek");

	// Search the input file for the 'data' chunk.
	_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(_hmmio, &_ck, &_ckRiff, MMIO_FINDCHUNK))
		return TraceError(L"mmioDescend");

	return true;
}

bool easy2d::Music::_read(BYTE* pBuffer, DWORD dwSizeToRead)
{
	MMIOINFO mmioinfoIn; // current status of _hmmio

	if (0 != mmioGetInfo(_hmmio, &mmioinfoIn, 0))
		return TraceError(L"mmioGetInfo");

	UINT cbDataIn = dwSizeToRead;
	if (cbDataIn > _ck.cksize)
		cbDataIn = _ck.cksize;

	_ck.cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; ++cT)
	{
		// Copy the bytes from the io to the buffer.
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			if (0 != mmioAdvance(_hmmio, &mmioinfoIn, MMIO_READ))
				return TraceError(L"mmioAdvance");

			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
				return TraceError(L"mmioinfoIn.pchNext");
		}

		// Actual copy.
		*((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
		++mmioinfoIn.pchNext;
	}

	if (0 != mmioSetInfo(_hmmio, &mmioinfoIn, 0))
		return TraceError(L"mmioSetInfo");

	return true;
}

bool easy2d::Music::_findMediaFileCch(wchar_t* strDestPath, int cchDest, const wchar_t * strFilename)
{
	bool bFound = false;

	if (nullptr == strFilename || nullptr == strDestPath || cchDest < 10)
		return false;

	// Get the exe name, and exe path
	wchar_t strExePath[MAX_PATH] = { 0 };
	wchar_t strExeName[MAX_PATH] = { 0 };
	wchar_t* strLastSlash = nullptr;
	GetModuleFileName(HINST_THISCOMPONENT, strExePath, MAX_PATH);
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr(strExePath, TEXT('\\'));
	if (strLastSlash)
	{
		wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr(strExeName, TEXT('.'));
		if (strLastSlash)
			*strLastSlash = 0;
	}

	wcscpy_s(strDestPath, cchDest, strFilename);
	if (GetFileAttributes(strDestPath) != 0xFFFFFFFF)
		return true;

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	wchar_t strLeafName[MAX_PATH] = { 0 };
	wcscpy_s(strLeafName, MAX_PATH, strFilename);

	wchar_t strFullPath[MAX_PATH] = { 0 };
	wchar_t strFullFileName[MAX_PATH] = { 0 };
	wchar_t strSearch[MAX_PATH] = { 0 };
	wchar_t* strFilePart = nullptr;

	GetFullPathName(L".", MAX_PATH, strFullPath, &strFilePart);
	if (strFilePart == nullptr)
		return false;

	while (strFilePart != nullptr && *strFilePart != '\0')
	{
		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strDestPath, cchDest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName);
		if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
		{
			wcscpy_s(strDestPath, cchDest, strFullFileName);
			bFound = true;
			break;
		}

		swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
		GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
	}
	if (bFound)
		return true;

	// 失败时，将文件作为路径返回，同时也返回错误代码
	wcscpy_s(strDestPath, cchDest, strFilename);

	return false;
}

bool easy2d::Music::__init()
{
	HRESULT hr;

	if (FAILED(hr = XAudio2Create(&s_pXAudio2, 0)))
	{
		E2D_WARNING(L"Failed to init XAudio2 engine");
		return false;
	}

	if (FAILED(hr = s_pXAudio2->CreateMasteringVoice(&s_pMasteringVoice)))
	{
		E2D_WARNING(L"Failed creating mastering voice");
		easy2d::SafeRelease(s_pXAudio2);
		return false;
	}

	return true;
}

void easy2d::Music::__uninit()
{
	if (s_pMasteringVoice)
	{
		s_pMasteringVoice->DestroyVoice();
	}

	easy2d::SafeRelease(s_pXAudio2);
}


#else


///////////////////////////////////////////////////////////////////////////////////////////
//
// Music with MCI
//
///////////////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib , "winmm.lib")

#define WIN_CLASS_NAME L"Easy2DMciCallbackWnd"

namespace
{
	HINSTANCE s_hInstance = nullptr;
}

easy2d::Music::Music()
	: _wnd(NULL)
	, _dev(0L)
	, _playing(false)
	, _repeatTimes(0)
{
	_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		WIN_CLASS_NAME,
		NULL,
		WS_POPUPWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		s_hInstance,
		NULL);

	if (_wnd)
	{
		SetWindowLongPtr(_wnd, GWLP_USERDATA, (LONG_PTR)this);
	}
}

easy2d::Music::~Music()
{
	close();
	DestroyWindow(_wnd);
}

bool easy2d::Music::open(const String& pFileName)
{
	if (pFileName.empty())
		return false;

	close();

	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrDeviceType = 0;
	mciOpen.lpstrElementName = pFileName.c_str();

	MCIERROR mciError;
	mciError = mciSendCommand(
		0,
		MCI_OPEN,
		MCI_OPEN_ELEMENT,
		reinterpret_cast<DWORD_PTR>(&mciOpen)
	);

	if (mciError == 0)
	{
		_dev = mciOpen.wDeviceID;
		_playing = false;
		return true;
	}
	return false;
}

bool easy2d::Music::open(int resNameId, const String& resType)
{
	// NOT SUPPORTED
	return false;
}

bool easy2d::Music::play(int nLoopCount)
{
	if (!_dev)
	{
		return false;
	}

	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(_wnd);

	// 播放声音
	MCIERROR mciError = mciSendCommand(
		_dev,
		MCI_PLAY,
		MCI_FROM | MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);

	if (!mciError)
	{
		_playing = true;
		_repeatTimes = nLoopCount;
		return true;
	}
	return false;
}

void easy2d::Music::close()
{
	if (_playing)
	{
		stop();
	}

	if (_dev)
	{
		_sendCommand(MCI_CLOSE);
	}

	_dev = 0;
	_playing = false;
}

void easy2d::Music::pause()
{
	_sendCommand(MCI_PAUSE);
	_playing = false;
}

void easy2d::Music::resume()
{
	_sendCommand(MCI_RESUME);
	_playing = true;
}

void easy2d::Music::stop()
{
	_sendCommand(MCI_STOP);
	_playing = false;
}

bool easy2d::Music::isPlaying() const
{
	return _playing;
}

bool easy2d::Music::setVolume(float volume)
{
	// NOT SUPPORTED
	return false;
}

void easy2d::Music::_sendCommand(int nCommand, DWORD_PTR param1, DWORD_PTR parma2)
{
	// 空设备时忽略这次操作
	if (!_dev)
	{
		return;
	}
	// 向当前设备发送操作
	mciSendCommand(_dev, nCommand, param1, parma2);
}

LRESULT WINAPI easy2d::Music::_MciProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	easy2d::Music* pMusic = NULL;

	if (Msg == MM_MCINOTIFY
		&& wParam == MCI_NOTIFY_SUCCESSFUL
		&& (pMusic = (easy2d::Music*)GetWindowLongPtr(hWnd, GWLP_USERDATA)))
	{
		if (pMusic->_repeatTimes > 0)
		{
			pMusic->_repeatTimes--;
		}

		if (pMusic->_repeatTimes)
		{
			mciSendCommand(static_cast<MCIDEVICEID>(lParam), MCI_SEEK, MCI_SEEK_TO_START, 0);

			MCI_PLAY_PARMS mciPlay = { 0 };
			mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(hWnd);
			mciSendCommand(static_cast<MCIDEVICEID>(lParam), MCI_PLAY, MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay));
		}
		else
		{
			pMusic->_playing = false;
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

bool easy2d::Music::__init()
{
	s_hInstance = HINST_THISCOMPONENT;

	WNDCLASS  wc;
	wc.style = 0;
	wc.lpfnWndProc = _MciProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = s_hInstance;
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WIN_CLASS_NAME;

	if (!RegisterClass(&wc) && 1410 != GetLastError())
	{
		return false;
	}
	return true;
}

void easy2d::Music::__uninit()
{
}


#endif