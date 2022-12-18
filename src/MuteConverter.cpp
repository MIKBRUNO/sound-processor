#include "MuteConverter.hpp"
#include "Converter.hpp"

using namespace std;

namespace SoundProcessor {

    void MuteConverter::convert(int16_t* block, size_t blockSize, size_t blockBeg) {
        if ((blockBeg >= muteEnd && 0 != muteEnd) || blockBeg + blockSize < muteBeg)
            return;
        size_t i = muteBeg < blockBeg ? 0 : muteBeg - blockBeg;
        for (;i < blockSize && (i < muteEnd - blockBeg || 0 == muteEnd); ++i)
            block[i] = 0;
    }

    shared_ptr<Converter> MuteConverterCreator::parse (
        const std::vector<int>& iargs,
        const std::vector<std::string>& files,
        const std::vector<size_t>& fileidxs
    ) const {
        if (iargs.size() > 2 || fileidxs.size() != 0)
            throw config_failure("Bad number of arguments for mute converter");
        int beg = iargs.size() >= 1 ? iargs[0] : 0;
        int end = iargs.size() == 2 ? iargs[1] : 0;
        if (beg < 0 || end < 0)
            throw config_failure("Bad arguments for mute converter");
        return make_shared<MuteConverter>(
            static_cast<size_t>(beg)*SAMPLE_RATE,
            static_cast<size_t>(end)*SAMPLE_RATE
        );
    }

    const char* MuteConverterCreator::getHelp() const {
        return "mute converter: "
            "Mutes stream from previous converter at interval\n"
            "configuration: mute <begin>[=0] <end>[=0]\n";
    }

    const char* MuteConverterCreator::getName() const { return "mute"; }

}