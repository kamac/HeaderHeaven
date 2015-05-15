#include "Config.h"
#include "tinydir.h"
#include <algorithm>
#include <sstream>

Config::Config(const std::string& aConfigPath, const std::string& aConfigName)
{
	m_configPath = FixDir(aConfigPath);
	m_configName = aConfigName;
	RefreshConfig();
}

Config::~Config()
{

}

void Config::RefreshConfig()
{
	std::ifstream configFile(m_configPath + m_configName);
	std::string path;
	while (std::getline(configFile, path)) {
		std::stringstream out(path);
		std::string headerPath, sourcePath;
		out >> headerPath;
		out >> sourcePath;
		ScanDir(m_configPath + FixDir(headerPath), m_configPath + FixDir(sourcePath), "");
	}
	configFile.close();
}

const std::vector<Config::SourceFile>& Config::GetFiles() const
{
	return m_files;
}

void Config::ScanDir(const std::string& aHeaderPath, const std::string& aSourcePath, const std::string& aExtendedPath)
{
	tinydir_dir dir;
	// tinydir requires that the path doesn't end with "/"
	std::string tinydirPath = (aHeaderPath + aExtendedPath);
	tinydirPath = tinydirPath.substr(0, tinydirPath.size() - 1);
	tinydir_open(&dir, tinydirPath.c_str());

	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (std::string(file.name) == "." || std::string(file.name) == "..") {
			tinydir_next(&dir);
			continue;
		}

		if (file.is_dir) {
			ScanDir(aHeaderPath, aSourcePath, aExtendedPath + file.name + '/');
		}
		else {
			std::string extension(file.extension);
			if (extension == "h" || extension == "hpp")
			{
				SourceFile sourceFile;
				sourceFile.headerPath = aHeaderPath + aExtendedPath + file.name;
				std::string cppName(file.name);
				cppName = cppName.substr(0, cppName.find_last_of('.')) + ".cpp";
				sourceFile.sourcePath = aSourcePath + aExtendedPath + cppName;
				m_files.push_back(sourceFile);
			}
		}

		tinydir_next(&dir);
	}
}

std::string Config::FixDir(std::string aDir) const
{
	if (!aDir.length())
		return aDir;

	std::replace(aDir.begin(), aDir.end(), '\\', '/');
	for (size_t i = 0; i < aDir.length(); i++) {
		if (aDir[i++] == '/') {
			while (aDir[i] == '/')
				aDir.erase(aDir.begin() + i);
		}
	}
	if (aDir[aDir.length() - 1] != '/')
		aDir += '/';
	return aDir;
}