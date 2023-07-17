#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Headliner.h"
#include <algorithm>

extern std::vector<std::string> g_blacklist;

class PapyrusSource
{
    public:
        PapyrusSource(std::string a_name,std::string a_path);
        ~PapyrusSource();
        bool IsValid();
        std::string GetPath();
        bool Process();
        bool Filter(std::string a_line);
        bool FilterKeyword(std::string a_line);
    private:
        std::ifstream _file;
        std::string _filepath;
        std::string _filename;
        std::string _endpatern = "";
        bool _dontignorenext = false;
};