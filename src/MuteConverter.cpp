#include "MuteConverter.hpp"
#include "Converter.hpp"

using namespace std;

namespace SoundProcessor {

    void MuteConverter::convert(int16_t* block, size_t blockSize, size_t blockBeg) {
        if (blockBeg >= muteEnd || blockBeg + blockSize < muteBeg)
            return;
        size_t i = muteBeg < blockBeg ? 0 : muteBeg - blockBeg;
        for (;i < blockSize && i < muteEnd - blockBeg; ++i)
            block[i] = 0;
    }

    shared_ptr<Converter> muteConverterParser (
        const std::vector<int>& iargs,
        const std::vector<std::string>& files,
        const std::vector<size_t>& fileidxs
    ) {
        if (iargs.size() != 2 || fileidxs.size() != 0)
            throw config_failure("Bad number of arguments for mute converter");
        if (iargs[0] < 0 || iargs[1] < 0)
            throw config_failure("Bad arguments for mute converter");
        MuteConverterCreator mcc(iargs[0], iargs[1]);
        return mcc.create();
    }

}