#ifndef EDEN_H
#define EDEN_H

#include <QString>
#include <QDir>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <fstream>
#define Diff(X, Y) ((((X) - (Y)) > 0) ? ((X) - (Y)) : ((Y) - (X)))
#define Abs(X) (((X) > 0) ? (X) : (0 - (X)))
#define Min(X, Y) (((X) > (Y)) ? (Y) : (X))
#define Max(X, Y) (((X) < (Y)) ? (Y) : (X))
#define Shl(X, Y) (((Y) >= 0) ? ((X) << (Y)) : ((X) >> (0 - (Y))))
#define Shr(X, Y) (((Y) >= 0) ? ((X) >> (Y)) : ((X) << (0 - (Y))))
#define Range(X, A, B) (((X) < (A)) ? (A) : (((X) > (B)) ? (B) : (X)))


using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
typedef unsigned short ushort;
typedef long double ldouble;
class Eden
{
public:
    Eden();

    static llong FileSize(string FileName);

    static double Round(double N);
    static string EOL();
    static int RandomInt(int Min, int Max);
    static double RandomDouble(double Min, double Max);
    static double Random();

    static QString ToQStr(int N);
    static QString ToQStr(llong N);
    static QString ToQStr(double N);
    static int ToInt(string S);
    static int ToInt(QString S);
    static long ToLong(string S);
    static long ToLong(QString S);
    static llong ToLLong(string S);
    static llong ToLLong(QString S);
    static string ToStr(int N);
    static string ToStr(uint N);
    static string ToStr(long N);
    static string ToStr(ulong N);
    static string ToStr(llong N);
    static string ToStr(ullong N);
    static string ToStr(float N);
    static string ToStr(double N);
    static string ToStr(char C);
    static QString ToQStr(string S);
    static string ToStr(QString S);

    static float ToFloat(string S);
    static double ToDouble(string S);
    static ldouble ToLDouble(string S);
    static float ToFloat(string S, float DefaultValue);
    static double ToDouble(string S, double DefaultValue);
    static ldouble ToLDouble(string S, ldouble DefaultValue);

    static void StringReplace(std::string& str, const std::string& oldStr, const std::string& newStr);
    static string StringPad(string S, int L, char C, int Mode);
    static string RemoveEndLineChar(string S);
    static bool FileExists(string FN);
    static string CorrectDir(string D);
    static QString CorrectDir(QString D);
    static string FileExtension(string FileName, string FileExt);

    static unsigned int* GetRGB(unsigned int ColorRGB);
    static unsigned int XRGB(unsigned int R, unsigned int G, unsigned int B);

    static vector<string> &StringSplit(const string &S, char Delim, vector<string> &Elems);
    static vector<string> StringSplit(const string &S, char Delim);

    static vector<string> TextListFileLoad(QString FileName);
    static void TextListFileSave(QString FileName, vector<string> &L);
    static vector<string> TextListFileLoad(string FileName);
    static void TextListFileSave(string FileName, vector<string> &L);

    static string IntToHex4(int Data);
    static string IntToHex8(int Data);
    static string IntToHex16(int Data);
    static string IntToHex32(int Data);
    static string IntToHex32(uint Data);

    static string ToHex(char * Raw, uint RawSize);
    static string ToHex(uchar * Raw, uint RawSize);

    static string ApplicationDirectory();


    static int HexToInt(string Hex0);

    static string MultilineEncode(string X);
    static string MultilineDecode(string X);

    static string Trim(string X);
};

#endif // EDEN_H
