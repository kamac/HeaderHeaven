#ifndef HHEAVEN_CONFIG_H
#define HHEAVEN_CONFIG_H

#include "Common.h"

class Config
{
public:
	struct SourceFile {
		std::string headerPath, sourcePath;
	};

	Config(const std::string& aConfigPath, const std::string& aConfigName);
	~Config();

	const std::vector<SourceFile>& GetFiles() const;
	void RefreshConfig();

private:
	void ScanDir(const std::string& aHeaderPath, const std::string& aSourcePath, const std::string& aExtendedPath);
	std::string FixDir(std::string aDir) const;

	std::vector<SourceFile> m_files;
	std::string m_configPath, m_configName;
};

#endif