#include "core/utils/File.h"

std::string File::readFile(const std::string& path)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error(std::string {"Error while opening the file : "} + path + "!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    std::string s(buffer.begin(), buffer.end());

    return s;
}
