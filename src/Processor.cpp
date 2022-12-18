#include <fstream>
#include <sstream>
#include "Processor.hpp"
#include "MuteConverter.hpp"
#include "MixConverter.hpp"
#include "VolumeConverter.hpp"

using namespace std;

namespace {

    SoundProcessor::MuteConverterCreator mucc;
    SoundProcessor::MixConverterCreator micc;
    SoundProcessor::VolumeConverterCreator vcc;

    const vector<SoundProcessor::ConverterCreator*> USABLE_CONVERTERS = {
        &mucc,
        &micc,
        &vcc
    };

}

namespace SoundProcessor {
    
    inline shared_ptr<Converter> Configuration::converterParser(
        const string& name,
        const vector<int>& iargs,
        const vector<string>& streams,
        const vector<size_t>& streamIdxs
    ) {
        for (auto creator : creators) {
            if (0 == name.compare(creator->getName()))
                return creator->parse(iargs, streams, streamIdxs);
        }
        throw SoundProcessor::config_failure("Bad converter name");
    }

    const vector<ConverterCreator*>& Configuration::creators = USABLE_CONVERTERS;

    Configuration::Configuration(const string& filename, const vector<string>& streams) {
        ifstream f;
        f.exceptions( fstream::badbit | fstream::failbit );
        try {
            f.open(filename, ios::in);
        }
        catch (const fstream::failure& e) {
            throw fstream::failure("Cannot open configuration file");
        }
        try {
            readConfig(f, streams);
        }
        catch (const fstream::failure& e) {
            throw fstream::failure("Unexpected failure during reading configuration file");
        }
    }

    void Configuration::readConfig(ifstream& f, const vector<string>& streams) {
        while (!f.eof() && EOF != f.peek()) {
            stringstream ss;
            string name;
            getline(f, name);
            ss << name;
            ss >> name;
            if ('#' == name[0])
                continue;
            vector<int> iargs;
            vector<size_t> inIdxs;
            string buf;
            while (!ss.eof()) {
                ss >> buf;
                if (buf[0] == '$') {
                    buf.erase(0, 1);
                    size_t id = 0;
                    try {
                        id = stoul(buf);
                    }
                    catch (const exception&) {
                        throw config_failure("Bad stream number after $");
                    }
                    if (id >= streams.size() + 1 || 0 == id)
                        throw config_failure("Bad stream number after $");
                    inIdxs.push_back(id - 1);
                }
                else if (!buf.empty()) {
                    try {
                        iargs.push_back(stoi(buf));
                    }
                    catch (const exception&) {
                        throw config_failure("Bad not integer argument for converter");
                    }
                }
            }
            if (!isspace(name[0]))
                converters.push_back(converterParser(name, iargs, streams, inIdxs));
        }
    }

    void Processor::run() {
        size_t size;
        size_t i = 0;
        int16_t second[SoundProcessor::SAMPLE_RATE];
        const Configuration::converterVector& converters = config.getConverters();
        while (0 != (size = iss.read(second, SoundProcessor::SAMPLE_RATE)))
        {
            for (auto converter : converters)
                converter->convert(second, size, i);
            oss.write(second, size);
            i += size;
        }
    }

}
