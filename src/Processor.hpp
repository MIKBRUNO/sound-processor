#pragma once
#include <string>
#include <vector>
#include "Converter.hpp"
#include "SampleStream.hpp"

namespace SoundProcessor {

    const size_t SAMPLE_RATE = 44100ul;

    class config_failure : public std::exception {
    public:
        inline config_failure(const char* msg) : m(msg) {}
        inline config_failure() : m(nullptr) {}
        virtual const char* what() const noexcept {
            return m == nullptr ? "Bad configuration file" : m;
        }
    private:
        const char* m;
    };
    
    class Configuration {
    public:
        Configuration(const std::string&, const std::vector<std::string>&);
        using converterVector = std::vector<std::shared_ptr<Converter>>;
        inline const converterVector& getConverters() const { return converters; }
    private:
        converterVector converters;
        void readConfig(std::ifstream&, const std::vector<std::string>&);
    };

    class Processor {
    public:
        inline Processor(const Configuration& cfg, const std::string& outFile, const std::string& inFile)
        : config(cfg), iss(inFile), oss(outFile) {  }
        void run();

        Processor(const Processor&) = delete;
        inline Processor(Processor&& o) = delete;
        Processor& operator= (const Processor&) = delete;
        inline Processor& operator= (Processor&& o) = delete;
    private:
        const Configuration& config;
        iSampleStream iss;
        oSampleStream oss;
    };

}
