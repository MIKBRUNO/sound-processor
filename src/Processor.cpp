#include <fstream>
#include <sstream>
#include "Processor.hpp"
#include "MuteConverter.hpp"

using namespace std;

namespace {

    inline shared_ptr<SoundProcessor::Converter> ConverterFactory(
        const string& name,
        const vector<int>& iargs,
        const vector<string>& streams,
        const vector<size_t>& streamIdxs
    ) {
        if (0 == name.compare("mute")) {
            return SoundProcessor::muteConverterParser(iargs, streams, streamIdxs);
        }
        // else if
        else {
            throw SoundProcessor::config_failure("Bad converter name");
        }
    }

}

namespace SoundProcessor {
    
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
                        size_t id = stoul(buf);
                    }
                    catch (const exception&) {
                        throw config_failure("Bad stream number after $");
                    }
                    if (id >= streams.size() || 0 == id)
                        throw config_failure("Bad stream number after $");
                    inIdxs.push_back(id);
                }
                else {
                    try {
                        iargs.push_back(stoi(buf));
                    }
                    catch (const exception&) {
                        throw config_failure("Bad not integer argument for converter");
                    }
                }
            }
            if (!isspace(name[0]))
                converters.push_back(ConverterFactory(name, iargs, streams, inIdxs));
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
