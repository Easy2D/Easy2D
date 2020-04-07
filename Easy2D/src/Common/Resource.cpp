#include <easy2d/e2dcommon.h>
#include <easy2d/e2dbase.h>

easy2d::Resource::Data::Data()
	: buffer(nullptr)
	, size(0)
{
}

bool easy2d::Resource::Data::isValid() const
{
	return buffer != nullptr && size;
}

easy2d::Resource::Resource()
    : _id(0)
    , _type()
{
}

easy2d::Resource::Resource(int id, const String& type)
    : _id(id)
    , _type(type)
{
}

easy2d::Resource::Data easy2d::Resource::loadData() const
{
    do
    {
        if (_data.buffer && _data.size)
        {
            break;
        }

        HRSRC res_info = FindResourceW(nullptr, MAKEINTRESOURCE(_id), _type.c_str());
        if (res_info == nullptr)
        {
            E2D_WARNING(L"FindResource failed");
            break;
        }

        HGLOBAL res_data = LoadResource(nullptr, res_info);
        if (res_data == nullptr)
        {
            E2D_WARNING(L"LoadResource failed");
            break;
        }

        DWORD size = SizeofResource(nullptr, res_info);
        if (size == 0)
        {
            E2D_WARNING(L"SizeofResource failed");
            break;
        }

        LPVOID buffer = LockResource(res_data);
        if (buffer == nullptr)
        {
            E2D_WARNING(L"LockResource failed");
            break;
        }

        _data.buffer = static_cast<void*>(buffer);
        _data.size = static_cast<uint32_t>(size);
    } while (0);

    return _data;
}

int easy2d::Resource::getId() const
{
    return _id;
}

easy2d::String easy2d::Resource::getType() const
{
    return _type;
}
