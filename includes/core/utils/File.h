#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>

class File
{
    public:
        static std::string readFile(const std::string& path);
};
