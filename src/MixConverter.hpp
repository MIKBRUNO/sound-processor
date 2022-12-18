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
        virtual std::shared_ptr<Converter> parse(
            const std::vector<int>& iargs,
            const std::vector<std::string>& streams,
            const std::vector<size_t>& streamIdxs
        ) const;
        virtual const char* getName() const;
        virtual const char* getHelp() const;
    };
    
}