#include "MixConverter.hpp"
#include "Converter.hpp"

using namespace std;

namespace SoundProcessor {

    void MixConverter::convert(int16_t* block, size_t blockSize, size_t blockBeg) {
        int16_t* mixBlock = new int16_t[blockSize];
        size_t mixSize = mixingStream.read(mixBlock, blockSize);
        if ((blockBeg >= end && 0 != end) || blockBeg + blockSize < beg)
            return;
        size_t i = beg < blockBeg ? 0 : beg - blockBeg;
        for (;i < blockSize && (i < end - blockBeg || 0 == end); ++i) {
            int16_t avg = block[i];
            if (i < mixSize) {
                avg /= 2;
                avg += mixBlock[i] / 2;
            }
            block[i] = avg;
        }

        delete[] mixBlock;
    }

    shared_ptr<Converter> MixConverterCreator::parse (
        const std::vector<int>& iargs,
        const std::vector<std::string>& files,
        const std::vector<size_t>& fileidxs
    ) const {
        if (iargs.size() > 2 || fileidxs.size() != 1)
            throw config_failure("Bad arguments for mix converter");
        int beg = iargs.size() >= 1 ? iargs[0] : 0;
        int end = iargs.size() == 2 ? iargs[1] : 0;
        if (beg < 0 || end < 0)
            throw config_failure("Bad arguments for mix converter");
        iSampleStream iss { files[fileidxs[0]] };
        return make_shared<MixConverter>(
            iss,
            static_cast<size_t>(beg)*SAMPLE_RATE,
            static_cast<size_t>(end)*SAMPLE_RATE
        );
    }

    const char* MixConverterCreator::getName() const { return "mix"; }
    const char* MixConverterCreator::getHelp() const {
        return "mix converter: "
            "Mixes stream from previous converter and mixes with input stream\n"
            "configuration: mix $<stream number> <begin>[=0] <end>[=0]\n";
    }

}