#include "..\e2dcustom.h"

e2d::SystemException::SystemException() throw()
	: Exception(L"δ֪��ϵͳ�쳣")
{
}

e2d::SystemException::SystemException(const String& message) throw()
	: Exception(message)
{
}

