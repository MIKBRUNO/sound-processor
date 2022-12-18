#pragma once
#include "Processor.hpp"

namespace SoundProcessor {

    class VolumeConverter : public Converter {
    public:
        inline VolumeConverter(unsigned int vol, size_t b = 0, size_t e = 0)
        : beg(b), end(e), volume(vol) {  }
        virtual void convert(int16_t*, size_t, size_t);
        virtual ~VolumeConverter() noexcept {}
    private:
        unsigned int volume;
        size_t beg;
        size_t end;
    };

    class VolumeConverterCreator : public ConverterCreator {
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
