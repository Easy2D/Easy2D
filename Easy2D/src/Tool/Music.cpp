#include <easy2d/e2dtool.h>

#ifndef E2D_USE_MCI

///////////////////////////////////////////////////////////////////////////////////////////
//
// Music with XAudio2
//
///////////////////////////////////////////////////////////////////////////////////////////

#include <shlwapi.h>

#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "Shlwapi.lib")


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
	E2D_WARNING(L"%s!", sPrompt);
	return false;
}

inline bool TraceError(wchar_t* sPrompt, HRESULT hr)
{
	E2D_WARNING(L"%s (%#X)", sPrompt, hr);
	return false;
}


easy2d::Music::Music()
	: _opened(false)
	, _playing(false)
	, _wfx(nullptr)
	, _waveData(nullptr)
	, _waveDataSize(0)
	, _voice(nullptr)
{
}

easy2d::Music::Music(const easy2d::String & filePath)
	: Music()
{
	this->open(filePath);
}

easy2d::Music::Music(int resNameId, const String & resType)
	: Music()
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

	if (FAILED(_loadMediaFile(actualFilePath)))
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

	if (FAILED(_loadMediaResource(pvRes, dwSize)))
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
	buffer.AudioBytes = _waveDataSize;
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

	if (_wfx)
	{
		::CoTaskMemFree(_wfx);
		_wfx = nullptr;
	}

	SAFE_DELETE_ARRAY(_waveData);

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

HRESULT easy2d::Music::_loadMediaFile(String const& file_path)
{
	HRESULT hr = S_OK;

	IMFSourceReader* reader = nullptr;

	hr = MFCreateSourceReaderFromURL(
		file_path.c_str(),
		nullptr,
		&reader
	);

	if (SUCCEEDED(hr))
	{
		hr = _readSource(reader);
	}

	SafeRelease(reader);
	return hr;
}

HRESULT easy2d::Music::_loadMediaResource(LPVOID buffer, DWORD bufferSize)
{
	HRESULT	hr = S_OK;

	IStream* stream = nullptr;
	IMFByteStream* byte_stream = nullptr;
	IMFSourceReader* reader = nullptr;

	stream = SHCreateMemStream(
		static_cast<const BYTE*>(buffer),
		static_cast<UINT32>(bufferSize)
	);

	if (stream == nullptr)
	{
		E2D_WARNING(L"SHCreateMemStream failed");
		return E_OUTOFMEMORY;
	}

	if (SUCCEEDED(hr))
	{
		hr = MFCreateMFByteStreamOnStream(stream, &byte_stream);
	}

	if (SUCCEEDED(hr))
	{
		hr = MFCreateSourceReaderFromByteStream(
			byte_stream,
			nullptr,
			&reader
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = _readSource(reader);
	}

	SafeRelease(stream);
	SafeRelease(byte_stream);
	SafeRelease(reader);
	return hr;
}

HRESULT easy2d::Music::_readSource(IMFSourceReader* reader)
{
	HRESULT hr = S_OK;
	DWORD max_stream_size = 0;

	IMFMediaType* partial_type = nullptr;
	IMFMediaType* uncompressed_type = nullptr;

	hr = MFCreateMediaType(&partial_type);

	if (SUCCEEDED(hr))
	{
		hr = partial_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	}

	if (SUCCEEDED(hr))
	{
		hr = partial_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	}

	// 设置 source reader 的媒体类型，它将使用合适的解码器去解码这个音频
	if (SUCCEEDED(hr))
	{
		hr = reader->SetCurrentMediaType(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0,
			partial_type
		);
	}

	// 从 IMFMediaType 中获取 WAVEFORMAT 结构
	if (SUCCEEDED(hr))
	{
		hr = reader->GetCurrentMediaType(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			&uncompressed_type
		);
	}

	// 指定音频流
	if (SUCCEEDED(hr))
	{
		hr = reader->SetStreamSelection(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			true
		);
	}

	// 获取 WAVEFORMAT 数据
	if (SUCCEEDED(hr))
	{
		UINT32 size = 0;
		hr = MFCreateWaveFormatExFromMFMediaType(
			uncompressed_type,
			&_wfx,
			&size,
			(DWORD)MFWaveFormatExConvertFlag_Normal
		);
	}

	// 估算音频流大小
	if (SUCCEEDED(hr))
	{
		PROPVARIANT prop;
		PropVariantInit(&prop);

		hr = reader->GetPresentationAttribute(
			(DWORD)MF_SOURCE_READER_MEDIASOURCE,
			MF_PD_DURATION,
			&prop
		);

		LONGLONG duration = prop.uhVal.QuadPart;
		max_stream_size = static_cast<DWORD>(
			(duration * _wfx->nAvgBytesPerSec) / 10000000 + 1
			);
		PropVariantClear(&prop);
	}

	// 读取音频数据
	if (SUCCEEDED(hr))
	{
		DWORD flags = 0;
		DWORD position = 0;
		BYTE* data = new (std::nothrow) BYTE[max_stream_size];

		IMFSample* sample = nullptr;
		IMFMediaBuffer* buffer = nullptr;

		if (data == nullptr)
		{
			E2D_WARNING(L"Low memory");
			hr = E_OUTOFMEMORY;
		}
		else
		{
			while (true)
			{
				hr = reader->ReadSample(
					(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
					0,
					nullptr,
					&flags,
					nullptr,
					&sample
				);

				if (flags & MF_SOURCE_READERF_ENDOFSTREAM) { break; }

				if (sample == nullptr) { continue; }

				if (SUCCEEDED(hr))
				{
					hr = sample->ConvertToContiguousBuffer(&buffer);

					if (SUCCEEDED(hr))
					{
						BYTE* audio_data = nullptr;
						DWORD sample_buffer_length = 0;

						hr = buffer->Lock(
							&audio_data,
							nullptr,
							&sample_buffer_length
						);

						if (SUCCEEDED(hr) && sample_buffer_length <= max_stream_size)
						{
							for (DWORD i = 0; i < sample_buffer_length; i++)
							{
								data[position++] = audio_data[i];
							}
							hr = buffer->Unlock();
						}
					}
					buffer = nullptr;
				}
				sample = nullptr;

				if (FAILED(hr)) { break; }
			}

			if (SUCCEEDED(hr))
			{
				_waveData = data;
				_waveDataSize = position;
			}
			else
			{
				delete[] data;
				data = nullptr;
			}
		}

		SafeRelease(sample);
		SafeRelease(buffer);
	}

	SafeRelease(partial_type);
	SafeRelease(uncompressed_type);
	return hr;
}

bool easy2d::Music::__init()
{
	HRESULT hr;

	if (FAILED(hr = MFStartup(MF_VERSION, MFSTARTUP_FULL)))
	{
		TraceError(L"Failed to startup MediaFoundation device", hr);
		return false;
	}

	if (FAILED(hr = XAudio2Create(&s_pXAudio2, 0)))
	{
		TraceError(L"Failed to init XAudio2 engine", hr);
		return false;
	}

	if (FAILED(hr = s_pXAudio2->CreateMasteringVoice(&s_pMasteringVoice)))
	{
		TraceError(L"Failed to create mastering voice", hr);
		SafeRelease(s_pXAudio2);
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

	SafeRelease(s_pXAudio2);

	MFShutdown();
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