#include <fstream>
#include <cstdint>
#include <string>

namespace SoundProcessor {

    class wav_failure : public std::exception {
    public:
        inline wav_failure(const char* msg) : m(msg) {}
        inline wav_failure() : m(nullptr) {}
        inline virtual const char* what() const noexcept {
            return m == nullptr ? "Bad WAV format" : m;
        }
    private:
        const char* m;
    };

    class iSampleStream {
    public:
        explicit iSampleStream(const std::string&);
        iSampleStream(const iSampleStream&) = delete;
        iSampleStream(iSampleStream&&) = delete;
        iSampleStream& operator= (const iSampleStream&) = delete;
        iSampleStream& operator= (iSampleStream&&) = delete;
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
        oSampleStream(oSampleStream&&) = delete;
        oSampleStream& operator= (const oSampleStream&) = delete;
        oSampleStream& operator= (oSampleStream&&) = delete;
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
