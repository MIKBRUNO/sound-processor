#pragma once
#include "Processor.hpp"

namespace SoundProcessor {

    class MuteConverter : public Converter {
    public:
        inline MuteConverter(size_t b, size_t e) : muteBeg(b*SAMPLE_RATE), muteEnd(e*SAMPLE_RATE) {  }
        virtual void convert(int16_t*, size_t, size_t);
        virtual ~MuteConverter() noexcept {}
    private:
        size_t muteBeg;
        size_t muteEnd;
    };

    class MuteConverterCreator : public ConverterCreator {
    public:
        inline MuteConverterCreator(size_t begin, size_t end) : b(begin), e(end) {}
        virtual std::shared_ptr<Converter> create() { return std::make_shared<MuteConverter>(b, e); }
    private:
        size_t b, e;
    };
    
    std::shared_ptr<Converter> muteConverterParser (
        const std::vector<int>&,
        const std::vector<std::string>&,
        const std::vector<size_t>&
    );
    
}