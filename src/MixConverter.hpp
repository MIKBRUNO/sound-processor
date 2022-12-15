#pragma once
#include "Processor.hpp"

namespace SoundProcessor {

    class MixConverter : public Converter {
    public:
        inline MixConverter(const iSampleStream& mixing, size_t b = 0, size_t e = 0)
        : beg(b), end(e),
        mixingStream(mixing) {  }
        virtual void convert(int16_t*, size_t, size_t);
        virtual ~MixConverter() noexcept {}
    private:
        size_t beg;
        size_t end;
        iSampleStream mixingStream;
    };

    class MixConverterCreator : public ConverterCreator {
    public:
        inline MixConverterCreator(
            const iSampleStream& mixing, 
            size_t begin, size_t end)
            : b(begin), e(end), mixingStream(mixing) {}
        virtual std::shared_ptr<Converter> create() {
            return std::make_shared<MixConverter>(mixingStream, b, e);
        }
    private:
        size_t b, e;
        const iSampleStream& mixingStream;
    };
    
    std::shared_ptr<Converter> mixConverterParser (
        const std::vector<int>&,
        const std::vector<std::string>&,
        const std::vector<size_t>&
    );
    
}