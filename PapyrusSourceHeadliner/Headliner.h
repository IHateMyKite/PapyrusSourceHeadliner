#pragma once

#include "PapyrusSource.h"
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>
#include <string>

extern std::string g_workdir;

#define VERSION 1.0

//#define PAUSE

class PapyrusSource;

class Headliner
{
public:
    Headliner();
    Headliner(std::string a_path);
    void Init(std::string a_path);
    void Process();
private:
    std::vector<std::unique_ptr<PapyrusSource>> _sources;
};