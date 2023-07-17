#include "Headliner.h"

std::string g_workdir = "";

Headliner::Headliner()
{
    std::cout << "Enter name of file or folder to process (without extension): ";
    std::string loc_filename;
    std::cin >> loc_filename;
    Init(loc_filename);
}

Headliner::Headliner(std::string a_path)
{
    Init(a_path);
}

void Headliner::Init(std::string a_path)
{
    std::string loc_filename = a_path;

    struct stat loc_stat;

    //check if path is file or directory
    if (( stat(loc_filename.c_str(),&loc_stat) == 0 ) && ( loc_stat.st_mode & S_IFDIR ))
    {
        g_workdir = loc_filename;
        namespace fs = std::filesystem;
        for (const auto & entry : fs::directory_iterator(loc_filename))
        {
            if (entry.path().filename().u8string().find(".psc") != std::string::npos) {
                std::string loc_path = entry.path().u8string();
                std::string loc_name = entry.path().filename().u8string();
                _sources.push_back(std::unique_ptr<PapyrusSource>(new PapyrusSource(loc_name,loc_path)));
            }
        }
    }
    else
    {
        //add extension and try if its file
        loc_filename += ".psc";
        if (( stat(loc_filename.c_str(),&loc_stat) == 0 ) && ( loc_stat.st_mode & S_IFREG )) 
        {
            _sources.push_back(std::unique_ptr<PapyrusSource>(new PapyrusSource(loc_filename,loc_filename)));
        }
    }

    //print info in console for user
    std::cout << "Total number of files to process = " << _sources.size() << std::endl;

    if (_sources.size() > 0)
    {
        //process loaded sources
        Process();
    }
    else
    {
        std::cout << "No files to process. Exiting..." << std::endl;
    }

    #ifdef PAUSE
          system("pause");
    #endif
}

void Headliner::Process()
{
    std::cout << "Starting processing files" << std::endl;
    for (auto&& it : _sources)
    {
        it->Process();
    }
}
