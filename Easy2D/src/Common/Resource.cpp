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

easy2d::Resource::Resource(int id, const String& type)
    : _id(id)
    , _type(type)
{
}

easy2d::Resource::Data easy2d::Resource::loadData() const
{
    Data data;
    do
    {
        if (data.buffer && data.size)
        {
            break;
        }

        HRSRC res_info = FindResourceA(nullptr, MAKEINTRESOURCEA(_id), _type.c_str());
        if (res_info == nullptr)
        {
            E2D_WARNING("FindResource failed");
            break;
        }

        HGLOBAL res_data = LoadResource(nullptr, res_info);
        if (res_data == nullptr)
        {
            E2D_WARNING("LoadResource failed");
            break;
        }

        DWORD size = SizeofResource(nullptr, res_info);
        if (size == 0)
        {
            E2D_WARNING("SizeofResource failed");
            break;
        }

        LPVOID buffer = LockResource(res_data);
        if (buffer == nullptr)
        {
            E2D_WARNING("LockResource failed");
            break;
        }

        data.buffer = static_cast<void*>(buffer);
        data.size = static_cast<int>(size);
    } while (0);

    return data;
}

int easy2d::Resource::getId() const
{
    return _id;
}

easy2d::String easy2d::Resource::getType() const
{
    return _type;
}
