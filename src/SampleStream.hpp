#pragma once
#include <fstream>
#include <cstdint>
#include <string>

namespace SoundProcessor {

    class wav_failure : public std::exception {
    public:
        inline wav_failure(const char* msg) : m(msg) {}
        inline wav_failure() : m(nullptr) {}
        virtual const char* what() const noexcept {
            return m == nullptr ? "Bad WAV format" : m;
        }
    private:
        const char* m;
    };

    class iSampleStream {
    public:
        explicit iSampleStream(const std::string&);
        iSampleStream(const iSampleStream&) = delete;
        inline iSampleStream(iSampleStream&& o) = delete;
        iSampleStream& operator= (const iSampleStream&) = delete;
        inline iSampleStream& operator= (iSampleStream&& o) = delete;
        inline ~iSampleStream() {
            file.close();
        }
        size_t read(int16_t* buffer, size_t size);
    private:
        std::ifstream file;
        std::streampos dataBeg;
        std::size_t dataBytes;
        void initWAVfile();
    };

    class oSampleStream {
    public:
        explicit oSampleStream(const std::string&);
        oSampleStream(const oSampleStream&) = delete;
        inline oSampleStream(oSampleStream&& o) = delete;
        oSampleStream& operator= (const oSampleStream&) = delete;
        inline oSampleStream& operator= (oSampleStream&& o) = delete;
        inline ~oSampleStream() {
            file.close();
        }
        void write(const int16_t* buffer, size_t size);
    private:
        std::ofstream file;
        std::size_t dataSize;
        void commitWAVfile();
    };

}
