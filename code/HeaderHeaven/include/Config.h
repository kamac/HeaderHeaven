/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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