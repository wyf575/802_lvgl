#include "StoragePreferences.h"
#include "json/json.h"
#include <unistd.h>

#define SYSTEM_STORAGE_FILE	"./storagePrf.js"

static bool StoragePreferences::putString(const std :: string & key, const std :: string & val)
{
	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(SYSTEM_STORAGE_FILE, root)){
		return false;
	}

	root[key] = val;

	std::string str = root.toStyledString();
	std::ofstream ofs(SYSTEM_STORAGE_FILE);
	ofs << str;
	ofs.close();

	return true;
}

static bool StoragePreferences::putInt(const std :: string & key, int val)
{
	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(SYSTEM_STORAGE_FILE, root)){
		return false;
	}

	root[key] = val;

	std::string str = root.toStyledString();
	std::ofstream ofs(SYSTEM_STORAGE_FILE);
	ofs << str;
	ofs.close();

	return true;
}

static bool StoragePreferences::putBool(const std :: string & key, bool val)
{
	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(SYSTEM_STORAGE_FILE, root)){
		return false;
	}

	root[key] = val;

	std::string str = root.toStyledString();
	std::ofstream ofs(SYSTEM_STORAGE_FILE);
	ofs << str;
	ofs.close();

	return true;
}

static bool StoragePreferences::putFloat(const std :: string & key, float val)
{
	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(SYSTEM_STORAGE_FILE, root)){
		return false;
	}

	root[key] = (double)val;

	std::string str = root.toStyledString();
	std::ofstream ofs(SYSTEM_STORAGE_FILE);
	ofs << str;
	ofs.close();

	return true;
}

static bool StoragePreferences::remove(const std :: string & key)
{
	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(SYSTEM_STORAGE_FILE, root)){
		return false;
	}

	root.removeMember(key);

	std::string str = root.toStyledString();
	std::ofstream ofs(SYSTEM_STORAGE_FILE);
	ofs << str;
	ofs.close();

	return true;
}

static bool StoragePreferences::clear()
{
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter fwriter;

	if(!reader.parse(SYSTEM_STORAGE_FILE, root)){
		return false;
	}

	root.clear();

	std::string str = root.toStyledString();
	std::ofstream ofs(SYSTEM_STORAGE_FILE);
	ofs << str;
	ofs.close();

	return true;
}

static std::string StoragePreferences::getString(const std :: string & key, const std :: string & defVal)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream ifs(SYSTEM_STORAGE_FILE);

	if(!reader.parse(ifs, root)){
		fprintf(stderr, "[%s]json parse fail\n", __FUNCTION__);
		return defVal;
	}else{
		if(root.isMember(key)){
			return root[key].asString();
		}else{
			return defVal;
		}
	}
}

static int StoragePreferences::getInt(const std :: string & key, int defVal)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream ifs(SYSTEM_STORAGE_FILE);

	if(!reader.parse(ifs, root)){
		fprintf(stderr, "[%s]json parse fail\n", __FUNCTION__);
		return defVal;
	}else{
		if(root.isMember(key)){
			return root[key].asInt();
		}else{
			return defVal;
		}
	}
}

static bool StoragePreferences::getBool(const std :: string & key, bool defVal)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream ifs(SYSTEM_STORAGE_FILE);

	if(!reader.parse(ifs, root)){
		fprintf(stderr, "[%s]json parse fail\n", __FUNCTION__);
		return defVal;
	}else{
		if(root.isMember(key)){
			return root[key].asBool();
		}else{
			return defVal;
		}
	}
}

static float StoragePreferences::getFloat(const std :: string & key, float defVal)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream ifs(SYSTEM_STORAGE_FILE);

	if(!reader.parse(ifs, root)){
		fprintf(stderr, "[%s]json parse fail\n", __FUNCTION__);
		return defVal;
	}else{
		if(root.isMember(key)){
			return (float)root[key].asDouble();
		}else{
			return defVal;
		}
	}
}
