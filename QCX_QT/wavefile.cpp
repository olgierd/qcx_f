#include "wavefile.h"

namespace EdenClass
{
    WaveFile::WaveFile()
    {
        CheckPCM = false;
    }

    WaveFile::~WaveFile()
    {
    }

    bool WaveFile::FileNew(string FileName, int SampleRate_, bool TwoBytesPerSample_, bool Stereo_)
    {
        SampleRate = SampleRate_;
        TwoBytesPerSample = TwoBytesPerSample_;
        Stereo = Stereo_;
        HeaderSize = 44;

        // "RIFF"
        Header[0] = 0x52;
        Header[1] = 0x49;
        Header[2] = 0x46;
        Header[3] = 0x46;
        // Number of bytes = (Number_of_samples * Bytes_per_sample * Channel_number) + 36
        Header[4] = 0x00;
        Header[5] = 0x00;
        Header[6] = 0x00;
        Header[7] = 0x00;
        // "WAVEfmt "
        Header[8] = 0x57;
        Header[9] = 0x41;
        Header[10] = 0x56;
        Header[11] = 0x45;
        Header[12] = 0x66;
        Header[13] = 0x6D;
        Header[14] = 0x74;
        Header[15] = 0x20;
        // PCM Format
        Header[16] = 0x10;
        Header[17] = 0x00;
        Header[18] = 0x00;
        Header[19] = 0x00;
        Header[20] = 0x01;
        Header[21] = 0x00;
        // Mono or stereo
        Header[22] = Stereo ? 0x02 : 0x01;
        Header[23] = 0x00;
        // Samples per second
        Int32__.ValU = SampleRate;
        Header[24] = Int32__.Raw[0];
        Header[25] = Int32__.Raw[1];
        Header[26] = Int32__.Raw[2];
        Header[27] = Int32__.Raw[3];
        // Bytes per second
        Int32__.ValU = SampleRate;
        if (Stereo)
        {
            Int32__.ValU = Int32__.ValU << 1;
        }
        if (TwoBytesPerSample)
        {
            Int32__.ValU = Int32__.ValU << 1;
        }
        Header[28] = Int32__.Raw[0];
        Header[29] = Int32__.Raw[1];
        Header[30] = Int32__.Raw[2];
        Header[31] = Int32__.Raw[3];
        // Bytes per second multiplied per number of cannels
        Header[32] = 0x01;
        if (Stereo)
        {
            Header[32] = Header[32] << 1;
        }
        if (TwoBytesPerSample)
        {
            Header[32] = Header[32] << 1;
        }
        Header[33] = 0x00;
        // 8-bit or 16-bit
        Header[34] = TwoBytesPerSample ? 0x10 : 0x08;
        Header[35] = 0x00;
        // "data"
        Header[36] = 0x64;
        Header[37] = 0x61;
        Header[38] = 0x74;
        Header[39] = 0x61;
        // Number of bytes = (Number_of_samples * Bytes_per_sample * Channel_number)
        Header[40] = 0x00;
        Header[41] = 0x00;
        Header[42] = 0x00;
        Header[43] = 0x00;
        F.open(FileName, ios::out | ios::binary);
        if (F.is_open())
        {
            F.write((char*)&Header, HeaderSize);
            F.close();
            return FileOpen(FileName, false);
        }
        else
        {
            return false;
        }
    }

    bool WaveFile::FileOpen(string FileName, bool ReadOnly)
    {
        NeedHeaderUpdate = false;
        if (!Eden::FileExists(FileName))
        {
            return false;
        }
        int FileSize = Eden::FileSize(FileName);
        if (FileSize < 44)
        {
            return false;
        }
        if (ReadOnly)
        {
            F.open(FileName, ios::in | ios::binary);
        }
        else
        {
            F.open(FileName, ios::in | ios::out | ios::binary);
        }
        if (F.is_open())
        {
            // Reading header
            F.read((char*)&Header, 44);

            // Checking header format
            bool Good = true;
            // "RIFF"
            if (Header[0] != 0x52) { Good = false; }
            if (Header[1] != 0x49) { Good = false; }
            if (Header[2] != 0x46) { Good = false; }
            if (Header[3] != 0x46) { Good = false; }
            // "WAVEfmt "
            if (Header[8] != 0x57) { Good = false; }
            if (Header[9] != 0x41) { Good = false; }
            if (Header[10] != 0x56) { Good = false; }
            if (Header[11] != 0x45) { Good = false; }
            if (Header[12] != 0x66) { Good = false; }
            if (Header[13] != 0x6D) { Good = false; }
            if (Header[14] != 0x74) { Good = false; }
            if (Header[15] != 0x20) { Good = false; }

            Int32__.Raw[0] = Header[16];
            Int32__.Raw[1] = Header[17];
            Int32__.Raw[2] = Header[18];
            Int32__.Raw[3] = Header[19];
            HeaderSize = 28 + Int32__.ValU;

            if (HeaderSize > 44)
            {
                F.seekg(0);
                F.read((char*)&Header, HeaderSize);
            }

            if (CheckPCM)
            {
                // Header size - 16 for PCM
                if (Header[16] != 0x10) { Good = false; }
                if (Header[17] != 0x00) { Good = false; }
                if (Header[18] != 0x00) { Good = false; }
                if (Header[19] != 0x00) { Good = false; }
                // PCM Format
                if (Header[20] != 0x01) { Good = false; }
                if (Header[21] != 0x00) { Good = false; }
            }

            // Mono or stereo
            if ((Header[22] != 0x01) && (Header[22] != 0x02)) { Good = false; }
            if (Header[23] != 0x00) { Good = false; }
            // 8-bit or 16-bit
            if ((Header[34] != 0x08) && (Header[34] != 0x10)) { Good = false; }
            if (Header[35] != 0x00) { Good = false; }
            // "data"
            if (Header[HeaderSize - 8] != 0x64) { Good = false; }
            if (Header[HeaderSize - 7] != 0x61) { Good = false; }
            if (Header[HeaderSize - 6] != 0x74) { Good = false; }
            if (Header[HeaderSize - 5] != 0x61) { Good = false; }
            if (!Good)
            {
                F.close();
                return false;
            }

            // Number of channels
            if (Header[22] == 2)
            {
                Stereo = true;
            }
            else
            {
                Stereo = false;
            }

            // Sample resolution
            if (Header[34] == 16)
            {
                TwoBytesPerSample = true;
            }
            else
            {
                TwoBytesPerSample = false;
            }

            // Sample rate
            Int32__.Raw[0] = Header[24];
            Int32__.Raw[1] = Header[25];
            Int32__.Raw[2] = Header[26];
            Int32__.Raw[3] = Header[27];
            SampleRate = Int32__.ValU;

            // Number of samples in file
            Int32__.Raw[0] = Header[HeaderSize - 4];
            Int32__.Raw[1] = Header[HeaderSize - 3];
            Int32__.Raw[2] = Header[HeaderSize - 2];
            Int32__.Raw[3] = Header[HeaderSize - 1];
            Length = Int32__.ValU;
            if ((FileSize - HeaderSize < Length))
            {
                return false;
            }
            if (TwoBytesPerSample)
            {
                Length = Length >> 1;
            }
            if (Stereo)
            {
                Length = Length >> 1;
            }
            F.seekg(HeaderSize);
            F.seekp(HeaderSize);
            return true;
        }
        else
        {
            return false;
        }
    }

    void WaveFile::FileClose()
    {
        if (NeedHeaderUpdate)
        {
            Int32__.ValU = Length;
            if (Stereo) { Int32__.ValU = Int32__.ValU << 1; }
            if (TwoBytesPerSample) { Int32__.ValU = Int32__.ValU << 1; }

            // Number of bytes = (Number_of_samples * Bytes_per_sample * Channel_number)
            Header[HeaderSize - 4] = Int32__.Raw[0];
            Header[HeaderSize - 3] = Int32__.Raw[1];
            Header[HeaderSize - 2] = Int32__.Raw[2];
            Header[HeaderSize - 1] = Int32__.Raw[3];

            // Number of bytes = (Number_of_samples * Bytes_per_sample * Channel_number) + 36
            Int32__.ValU = Int32__.ValU + 36;
            Header[4] = Int32__.Raw[0];
            Header[5] = Int32__.Raw[1];
            Header[6] = Int32__.Raw[2];
            Header[7] = Int32__.Raw[3];

            F.seekp(0);
            F.write((char*)&Header, HeaderSize);
        }
        F.close();
    }

    void WaveFile::ReadM(unsigned char * Raw, long Offset, long Size)
    {
        if (Stereo)
        {
            unsigned char * RawX = new unsigned char[Size];
            ReadS(Raw, RawX, Offset, Size);
            for (int I = 0; I < Size; I++)
            {
                int T1 = Raw[I];
                int T2 = RawX[I];
                T1 = (T1 + T2) >> 1;
                Raw[I] = T1;
            }
            delete[] RawX;
            return;
        }

        int Size1 = Size;
        char * Temp = new char[Size1];
        if (Offset >= 0)
        {
            F.seekg(HeaderSize + (Offset));
            FOffset = Offset;
        }
        else
        {
            Offset = FOffset;
        }
        if ((Offset + Size) > Length)
        {
            for (int I = 0; I < Size; I++)
            {
                Raw[I] = 0;
            }
            Size = Length - Offset;
            if (Size <= 0)
            {
                return;
            }
            Size1 = Size;
        }
        F.read(Temp, Size1);
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Raw[I] = Temp[II];
            II++;
        }
        delete[] Temp;
        FOffset += Size;
        F.seekp(F.tellg());
    }

    void WaveFile::ReadS(unsigned char * RawL, unsigned char * RawR, long Offset, long Size)
    {
        if (!Stereo)
        {
            ReadM(RawL, Offset, Size);
            for (int I = 0; I < Size; I++)
            {
                RawR[I] = RawL[I];
            }
            return;
        }

        int Size2 = Size << 1;
        char * Temp = new char[Size2];
        if (Offset >= 0)
        {
            F.seekg(HeaderSize + (Offset << 1));
            FOffset = Offset;
        }
        else
        {
            Offset = FOffset;
        }
        if ((Offset + Size) > Length)
        {
            for (int I = 0; I < Size; I++)
            {
                RawL[I] = 0;
                RawR[I] = 0;
            }
            Size = Length - Offset;
            if (Size <= 0)
            {
                return;
            }
            Size2 = Size << 2;
        }
        F.read(Temp, Size2);
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            RawL[I] = Temp[II];
            II++;
            RawR[I] = Temp[II];
            II++;
        }
        delete[] Temp;
        FOffset += Size;
        F.seekp(F.tellg());
    }

    void WaveFile::ReadM(short * Raw, long Offset, long Size)
    {
        if (Stereo)
        {
            short * RawX = new short[Size];
            ReadS(Raw, RawX, Offset, Size);
            for (int I = 0; I < Size; I++)
            {
                int T1 = Raw[I];
                int T2 = RawX[I];
                T1 = (T1 + T2) >> 1;
                Raw[I] = T1;
            }
            delete[] RawX;
            return;
        }

        int Size2 = Size << 1;
        char * Temp = new char[Size2];
        if (Offset >= 0)
        {
            F.seekg(HeaderSize + (Offset << 1));
            FOffset = Offset;
        }
        else
        {
            Offset = FOffset;
        }
        if ((Offset + Size) > Length)
        {
            for (int I = 0; I < Size; I++)
            {
                Raw[I] = 0;
            }
            Size = Length - Offset;
            if (Size <= 0)
            {
                return;
            }
            Size2 = Size << 1;
        }
        F.read(Temp, Size2);
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Int16__.Raw[0] = Temp[II];
            II++;
            Int16__.Raw[1] = Temp[II];
            II++;
            Raw[I] = Int16__.ValS;
        }
        delete[] Temp;
        FOffset += Size;
        F.seekp(F.tellg());
    }

    void WaveFile::ReadS(short * RawL, short * RawR, long Offset, long Size)
    {
        if (!Stereo)
        {
            ReadM(RawL, Offset, Size);
            for (int I = 0; I < Size; I++)
            {
                RawR[I] = RawL[I];
            }
            return;
        }

        int Size4 = Size << 2;
        char * Temp = new char[Size4];
        if (Offset >= 0)
        {
            F.seekg(HeaderSize + (Offset << 2));
            FOffset = Offset;
        }
        else
        {
            Offset = FOffset;
        }
        if ((Offset + Size) > Length)
        {
            for (int I = 0; I < Size; I++)
            {
                RawL[I] = 0;
                RawR[I] = 0;
            }
            Size = Length - Offset;
            if (Size <= 0)
            {
                return;
            }
            Size4 = Size << 2;
        }
        F.read(Temp, Size4);
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Int16__.Raw[0] = Temp[II];
            II++;
            Int16__.Raw[1] = Temp[II];
            II++;
            RawL[I] = Int16__.ValS;
            Int16__.Raw[0] = Temp[II];
            II++;
            Int16__.Raw[1] = Temp[II];
            II++;
            RawR[I] = Int16__.ValS;
        }
        delete[] Temp;
        FOffset += Size;
        F.seekp(F.tellg());
    }

    void WaveFile::WriteM(unsigned char * Raw, long Offset, long Size)
    {
        if (Stereo)
        {
            WriteS(Raw, Raw, Offset, Size);
            return;
        }

        NeedHeaderUpdate = true;
        int Size1 = Size;
        char * Temp = new char[Size1];
        if (Offset >= 0)
        {
            F.seekp(HeaderSize + (Offset));
            FOffset = Offset;
        }
        else
        {
            Offset = F.tellp();
            Offset = Offset - HeaderSize;
        }
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Temp[II] = Raw[I];
            II++;
        }
        F.write(Temp, Size1);
        delete[] Temp;
        if (Length < (Offset + Size))
        {
            Length = (Offset + Size);
        }
        F.seekg(F.tellp());
    }

    void WaveFile::WriteS(unsigned char * RawL, unsigned char * RawR, long Offset, long Size)
    {
        if (!Stereo)
        {
            unsigned char * RawX = new unsigned char[Size];
            for (int I = 0; I < Size; I++)
            {
                int T1 = RawL[I];
                int T2 = RawR[I];
                T1 = (T1 + T2) >> 1;
                RawX[I] = T1;
            }
            WriteM(RawX, Offset, Size);
            delete[] RawX;
            return;
        }

        NeedHeaderUpdate = true;
        int Size2 = Size << 1;
        char * Temp = new char[Size2];
        if (Offset >= 0)
        {
            F.seekp(HeaderSize + (Offset << 1));
            FOffset = Offset;
        }
        else
        {
            Offset = F.tellp();
            Offset = (Offset - HeaderSize) >> 1;
        }
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Temp[II] = RawL[I];
            II++;
            Temp[II] = RawR[I];
            II++;
        }
        F.write(Temp, Size2);
        delete[] Temp;
        if (Length < (Offset + Size))
        {
            Length = (Offset + Size);
        }
        F.seekg(F.tellp());
    }

    void WaveFile::WriteM(short * Raw, long Offset, long Size)
    {
        if (Stereo)
        {
            WriteS(Raw, Raw, Offset, Size);
            return;
        }

        NeedHeaderUpdate = true;
        int Size2 = Size << 1;
        char * Temp = new char[Size2];
        if (Offset >= 0)
        {
            F.seekp(HeaderSize + (Offset << 1));
            FOffset = Offset;
        }
        else
        {
            Offset = F.tellp();
            Offset = (Offset - HeaderSize) >> 1;
        }
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Int16__.ValS = Raw[I];
            Temp[II] = Int16__.Raw[0];
            II++;
            Temp[II] = Int16__.Raw[1];
            II++;
        }
        F.write(Temp, Size2);
        delete[] Temp;
        if (Length < (Offset + Size))
        {
            Length = (Offset + Size);
        }
        F.seekg(F.tellp());
    }

    void WaveFile::WriteS(short * RawL, short * RawR, long Offset, long Size)
    {
        if (!Stereo)
        {
            short * RawX = new short[Size];
            for (int I = 0; I < Size; I++)
            {
                int T1 = RawL[I];
                int T2 = RawR[I];
                T1 = (T1 + T2) >> 1;
                RawX[I] = T1;
            }
            WriteM(RawX, Offset, Size);
            delete[] RawX;
            return;
        }

        NeedHeaderUpdate = true;
        int Size4 = Size << 2;
        char * Temp = new char[Size4];
        if (Offset >= 0)
        {
            F.seekp(HeaderSize + (Offset << 2));
            FOffset = Offset;
        }
        else
        {
            Offset = F.tellp();
            Offset = (Offset - HeaderSize) >> 2;
        }
        int II = 0;
        for (int I = 0; I < Size; I++)
        {
            Int16__.ValS = RawL[I];
            Temp[II] = Int16__.Raw[0];
            II++;
            Temp[II] = Int16__.Raw[1];
            II++;
            Int16__.ValS = RawR[I];
            Temp[II] = Int16__.Raw[0];
            II++;
            Temp[II] = Int16__.Raw[1];
            II++;
        }
        F.write(Temp, Size4);
        delete[] Temp;
        if (Length < (Offset + Size))
        {
            Length = (Offset + Size);
        }
        F.seekg(F.tellp());
    }

    void WaveFile::Seek(llong Offset)
    {
        if (Offset >= Length)
        {
            Offset = Length - 1;
        }
        if (Offset < 0)
        {
            Offset = 0;
        }
        FOffset = Offset;
        if (Stereo) { Offset = Offset << 1; }
        if (TwoBytesPerSample) { Offset = Offset << 1; }
        Offset = Offset + HeaderSize;
        F.seekg(Offset);
        F.seekp(Offset);
    }
}
