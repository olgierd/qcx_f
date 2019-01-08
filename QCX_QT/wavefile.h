#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <eden.h>

using namespace std;

// http://soundfile.sapp.org/doc/WaveFormat/

namespace EdenClass
{
    class WaveFile
    {
    public:
        bool CheckPCM;
        WaveFile();
        ~WaveFile();
        int SampleRate;
        long long Length;
        bool Stereo;
        bool TwoBytesPerSample;
        string FileName;
        bool FileNew(string FileName, int SampleRate_, bool TwoBytesPerSample_, bool Stereo_);
        bool FileOpen(string FileName, bool ReadOnly);
        void FileClose();

        void ReadM(unsigned char * Raw, long Offset, long Size);
        void ReadS(unsigned char * RawL, unsigned char * RawR, long Offset, long Size);
        void ReadM(short * Raw, long Offset, long Size);
        void ReadS(short * RawL, short * RawR, long Offset, long Size);

        void WriteM(unsigned char * Raw, long Offset, long Size);
        void WriteS(unsigned char * RawL, unsigned char * RawR, long Offset, long Size);
        void WriteM(short * Raw, long Offset, long Size);
        void WriteS(short * RawL, short * RawR, long Offset, long Size);

        void Seek(llong Offset);
    private:
        int HeaderSize;
        bool NeedHeaderUpdate;
        char Header[44 * 256];
        llong FOffset;
        union Int64_
        {
            char Raw[8];
            unsigned long long ValU;
            long long ValS;
        };

        union Int32_
        {
            char Raw[4];
            unsigned int ValU;
            int ValS;
        };
        union Int16_
        {
            char Raw[2];
            unsigned short ValU;
            short ValS;
        };
        Int32_ Int32__;
        Int16_ Int16__;
        fstream F;
    };
}

#endif // WAVEFILE_H
