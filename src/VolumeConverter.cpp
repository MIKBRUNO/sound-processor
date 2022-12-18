#include <cmath>
#include "VolumeConverter.hpp"
#include "Converter.hpp"

using namespace std;

namespace SoundProcessor {

    void VolumeConverter::convert(int16_t* block, size_t blockSize, size_t blockBeg) {
        if ((blockBeg >= end && 0 != end) || blockBeg + blockSize < beg)
            return;
        double vol = volume;
        vol /= 100;
        size_t i = beg < blockBeg ? 0 : beg - blockBeg;
        for (;i < blockSize && (i < end - blockBeg || 0 == end); ++i) {
            double nwValue = block[i];
            nwValue *= vol;
            if (block[i] < 0)
                block[i] = (nwValue >= INT16_MIN) ? static_cast<int16_t>(nwValue) : INT16_MIN;
            else
                block[i] = (nwValue <= INT16_MAX) ? static_cast<int16_t>(nwValue) : INT16_MAX;
        }
    }

    shared_ptr<Converter> VolumeConverterCreator::parse (
        const std::vector<int>& iargs,
        const std::vector<std::string>& files,
        const std::vector<size_t>& fileidxs
    ) const {
        if (iargs.size() < 1 || iargs.size() > 3 || fileidxs.size() != 0)
            throw config_failure("Bad number of arguments for volume converter");
        int vol = iargs[0];
        int beg = iargs.size() >= 2 ? iargs[1] : 0;
        int end = iargs.size() == 3 ? iargs[2] : 0;
        if (beg < 0 || end < 0)
            throw config_failure("Bad arguments for volume converter");
        return make_shared<VolumeConverter>(
            static_cast<size_t>(vol),
            static_cast<size_t>(beg)*SAMPLE_RATE,
            static_cast<size_t>(end)*SAMPLE_RATE
        );
    }

    const char* VolumeConverterCreator::getHelp() const {
        return "volume converter: "
            "Multiplying samples in interval by a <volume>/100. <volume> also can be more then 100.\n"
            "configuration: vol <volume> <begin>[=0] <end>[=0]\n";
    }

    const char* VolumeConverterCreator::getName() const { return "vol"; }

}
