#include <easy2d/e2dcommon.h>

easy2d::ByteString easy2d::FormatString(const char* format, ...)
{
    easy2d::ByteString result;
    if (format)
    {
        va_list args = nullptr;
        va_start(args, format);

        const auto len = static_cast<size_t>(::_vscprintf(format, args) + 1);
        if (len)
        {
            result.resize(len - 1);
            ::_vsnprintf_s(&result[0], len, len, format, args);
        }
        va_end(args);
    }
    return result;
}

easy2d::WideString easy2d::FormatString(const wchar_t* format, ...)
{
    easy2d::WideString result;
    if (format)
    {
        va_list args = nullptr;
        va_start(args, format);

        const auto len = static_cast<size_t>(::_vscwprintf(format, args) + 1);
        if (len)
        {
            result.resize(len - 1);
            ::_vsnwprintf_s(&result[0], len, len, format, args);
        }
        va_end(args);
    }
    return result;
}

easy2d::ByteString easy2d::WideToNarrow(const easy2d::WideString& str)
{
    if (str.empty())
        return easy2d::ByteString();

    int num = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    if (num > 0)
    {
        easy2d::ByteString result;
        result.resize(num - 1);

        // C++11 ��֤���ַ����ǿս�β��
        ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &result[0], num, NULL, NULL);
        return result;
    }
    return easy2d::ByteString();
}

easy2d::WideString easy2d::NarrowToWide(const easy2d::ByteString& str)
{
    if (str.empty())
        return easy2d::WideString();

    int num = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    if (num > 0)
    {
        easy2d::WideString result;
        result.resize(num - 1);

        // C++11 ��֤���ַ����ǿս�β��
        ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &result[0], num);
        return result;
    }
    return easy2d::WideString();
}
