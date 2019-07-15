#include <e2dtool.h>


void easy2d::Data::saveInt(const String& key, int value, const String& field)
{
	::WritePrivateProfileString(field.c_str(), key.c_str(), std::to_wstring(value).c_str(), Path::getDataSavePath().c_str());
}

void easy2d::Data::saveDouble(const String& key, float value, const String& field)
{
	::WritePrivateProfileString(field.c_str(), key.c_str(), std::to_wstring(value).c_str(), Path::getDataSavePath().c_str());
}

void easy2d::Data::saveBool(const String& key, bool value, const String& field)
{
	::WritePrivateProfileString(field.c_str(), key.c_str(), (value ? L"1" : L"0"), Path::getDataSavePath().c_str());
}

void easy2d::Data::saveString(const String& key, const String& value, const String& field)
{
	::WritePrivateProfileString(field.c_str(), key.c_str(), value.c_str(), Path::getDataSavePath().c_str());
}

int easy2d::Data::getInt(const String& key, int defaultValue, const String& field)
{
	return ::GetPrivateProfileInt(field.c_str(), key.c_str(), defaultValue, Path::getDataSavePath().c_str());
}

float easy2d::Data::getDouble(const String& key, float defaultValue, const String& field)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(field.c_str(), key.c_str(), std::to_wstring(defaultValue).c_str(), temp, 31, Path::getDataSavePath().c_str());
	return std::stof(temp);
}

bool easy2d::Data::getBool(const String& key, bool defaultValue, const String& field)
{
	int nDefaultValue = defaultValue ? 1 : 0;
	int nValue = ::GetPrivateProfileInt(field.c_str(), key.c_str(), nDefaultValue, Path::getDataSavePath().c_str());
	return nValue != 0;
}

easy2d::String easy2d::Data::getString(const String& key, const String& defaultValue, const String& field)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(field.c_str(), key.c_str(), defaultValue.c_str(), temp, 255, Path::getDataSavePath().c_str());
	return temp;
}