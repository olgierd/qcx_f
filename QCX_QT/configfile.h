#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace EdenClass
{
    class ConfigFile
    {
    public:
        ConfigFile();
        ~ConfigFile();
        vector<string> Raw;

        void FileLoad(string FileName);
        void FileSave(string FileName);

        void ParamClear();
        void ParamRemove(string Name);

        void ParamSet(string Name, string Value);
        void ParamSet(string Name, int Value);
        void ParamSet(string Name, long long Value);
        void ParamSet(string Name, bool Value);

        void ParamGet(string Name, string &Value);
        void ParamGet(string Name, int &Value);
        void ParamGet(string Name, long long &Value);
        void ParamGet(string Name, bool &Value);

        string ParamGetS(string Name);
        int ParamGetI(string Name);
        long long ParamGetL(string Name);
        bool ParamGetB(string Name);
    };
}

#endif // CONFIGFILE_H
