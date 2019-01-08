#include "configfile.h"

namespace EdenClass
{
    ConfigFile::ConfigFile()
    {

    }

    ConfigFile::~ConfigFile()
    {

    }

    void ConfigFile::FileLoad(string FileName)
    {
        ParamClear();
        fstream F(FileName, ios::in);
        if (F.is_open())
        {
            stringstream SS;
            SS << F.rdbuf();
            string RawS = SS.str();
            F.close();
            string Buf = "";
            for (unsigned int I = 0; I < RawS.length(); I++)
            {
                char C = RawS[I];
                if ((C == '\r') || (C == '\n'))
                {
                    if (Buf != "")
                    {
                        Raw.push_back(Buf);
                        Buf = "";
                    }
                }
                else
                {
                    Buf += C;
                }
            }
            if (Buf != "")
            {
                Raw.push_back(Buf);
                Buf = "";
            }
        }
    }

    void ConfigFile::FileSave(string FileName)
    {
        fstream F(FileName, ios::out);
        if (F.is_open())
        {
            for (unsigned int I = 0; I < Raw.size(); I++)
            {
                if (Raw[I] != "")
                {
                    F << Raw[I] << endl;
                }
            }
            F.close();
        }
    }

    void ConfigFile::ParamClear()
    {
        Raw.clear();
    }

    void ConfigFile::ParamRemove(string Name)
    {
        string NameX = Name + "=";
        for (unsigned int I = 0; I < Raw.size(); I++)
        {
            if (Raw[I].length() >= NameX.length())
            {
                if (Raw[I].substr(0, NameX.length()) == NameX)
                {
                    Raw[I] = "";
                    return;
                }
            }
        }
    }

    void ConfigFile::ParamSet(string Name, string Value)
    {
        string NameX = Name + "=";
        for (unsigned int I = 0; I < Raw.size(); I++)
        {
            if (Raw[I].length() >= NameX.length())
            {
                if (Raw[I].substr(0, NameX.length()) == NameX)
                {
                    Raw[I] = NameX + Value;
                    return;
                }
            }
        }
        Raw.push_back(NameX + Value);
    }

    void ConfigFile::ParamSet(string Name, int Value)
    {
        ParamSet(Name, to_string(Value));
    }

    void ConfigFile::ParamSet(string Name, long long Value)
    {
        ParamSet(Name, to_string(Value));
    }

    void ConfigFile::ParamSet(string Name, bool Value)
    {
        string X = "";
        if (Value)
        {
            X = "1";
            ParamSet(Name, X);
        }
        else
        {
            X = "0";
            ParamSet(Name, X);
        }
    }

    void ConfigFile::ParamGet(string Name, string &Value)
    {
        for (unsigned int I = 0; I < Raw.size(); I++)
        {
            string S = Raw[I];
            int X = -1;
            for (unsigned int II = 0; II < S.length(); II++)
            {
                if (S.at(II) == '=')
                {
                    X = II;
                    break;
                }
            }
            if (X > 0)
            {
                if (Name == S.substr(0, X))
                {
                    Value = S.substr(X + 1);
                }
            }
        }
    }

    void ConfigFile::ParamGet(string Name, int &Value)
    {
        string Val = "";
        ParamGet(Name, Val);
        if (Val != "")
        {
            try
            {
                Value = stoi(Val);
            }
            catch (...)
            {
            }
        }
    }

    void ConfigFile::ParamGet(string Name, long long &Value)
    {
        string Val = "";
        ParamGet(Name, Val);
        if (Val != "")
        {
            try
            {
                Value = stoll(Val);
            }
            catch (...)
            {
            }
        }
    }

    void ConfigFile::ParamGet(string Name, bool &Value)
    {
        string Val = "";
        ParamGet(Name, Val);
        if (Val == "1")
        {
            Value = true;
        }
        if (Val == "0")
        {
            Value = false;
        }
    }

    string ConfigFile::ParamGetS(string Name)
    {
        string X = "";
        ParamGet(Name, X);
        return X;
    }

    int ConfigFile::ParamGetI(string Name)
    {
        int X = 0;
        ParamGet(Name, X);
        return X;
    }

    long long ConfigFile::ParamGetL(string Name)
    {
        long long X = 0;
        ParamGet(Name, X);
        return X;
    }

    bool ConfigFile::ParamGetB(string Name)
    {
        bool X = false;
        ParamGet(Name, X);
        return X;
    }
}
