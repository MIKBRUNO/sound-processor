#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include "SampleStream.hpp"

namespace SoundProcessor {

    class Converter {
    public:
        virtual void convert(int16_t*, size_t, size_t) = 0;
        virtual ~Converter() noexcept {}
    };

    class ConverterCreator {
    public:
        virtual std::shared_ptr<Converter> parse(
            const std::vector<int>& iargs,
            const std::vector<std::string>& streams,
            const std::vector<size_t>& streamIdxs
        ) const = 0;
        virtual const char* getName() const = 0;
        virtual const char* getHelp() const = 0;
    };

}
