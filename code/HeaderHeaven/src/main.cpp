#include "Config.h"
#include "Generator.h"
#include <algorithm>

int main(int argc, char* argv[])
{
	std::string cfgPath(argv[0]);
	std::replace(cfgPath.begin(), cfgPath.end(), '\\', '/');
	cfgPath = cfgPath.substr(0, cfgPath.find_last_of('/') + 1);
	Config cfg(cfgPath, "config.txt");

	Generator generator;
	const auto& files = cfg.GetFiles();
	generator.Generate(files[0].headerPath, files[0].sourcePath);

	return 0;
}