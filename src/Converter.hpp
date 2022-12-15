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
        virtual std::shared_ptr<Converter> create() = 0;
    };

}