#include <cstring>
#include "SampleStream.hpp"
#include "Processor.hpp"

using namespace std;

namespace {

    const char* WAVHeader = "RIFF\0\0\0\0WAVE"
                            "fmt\x20\x10\0\0\0"     // fmt chunk header
                            "\x01\0\x01\0"          // PCM mono
                            "\x44\xAC\0\0"          // 44100
                            "\x88\x58\x01\0"        // 88200
                            "\2\0\x10\0"            // align & bits per sample
                            "data\0\0\0\0";         // data chunk header


    inline uint32_t le2ui32(const char bytes[4]) {
        return (bytes[0]&0xff) | ((bytes[1]&0xff) << 8) | ((bytes[2]&0xff) << 16) | ((bytes[3]&0xff) << 24);
    }

    inline void ui32_2le(char bytes[4], uint32_t i) {
        bytes[0] = i & 0xff;
        bytes[1] = (i >> 8) & 0xff;
        bytes[2] = (i >> 16) & 0xff;
        bytes[3] = (i >> 24) & 0xff;
    }

    inline uint16_t le2i16(const char bytes[2]) {
        return (bytes[0]&0xff) | ((bytes[1]&0xff) << 8);
    }

    inline void i16_2le(char bytes[2], uint16_t i) {
        bytes[0] = i & 0xff;
        bytes[1] = (i >> 8) & 0xff;
    }

}

namespace SoundProcessor {

    iSampleStream::iSampleStream(const string& filename) {
        file.exceptions( ios::badbit | ios::failbit | ios::eofbit );
        try {
            file.open(filename, ios::in | ios::binary);
        }
        catch (const fstream::failure& e) {
            throw fstream::failure("Cannot open input file");
        }
        try {
            initWAVfile();
        }
        catch (const fstream::failure& e) {
            throw wav_failure("Bad input WAV file");
        }
    }

    size_t iSampleStream::read(int16_t* buffer, size_t size) {
        size *= 2;
        streampos curpos = file.tellg();
        if (size + curpos - dataBeg > dataBytes)
            size = dataBytes - (curpos - dataBeg);
        size = (size/2)*2;
        char* cbuf = reinterpret_cast<char*>(buffer);
        try {
            file.read(cbuf, size);
        }
        catch (const ifstream::failure& e) {
            throw wav_failure("Unexpected failure during reading file.");
        }
        for (size_t i = 0; i < size / 2; ++i) {
            int16_t tmp = le2i16(cbuf + i*2);
            buffer[i] = tmp;
        }

        return size/2;
    }

    void iSampleStream::initWAVfile() {
        char buff[4];
        enum { RIFF, SEARCHFMT, FMT, SEARCHDATA, DATA } state = RIFF;
        while (state != DATA) {
            if (state == RIFF) {
                file.read(buff, 4);
                if (0 != strncmp(buff, "RIFF", 4))
                    throw wav_failure();
                file.read(buff, 4); // chunk size
                file.read(buff, 4);
                if (0 != strncmp(buff, "WAVE", 4))
                    throw wav_failure();
                state = SEARCHFMT;
            }
            else if (state == SEARCHFMT) {
                file.read(buff, 4);
                if (0 != strncmp(buff, "fmt\x20", 4)) {
                    file.read(buff, 4);
                    uint32_t subchunkSize = le2ui32(buff);
                    file.seekg(subchunkSize, file.cur);
                    state = SEARCHFMT;
                }
                else {
                    state = FMT;
                }
            }
            else if (state == FMT) {
                file.read(buff, 4);
                uint32_t subchunkSize = le2ui32(buff);
                file.read(buff, 2);
                if (1 != buff[0] || buff[1] != 0 || 16u != subchunkSize)
                    throw wav_failure("Not a PCM format");
                file.read(buff, 2);
                if (1 != buff[0] || 0 != buff[1])
                    throw wav_failure("Not a mono");
                file.read(buff, 4);
                if (SAMPLE_RATE != le2ui32(buff))
                    throw wav_failure("Not a 44100 rate");
                file.seekg(8, file.cur);
                state = SEARCHDATA;
            }
            else if (state == SEARCHDATA) {
                file.read(buff, 4);
                if (0 != strncmp(buff, "data", 4)) {
                    file.read(buff, 4);
                    uint32_t subchunkSize = le2ui32(buff);
                    file.seekg(subchunkSize, file.cur);
                    state = SEARCHDATA;
                }
                else {
                    file.read(buff, 4);
                    dataBeg = file.tellg();
                    dataBytes = le2ui32(buff);
                    state = DATA;
                }
            }
        }
    }

    oSampleStream::oSampleStream(const std::string& filename) {
        file.exceptions( ios::badbit | ios::failbit | ios::eofbit );
        try {
            file.open(filename, ios::out | ios::binary);
        }
        catch (const fstream::failure& e) {
            throw fstream::failure("Cannot open input file");
        }
        try {
            file.write(WAVHeader, 44);
        }
        catch (const fstream::failure& e) {
            throw fstream::failure("Unexpected exception while writing output file");
        }
    }

    void oSampleStream::write(const int16_t* buffer, size_t size) {
        size *= 2;
        dataSize += size;
        streampos tmp = file.tellp();
        commitWAVfile();
        tmp = file.tellp();
        char* cbuf = new char[size];
        for (size_t i = 0; i < size/2; ++i)
            i16_2le(cbuf + i*2, buffer[i]);
        file.write(cbuf, size);
        tmp = file.tellp();
    }

    void oSampleStream::commitWAVfile() {
        char buf[4];
        ui32_2le(buf, dataSize + 36);
        file.seekp(4, file.beg);
        file.write(buf, 4);
        ui32_2le(buf, dataSize);
        file.seekp(40, file.beg);
        file.write(buf, 4);
        file.seekp(0, file.end);
    }

}
