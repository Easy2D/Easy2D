#include <easy2d/e2dbase.h>
#include <iostream>
#include <fstream>

namespace
{
	bool s_bEnable = true;

	std::streambuf* s_cinBuffer, * s_coutBuffer, * s_cerrBuffer;
	std::fstream s_consoleInput, s_consoleOutput, s_consoleError;

	std::wstreambuf* s_wcinBuffer, * s_wcoutBuffer, * s_wcerrBuffer;
	std::wfstream s_wconsoleInput, s_wconsoleOutput, s_wconsoleError;
}

namespace 
{
	void Output(std::wostream& os, const wchar_t* prompt, const wchar_t* format, va_list args)
	{
		if (s_bEnable)
		{
			static wchar_t tempBuffer[1024 * 3 + 1];

			std::wstringstream ss;

			if (prompt)
				ss << prompt;

			if (format)
			{
				const auto len = ::_vscwprintf(format, args) + 1;
				::_vsnwprintf_s(tempBuffer, len, len, format, args);

				ss << tempBuffer << std::endl;
			}

			std::wstring output = ss.str();
			os << output << std::flush;
			::OutputDebugStringW(output.c_str());
		}
	}

	void RedirectStdIO()
	{
		s_cinBuffer = std::cin.rdbuf();
		s_coutBuffer = std::cout.rdbuf();
		s_cerrBuffer = std::cerr.rdbuf();
		s_wcinBuffer = std::wcin.rdbuf();
		s_wcoutBuffer = std::wcout.rdbuf();
		s_wcerrBuffer = std::wcerr.rdbuf();

		s_consoleInput.open("CONIN$", std::ios::in);
		s_consoleOutput.open("CONOUT$", std::ios::out);
		s_consoleError.open("CONOUT$", std::ios::out);
		s_wconsoleInput.open("CONIN$", std::ios::in);
		s_wconsoleOutput.open("CONOUT$", std::ios::out);
		s_wconsoleError.open("CONOUT$", std::ios::out);

		FILE* dummy;
		::freopen_s(&dummy, "CONOUT$", "w+t", stdout);
		::freopen_s(&dummy, "CONIN$", "r+t", stdin);
		::freopen_s(&dummy, "CONOUT$", "w+t", stderr);
		(void)dummy;

		std::cin.rdbuf(s_consoleInput.rdbuf());
		std::cout.rdbuf(s_consoleOutput.rdbuf());
		std::cerr.rdbuf(s_consoleError.rdbuf());
		std::wcin.rdbuf(s_wconsoleInput.rdbuf());
		std::wcout.rdbuf(s_wconsoleOutput.rdbuf());
		std::wcerr.rdbuf(s_wconsoleError.rdbuf());
	}

	void ResetStdIO()
	{
		s_consoleInput.close();
		s_consoleOutput.close();
		s_consoleError.close();
		s_wconsoleInput.close();
		s_wconsoleOutput.close();
		s_wconsoleError.close();

		std::cin.rdbuf(s_cinBuffer);
		std::cout.rdbuf(s_coutBuffer);
		std::cerr.rdbuf(s_cerrBuffer);
		std::wcin.rdbuf(s_wcinBuffer);
		std::wcout.rdbuf(s_wcoutBuffer);
		std::wcerr.rdbuf(s_wcerrBuffer);

		fclose(stdout);
		fclose(stdin);
		fclose(stderr);

		s_cinBuffer = nullptr;
		s_coutBuffer = nullptr;
		s_cerrBuffer = nullptr;
		s_wcinBuffer = nullptr;
		s_wcoutBuffer = nullptr;
		s_wcerrBuffer = nullptr;
	}

	HWND allocated_console = nullptr;

	HWND AllocateConsole()
	{
		if (::AllocConsole())
		{
			allocated_console = ::GetConsoleWindow();

			if (allocated_console)
			{
				RedirectStdIO();
			}
		}
		return allocated_console;
	}

	void FreeAllocatedConsole()
	{
		if (allocated_console)
		{
			ResetStdIO();
			::FreeConsole();
			allocated_console = nullptr;
		}
	}

	HWND GetAllocatedConsole()
	{
		return allocated_console;
	}
}

void easy2d::Logger::enable()
{
	s_bEnable = true;
}

void easy2d::Logger::disable()
{
	s_bEnable = false;
}

void easy2d::Logger::messageln(String format, ...)
{
	va_list args = nullptr;
	va_start(args, format);

	Output(std::wcout, L" ", format.c_str(), args);

	va_end(args);
}

void easy2d::Logger::warningln(String format, ...)
{
	va_list args = nullptr;
	va_start(args, format);

	Output(std::wcout, L"Warning: ", format.c_str(), args);

	va_end(args);
}

void easy2d::Logger::errorln(String format, ...)
{
	va_list args = nullptr;
	va_start(args, format);

	Output(std::wcout, L"Error: ", format.c_str(), args);

	va_end(args);
}

void easy2d::Logger::showConsole(bool show)
{
	HWND currConsole = ::GetConsoleWindow();
	if (show)
	{
		if (currConsole)
		{
			::ShowWindow(currConsole, SW_SHOW);
		}
		else
		{
			HWND console = ::AllocateConsole();
			if (!console)
			{
				E2D_WARNING(L"AllocConsole failed");
			}
			else
			{
				// disable the close button of console
				HMENU hmenu = ::GetSystemMenu(console, FALSE);
				::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			}
		}
	}
	else
	{
		if (currConsole)
		{
			if (currConsole == GetAllocatedConsole())
			{
				FreeAllocatedConsole();
			}
			else
			{
				::ShowWindow(currConsole, SW_HIDE);
			}
		}
	}
}
