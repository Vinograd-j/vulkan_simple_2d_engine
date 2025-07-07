#include "../include/file-reader.h"

#include <fstream>
#include <ios>

std::vector<char> FileReader::ReadFromFile(const std::string& fileName) const
{
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file. Check filename and path");

    size_t fileSize = file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}