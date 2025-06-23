#pragma once

#include <iosfwd>
#include <string>
#include <vector>

class FileReader
{

public:

    std::vector<char> ReadFromFile(const std::string& fileName) const;

};

