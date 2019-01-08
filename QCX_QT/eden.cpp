#include "eden.h"

Eden::Eden()
{
}

///
/// \brief Eden::Round - Rounds double number to integer number
/// \param N
/// \return
///
double Eden::Round(double N)
{
    return N < 0 ? ceil(N - 0.5): floor(N + 0.5);
}

///
/// \brief Eden::RandomDouble - Generates random double number within range
/// \param Min
/// \param Max
/// \return
///
double Eden::RandomDouble(double Min, double Max)
{
    double Factor = 1000;
    Min = Min * Factor;
    Max = Max * Factor;
    double Rnd = RandomInt(Min, Max);
    Rnd = Rnd / Factor;
    return Rnd;
}

///
/// \brief Eden::RandomInt - Generates random integer number within range
/// \param Min
/// \param Max
/// \return
///
int Eden::RandomInt(int Min, int Max)
{
    double X = Random();
    X = (X) * (double)(Max - Min + 1);
    int XX = Round(X) + Min;
    if (XX > Max)
    {
        XX = XX - (Max - Min + 1);
    }
    return XX;
}

///
/// \brief Eden::Random - Generates random double number from 0.000 and 1.000
/// \return
///
double Eden::Random()
{
    return ((double)rand()) / ((double)RAND_MAX);
}

///
/// \brief Eden::EOL - Returns end of line as string
/// \return
///
string Eden::EOL()
{
    return "\n";
}


float Eden::ToFloat(string S)
{
    return ToFloat(S, 0);
}
double Eden::ToDouble(string S)
{
    return ToDouble(S, 0);
}
ldouble Eden::ToLDouble(string S)
{
    return ToLDouble(S, 0);
}


float Eden::ToFloat(string S, float DefaultValue)
{
    try
    {
        StringReplace(S, ",", ".");
        return stof(S);
    }
    catch (...)
    {
        return DefaultValue;
    }
}

double Eden::ToDouble(string S, double DefaultValue)
{
    try
    {
        StringReplace(S, ",", ".");
        return stod(S);
    }
    catch (...)
    {
        return DefaultValue;
    }
}

ldouble Eden::ToLDouble(string S, ldouble DefaultValue)
{
    try
    {
        StringReplace(S, ",", ".");
        return stold(S);
    }
    catch (...)
    {
        return DefaultValue;
    }
}


///
/// \brief Eden::ToInt - Converts string to integer number
/// \param S
/// \return
///
int Eden::ToInt(string S)
{
    try
    {
        return stoi(S);
    }
    catch (...)
    {
        return 0;
    }

    /*
    long tmp = 0;
    unsigned int i = 0;
    bool m = false;
    if(S[0] == '-')
    {
        m = true;
        i++;
    }
    for(; i < S.size(); i++)
    {
        tmp = 10 * tmp + S[i] - 48;
    }
    return m ? -tmp : tmp;
    */
}

///
/// \brief Eden::ToLong - Converts string to long number
/// \param S
/// \return
///
long Eden::ToLong(string S)
{
    try
    {
        return stol(S);
    }
    catch (...)
    {
        return 0;
    }

    /*
    long tmp = 0;
    unsigned int i = 0;
    bool m = false;
    if(S[0] == '-')
    {
        m = true;
        i++;
    }
    for(; i < S.size(); i++)
    {
        tmp = 10 * tmp + S[i] - 48;
    }
    return m ? -tmp : tmp;
    */
}

///
/// \brief Eden::ToLLong - Converts string to long-long number
/// \param S
/// \return
///
llong Eden::ToLLong(string S)
{
    try
    {
        return stoll(S);
    }
    catch (...)
    {
        return 0;
    }

    /*
    llong tmp = 0;
    unsigned int i = 0;
    bool m = false;
    if(S[0] == '-')
    {
        m = true;
        i++;
    }
    for(; i < S.size(); i++)
    {
        tmp = 10 * tmp + S[i] - 48;
    }
    return m ? -tmp : tmp;
    */
}

///
/// \brief Eden::ToStr - Converts integer number to string
/// \param N
/// \return
///
string Eden::ToStr(int N)
{
    string number;
    stringstream strstream;
    strstream << N;
    strstream >> number;
    return number;
}

///
/// \brief Eden::ToStr - Converts unsigned integer number to string
/// \param N
/// \return
///
string Eden::ToStr(uint N)
{
    string number;
    stringstream strstream;
    strstream << N;
    strstream >> number;
    return number;
}

///
/// \brief Eden::ToStr - Converts long number to string
/// \param N
/// \return
///
string Eden::ToStr(long N)
{
    string number;
    stringstream strstream;
    strstream << N;
    strstream >> number;
    return number;
}

///
/// \brief Eden::ToStr - Converts unsigned long number to string
/// \param N
/// \return
///
string Eden::ToStr(ulong N)
{
    string number;
    stringstream strstream;
    strstream << N;
    strstream >> number;
    return number;
}

///
/// \brief Eden::ToStr - Converts long-long number to string
/// \param N
/// \return
///
string Eden::ToStr(llong N)
{
    string number;
    stringstream strstream;
    strstream << N;
    strstream >> number;
    return number;
}

///
/// \brief Eden::ToStr - Converts unsigned long-long number to string
/// \param N
/// \return
///
string Eden::ToStr(ullong N)
{
    string number;
    stringstream strstream;
    strstream << N;
    strstream >> number;
    return number;
}

///
/// \brief Eden::ToStr - Converts float number to string
/// \param N
/// \return
///
string Eden::ToStr(float N)
{
    char Cstr[40];
    gcvt(N, 20, Cstr);
    return Cstr;
}

///
/// \brief Eden::ToStr - Converts double number to string
/// \param N
/// \return
///
string Eden::ToStr(double N)
{
    char Cstr[40];
    gcvt(N, 20, Cstr);
    return Cstr;
}


///
/// \brief Eden::ToStr - Converts character to string
/// \param C
/// \return
///
string Eden::ToStr(char C)
{
    return string(1, C);
}


///
/// \brief Eden::ToQStr - Converts string to QString
/// \param S
/// \return
///
QString Eden::ToQStr(string S)
{
    return QString::fromUtf8(S.c_str());
}

///
/// \brief Eden::ToStr - Converts QString to string
/// \param S
/// \return
///
string Eden::ToStr(QString S)
{
    return S.toUtf8().constData();
}


///
/// \brief Eden::ToQStr - Converts double number to QString
/// \param N
/// \return
///
QString Eden::ToQStr(double N)
{
    return ToQStr(ToStr(N));
}

///
/// \brief Eden::ToQStr - Converts integer number to QString
/// \param N
/// \return
///
QString Eden::ToQStr(int N)
{
    return ToQStr(ToStr(N));
}

///
/// \brief Eden::ToQStr - Converts long-long number to QString
/// \param N
/// \return
///
QString Eden::ToQStr(llong N)
{
    return ToQStr(ToStr(N));
}

///
/// \brief Eden::ToInt - Converts QString to integer number
/// \param S
/// \return
///
int Eden::ToInt(QString S)
{
    return ToInt(ToStr(S));
}

///
/// \brief Eden::ToLong - Converts QString to long number
/// \param S
/// \return
///
long Eden::ToLong(QString S)
{
    return ToLong(ToStr(S));
}

///
/// \brief Eden::ToLLong - Converts QString to long-long number
/// \param S
/// \return
///
llong Eden::ToLLong(QString S)
{
    return ToLLong(ToStr(S));
}


///
/// \brief Eden::CorrectDir - Adds "/" or "\" at the end of path if not exists
/// \param D
/// \return
///
string Eden::CorrectDir(string D)
{
    if ((D[D.size() - 1] != '/') && (D[D.size() - 1] != '\\'))
    {
        int T1 = 0;
        int T2 = 0;
        for (unsigned int I = 0; I < D.size(); I++)
        {
            if (D[I] == '/')
            {
                T1++;
            }
            if (D[I] == '\\')
            {
                T2++;
            }
        }
        if (T2 > T1)
        {
            return D + "\\";
        }
        else
        {
            return D + "/";
        }
    }
    else
    {
        return D;
    }
}


///
/// \brief Eden::CorrectDir - Adds "/" or "\" at the end of path if not exists
/// \param D
/// \return
///
QString Eden::CorrectDir(QString D)
{
    if (D.size() < 1)
    {
        return D;
    }
    if ((D[D.size() - 1] != '/') && (D[D.size() - 1] != '\\'))
    {
        int T1 = 0;
        int T2 = 0;
        for (int I = 0; I < D.size(); I++)
        {
            if (D[I] == '/')
            {
                T1++;
            }
            if (D[I] == '\\')
            {
                T2++;
            }
        }
        if (T2 > T1)
        {
            return D + "\\";
        }
        else
        {
            return D + "/";
        }
    }
    else
    {
        return D;
    }
}


///
/// \brief Eden::FileExtension - Checks, if file name contains specified extension
/// \param FileName
/// \param FileExt
/// \return
///
string Eden::FileExtension(string FileName, string FileExt)
{
    FileExt = "." + FileExt;

    if (FileName.size() < FileExt.size())
    {
        return (FileName + FileExt);
    }

    string FileName_ = FileName;
    for (uint I = 0; I < FileName.size(); I++)
    {
        char C = FileName_[I];
        if ((C >= 'A') && (C <= 'Z'))
        {
            C = C + 32;
            FileName_[I] = C;
        }
    }

    string FileExt_ = FileExt;
    for (uint I = 0; I < FileExt.size(); I++)
    {
        char C = FileExt_[I];
        if ((C >= 'A') && (C <= 'Z'))
        {
            C = C + 32;
            FileExt_[I] = C;
        }
    }

    if (FileName_.substr(FileName_.size() - FileExt_.size()) == FileExt_)
    {
        return FileName.substr(0, FileName_.size() - FileExt_.size()) + FileExt;
    }

    return FileName + FileExt;
}


///
/// \brief Eden::StringReplace - Replaces text in string
/// \param str
/// \param oldStr
/// \param newStr
///
void Eden::StringReplace(string &str, const string &oldStr, const string &newStr)
{
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != std::string::npos)
    {
       str.replace(pos, oldStr.length(), newStr);
       pos += newStr.length();
    }
}

///
/// \brief Eden::StringPad - Adds character to string to get specified length
/// \param S
/// \param L
/// \param C
/// \param Mode - 0 - add at the tight, 1 - add at the left
/// \return
///
string Eden::StringPad(string S, int L, char C, int Mode)
{
    unsigned int L0 = (unsigned int)L;
    string OutStr = S;
    if (Mode == 1)
    {
        while (OutStr.length() < L0)
        {
            OutStr = C + OutStr;
        }
    }
    if (Mode == 0)
    {
        while (OutStr.length() < L0)
        {
            OutStr = OutStr + C;
        }
    }
    return OutStr;
}

///
/// \brief Eden::RemoveEndLineChar - Removes end of line character from string
/// \param S
/// \return
///
string Eden::RemoveEndLineChar(string S)
{
    if (S.at(S.length() - 1) == '\r')
    {
        S = S.substr(0, S.length() - 1);
    }
    if (S.at(S.length() - 1) == '\n')
    {
        S = S.substr(0, S.length() - 1);
    }
    if (S.at(S.length() - 1) == '\r')
    {
        S = S.substr(0, S.length() - 1);
    }
    if (S.at(S.length() - 1) == '\n')
    {
        S = S.substr(0, S.length() - 1);
    }
    return S;
}


///
/// \brief Eden::FileExists - Checks if specified file exists
/// \param FN
/// \return
///
bool Eden::FileExists(string FN)
{
    /*FILE *f = fopen(FN.c_str(), "r");
    if (f)
    {
        fclose(f);
        return true;
    }
    return false;*/
    std::ifstream infile(FN);
    return infile.good();
}

///
/// \brief Eden::FileSize - Get specified file size
/// \param FileName
/// \return
///
llong Eden::FileSize(string FileName)
{
    std::ifstream in(FileName.c_str(), std::ifstream::ate | std::ifstream::binary);
    if (in.is_open())
    {
        llong Size = in.tellg();
        in.close();
        return Size;
    }
    else
    {
        return -1;
    }
}




///
/// \brief Eden::GetRGB - Get RGB values from 32-bit color number
/// \param ColorRGB
/// \return
///
unsigned int *Eden::GetRGB(unsigned int ColorRGB)
{
    unsigned int* X = new unsigned int[3];
    X[0] = (ColorRGB >> 16) & 255;
    X[1] = (ColorRGB >> 8) & 255;
    X[2] = (ColorRGB >> 0) & 255;
    return X;
}

///
/// \brief Eden::RGB - Creates 32-bit color number based on RGB values
/// \param R
/// \param G
/// \param B
/// \return
///
unsigned int Eden::XRGB(unsigned int R, unsigned int G, unsigned int B)
{
    return (R << 16) + (G << 8) + B;
}


///
/// \brief Eden::StringSplit - Splits string to vector using specified character
/// \param s
/// \param delim
/// \param elems
/// \return
///
vector<string> &Eden::StringSplit(const string &S, char Delim, vector<string> &Elems)
{
    string Temp = "";
    unsigned int L = S.length();
    for (unsigned int I = 0; I < L; I++)
    {
        if (S[I] == Delim)
        {
            Elems.push_back(Temp);
            Temp = "";
        }
        else
        {
            Temp = Temp + string(1, S[I]);
        }
    }
    Elems.push_back(Temp);
    return Elems;
}

///
/// \brief Eden::StringSplit - Splits string to vector using specified character
/// \param s
/// \param delim
/// \return
///
vector<string> Eden::StringSplit(const string &S, char Delim)
{
    vector<string> Elems;
    return Eden::StringSplit(S, Delim, Elems);
}



///
/// \brief Eden::TextListFileLoad - Loads text file to vector
/// \param FileName
/// \return
///
vector<string> Eden::TextListFileLoad(QString FileName)
{
    return TextListFileLoad(ToStr(FileName));
}

///
/// \brief Eden::TextListFileLoad - Loads text file to vector
/// \param FileName
/// \return
///
vector<string> Eden::TextListFileLoad(string FileName)
{
    vector<string> L;
    L.clear();
    fstream FS(FileName.c_str(), ios::in);
    if (FS.is_open())
    {
        string Buf;
        while (getline(FS, Buf))
        {
            L.push_back(Buf);
        }
        FS.close();
    }
    return L;
}

///
/// \brief Eden::TextListFileSave - Saves vector to text file
/// \param FileName
/// \param L
///
void Eden::TextListFileSave(QString FileName, vector<string> &L)
{
    TextListFileSave(ToStr(FileName), L);
}

///
/// \brief Eden::TextListFileSave - Saves vector to text file
/// \param FileName
/// \param L
///
void Eden::TextListFileSave(string FileName, vector<string> &L)
{
    fstream FS(FileName.c_str(), ios::out);
    if (FS.is_open())
    {
        for (vector<string>::iterator I = L.begin(); I != L.end(); I++)
        {
            FS << *I << endl;
        }
        FS.close();
    }
}

///
/// \brief Eden::IntToHex4 -  - Changes number to 4-bit hex (one character)
/// \param Data
/// \return
///
string Eden::IntToHex4(int Data)
{
    switch (Data)
    {
        case 0: return "0";
        case 1: return "1";
        case 2: return "2";
        case 3: return "3";
        case 4: return "4";
        case 5: return "5";
        case 6: return "6";
        case 7: return "7";
        case 8: return "8";
        case 9: return "9";
        case 10: return "A";
        case 11: return "B";
        case 12: return "C";
        case 13: return "D";
        case 14: return "E";
        case 15: return "F";
    }
    return "?";
}

///
/// \brief Eden::IntToHex8 - Changes number to 8-bit hex (two characters)
/// \param Data
/// \return
///
string Eden::IntToHex8(int Data)
{
    int D1 = Data >> 4;
    int D2 = Data & 15;
    return IntToHex4(D1) + IntToHex4(D2);
}

///
/// \brief Eden::IntToHex16 - Changes number to 16-bit hex (four characters)
/// \param Data
/// \return
///
string Eden::IntToHex16(int Data)
{
    int D1 = Data >> 8;
    int D2 = Data & 255;
    return IntToHex8(D1) + IntToHex8(D2);
}

///
/// \brief Eden::IntToHex32 - Changes number to 32-bit hex (eight characters)
/// \param Data
/// \return
///
string Eden::IntToHex32(int Data)
{
    int D1 = Data >> 16;
    int D2 = Data & 65535;
    return IntToHex16(D1) + IntToHex16(D2);
}

///
/// \brief Eden::IntToHex32 - Changes unsigned number to 32-bit hex (eight characters)
/// \param Data
/// \return
///
string Eden::IntToHex32(uint Data)
{
    uint D1 = Data >> 16;
    uint D2 = Data & 65535;
    return IntToHex16((int)D1) + IntToHex16((int)D2);
}

///
/// \brief Eden::ToHex - Converts byte array to hex representation
/// \param Raw
/// \param RawSize
/// \return
///
string Eden::ToHex(char *Raw, uint RawSize)
{
    return ToHex((uchar*)Raw, RawSize);
}

///
/// \brief Eden::ToHex - Converts byte array to hex representation
/// \param Raw
/// \param RawSize
/// \return
///
string Eden::ToHex(uchar *Raw, uint RawSize)
{
    string X = "";
    for (uint I = 0; I < RawSize; I++)
    {
        X = X + Eden::IntToHex8(Raw[I]);
    }
    return X;
}

///
/// \brief Eden::HexToInt - Changes 8-bit or 16-bit hex number to integer
/// \param Hex0
/// \return
///
int Eden::HexToInt(string Hex0)
{
    int L = Hex0.length();
    string Hex = "";
    for (int i = 0; i < L; i++)
    {
        Hex = Hex0[i] + Hex;
    }
    int D = 0;
    int N = 0;
    for (int i = L - 1; i >= 0; i--)
    {
        D = 0;
        if (Hex[i] == '0') { D = 0; }
        if (Hex[i] == '1') { D = 1; }
        if (Hex[i] == '2') { D = 2; }
        if (Hex[i] == '3') { D = 3; }
        if (Hex[i] == '4') { D = 4; }
        if (Hex[i] == '5') { D = 5; }
        if (Hex[i] == '6') { D = 6; }
        if (Hex[i] == '7') { D = 7; }
        if (Hex[i] == '8') { D = 8; }
        if (Hex[i] == '9') { D = 9; }
        if (Hex[i] == 'A') { D = 10; }
        if (Hex[i] == 'B') { D = 11; }
        if (Hex[i] == 'C') { D = 12; }
        if (Hex[i] == 'D') { D = 13; }
        if (Hex[i] == 'E') { D = 14; }
        if (Hex[i] == 'F') { D = 15; }
        if (Hex[i] == 'a') { D = 10; }
        if (Hex[i] == 'b') { D = 11; }
        if (Hex[i] == 'c') { D = 12; }
        if (Hex[i] == 'd') { D = 13; }
        if (Hex[i] == 'e') { D = 14; }
        if (Hex[i] == 'f') { D = 15; }
        if (i == 0) { N = N + (D); }
        if (i == 1) { N = N + (D * 16); }
        if (i == 2) { N = N + (D * 256); }
        if (i == 3) { N = N + (D * 4096); }
    }
    return N;
}

///
/// \brief Eden::ApplicationDirectory - Application directory
/// \return
///
string Eden::ApplicationDirectory()
{
    return Eden::CorrectDir(Eden::ToStr(QDir::currentPath()));
}

///
/// \brief Eden::MultilineEncode - Encodes multiline text into one line
/// \param X
/// \return
///
string Eden::MultilineEncode(string X)
{
    string XXX = "";
    bool Std = true;
    for (uint I = 0; I < X.length(); I++)
    {
        char C = X[I];
        Std = true;
        if (C == ';') { XXX += "\\#"; Std = false; }
        if (C == '\\') { XXX += "\\\\"; Std = false; }
        if (C == '\"') { XXX += "\\_"; Std = false; }
        if (C == '\'') { XXX += "\\="; Std = false; }
        if (C == '\r') { XXX += "\\r"; Std = false; }
        if (C == '\n') { XXX += "\\n"; Std = false; }
        if (C == '\t') { XXX += "\\t"; Std = false; }
        if (C == '\v') { XXX += "\\v"; Std = false; }
        if (Std)
        {
            XXX += C;
        }
    }
    return "\"" + XXX + "\"";
}

///
/// \brief Eden::MultilineDecode - Decodes multiline text from one line
/// \param X
/// \return
///
string Eden::MultilineDecode(string X)
{
    string XXX = "";
    if ((X[0] == '\"') && (X[X.length() - 1] == '\"'))
    {
        for (uint I = 1; I < (X.length() - 1); I++)
        {
            char C = X[I];
            if (C == '\\')
            {
                I++;
                C = X[I];
                if (C == '#') { XXX += ";"; }
                if (C == '\\') { XXX += "\\"; }
                if (C == '_') { XXX += "\""; }
                if (C == '=') { XXX += "\'"; }
                if (C == 'r') { XXX += "\r"; }
                if (C == 'n') { XXX += "\n"; }
                if (C == 't') { XXX += "\t"; }
                if (C == 'v') { XXX += "\v"; }
            }
            else
            {
                XXX += C;
            }
        }
        return XXX;
    }
    else
    {
        return X;
    }
}

///
/// \brief Eden::Trim - Removes leading and trailing whitespaces from string
/// \param X
/// \return
///
string Eden::Trim(string X)
{
    size_t first = X.find_first_not_of(' ');
    if (string::npos == first)
    {
        return X;
    }
    size_t last = X.find_last_not_of(' ');
    return X.substr(first, (last - first + 1));
}
