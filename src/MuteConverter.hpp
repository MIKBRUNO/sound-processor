#pragma once
#include "Processor.hpp"

namespace SoundProcessor {

    class MuteConverter : public Converter {
    public:
        inline MuteConverter(size_t b = 0, size_t e = 0) : muteBeg(b), muteEnd(e) {  }
        virtual void convert(int16_t*, size_t, size_t);
        virtual ~MuteConverter() noexcept {}
    private:
        size_t muteBeg;
        size_t muteEnd;
    };

    class MuteConverterCreator : public ConverterCreator {
    public:
        virtual std::shared_ptr<Converter> parse(
            const std::vector<int>& iargs,
            const std::vector<std::string>& streams,
            const std::vector<size_t>& streamIdxs
        ) const;
        virtual const char* getName() const;
        virtual const char* getHelp() const;
    };
    
}