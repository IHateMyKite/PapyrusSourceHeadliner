#include "PapyrusSource.h"

std::vector<std::string> g_blacklist 
{
    "endproperty",
    "property ",
    "scriptname ",
    "endfunction",
    "function ",
    "endevent",
    "event ",
    "endstate",
    "state "
};

PapyrusSource::PapyrusSource(std::string a_name,std::string a_path)
{
    _filename = a_name;
    _filepath = a_path;
    _file.open((_filepath).c_str());
}

bool PapyrusSource::Filter(std::string a_line)
{
    if (_dontignorenext)
    {
        _dontignorenext = false;
        return false;
    } 

    if (_endpatern != "")
    {
        if (a_line.find(_endpatern) != std::string::npos)
        {
            _endpatern = "";
            return true;
        } 
        else
        {
            return true;
        }
    }
    
    //block comment on one line
    size_t loc_comm1pos = a_line.find(";/");
    size_t loc_comm2pos = a_line.find("/;");
    if ((loc_comm1pos != std::string::npos) && (loc_comm2pos != std::string::npos))
    {
        //check if comment is not in string (fucking hell)
        size_t loc_posapo1 = a_line.find('\"');
        size_t loc_posapo2 = a_line.find('\"',loc_posapo1 + 1);

        std::cout << loc_comm1pos << " " << loc_comm2pos << std::endl;
        if (!(((loc_posapo1 < loc_comm1pos) && (loc_posapo2 > loc_comm1pos)) || ((loc_posapo1 < loc_comm2pos) && (loc_posapo2 > loc_comm2pos)))) return true;
    }

    //block comment on one line
    size_t loc_comm3pos = a_line.find('{');
    size_t loc_comm4pos = a_line.find('}');
    if ((loc_comm3pos != std::string::npos) && (loc_comm4pos != std::string::npos) && loc_comm3pos < loc_comm4pos)
    {
        //check if comment is not in string (fucking hell)
        size_t loc_posapo1 = a_line.find('\"');
        size_t loc_posapo2 = a_line.find('\"',loc_posapo1 + 1);

        if (!(((loc_posapo1 < loc_comm3pos) && (loc_posapo2 > loc_comm3pos)) || ((loc_posapo1 < loc_comm4pos) && (loc_posapo2 > loc_comm4pos)))) return true;
    }

    if ((a_line[0] == ';') && (a_line[1] == '/'))
    {
        
        _endpatern = "/;";
        return true;
    }

    if ((a_line[0] == '{'))
    {
        _endpatern = "}";
        return true;
    }

    if (!FilterKeyword(a_line))
    {
        if (a_line[a_line.length() - 1] == '\\') 
        {
            _dontignorenext = true;
        }
        return false;
    }

    return true;
}


PapyrusSource::~PapyrusSource()
{
    if (_file.good()) _file.close();
}

bool PapyrusSource::FilterKeyword(std::string a_line)
{
    for(auto&& it : g_blacklist)
    {
        size_t loc_pos = a_line.find(it);

        if (loc_pos != std::string::npos)
        {
            size_t loc_poscomm = a_line.find(';');
            size_t loc_posapo = a_line.find('\"');

            if (loc_pos == 0) return false;

            //check if there is not string before keyword
            if ((loc_posapo != std::string::npos) && (loc_posapo < loc_pos)) return true;

            //no comment on the line
            if (loc_poscomm == std::string::npos)
            {
                return !isspace(a_line[loc_pos - 1]);
            }
            //comment after main word
            else if (loc_poscomm > loc_pos)
            {
               return !isspace(a_line[loc_pos - 1]);
            }
        }
    }
}

bool PapyrusSource::IsValid()
{
     return _file.good();
}

std::string PapyrusSource::GetPath()
{
    return _filepath;
}

bool PapyrusSource::Process()
{
    std::cout << "Processing file = " << _filepath << std::endl;
    std::string loc_line;
    std::ofstream temp;
    std::string loc_tmppath = _filepath + ".tmp";

    temp.open(loc_tmppath);

    bool loc_ignore = false;
    
    while (std::getline(_file, loc_line)) 
    {

        //change to lowecase and change all tabs to spaces
        std::transform(loc_line.begin(), loc_line.end(), loc_line.begin(), [](unsigned char c)
            { 
                if (c == '\t') return static_cast<int>(' ');
                return std::tolower(c); 
            });

        //erase gap at start of line
        const std::string loc_delims(" \t"); //white chars
        size_t loc_firstcharpos = loc_line.find_first_not_of(loc_delims);
        loc_line.erase(loc_line.begin(),loc_line.begin() + loc_firstcharpos);


        loc_ignore = Filter(loc_line);

        if (!loc_ignore) 
        {
            temp << loc_line << std::endl;
        }
 
    }
    temp << ";This file was cleaned with PapyrusSourceHeadliner " << VERSION;
    temp.close();
    _file.close();

    remove(_filepath.c_str());
    (void)rename(loc_tmppath.c_str(),_filepath.c_str());

    return false;
}
