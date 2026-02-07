#include <easy2d/utils/data.h>
#include <simpleini/SimpleIni.h>

namespace easy2d {

class DataStore::Impl {
public:
    CSimpleIniA ini;
};

DataStore::DataStore()
    : impl_(makeUnique<Impl>())
{
}

DataStore::~DataStore() = default;

bool DataStore::load(const std::string& filename) {
    filename_ = filename;
    SI_Error rc = impl_->ini.LoadFile(filename.c_str());
    return rc >= 0;
}

bool DataStore::save(const std::string& filename) {
    const std::string& targetFile = filename.empty() ? filename_ : filename;
    if (targetFile.empty()) {
        return false;
    }
    
    SI_Error rc = impl_->ini.SaveFile(targetFile.c_str());
    return rc >= 0;
}

std::string DataStore::getString(const std::string& section, const std::string& key, const std::string& defaultValue) {
    const char* value = impl_->ini.GetValue(section.c_str(), key.c_str(), defaultValue.c_str());
    return value ? value : defaultValue;
}

int DataStore::getInt(const std::string& section, const std::string& key, int defaultValue) {
    return static_cast<int>(impl_->ini.GetLongValue(section.c_str(), key.c_str(), defaultValue));
}

float DataStore::getFloat(const std::string& section, const std::string& key, float defaultValue) {
    const char* value = impl_->ini.GetValue(section.c_str(), key.c_str(), nullptr);
    if (value) {
        return std::stof(value);
    }
    return defaultValue;
}

bool DataStore::getBool(const std::string& section, const std::string& key, bool defaultValue) {
    return impl_->ini.GetBoolValue(section.c_str(), key.c_str(), defaultValue);
}

void DataStore::setString(const std::string& section, const std::string& key, const std::string& value) {
    impl_->ini.SetValue(section.c_str(), key.c_str(), value.c_str());
}

void DataStore::setInt(const std::string& section, const std::string& key, int value) {
    impl_->ini.SetLongValue(section.c_str(), key.c_str(), value);
}

void DataStore::setFloat(const std::string& section, const std::string& key, float value) {
    impl_->ini.SetValue(section.c_str(), key.c_str(), std::to_string(value).c_str());
}

void DataStore::setBool(const std::string& section, const std::string& key, bool value) {
    impl_->ini.SetBoolValue(section.c_str(), key.c_str(), value);
}

void DataStore::removeKey(const std::string& section, const std::string& key) {
    impl_->ini.Delete(section.c_str(), key.c_str());
}

void DataStore::removeSection(const std::string& section) {
    impl_->ini.Delete(section.c_str(), nullptr);
}

bool DataStore::hasKey(const std::string& section, const std::string& key) {
    return impl_->ini.GetValue(section.c_str(), key.c_str(), nullptr) != nullptr;
}

bool DataStore::hasSection(const std::string& section) {
    return impl_->ini.GetSection(section.c_str()) != nullptr;
}

void DataStore::clear() {
    impl_->ini.Reset();
}

} // namespace easy2d
