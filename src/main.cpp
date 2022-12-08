#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdint>
#include "SampleStream.hpp"

using namespace std;

namespace {

    void getOpt(string& out, string& conf, vector<string>& in, int argc, char* argv[]) {
        unsigned int argi = 1;
        enum { FIRST, CONF, OUT, IN } state = FIRST;
        while (argi < argc) {
            if (state == FIRST)
                if (0 == strcmp("-c", argv[argi])) {
                    conf = argv[++argi];
                    state = OUT;
                }
                else {
                    out = argv[argi];
                    state = CONF;
                }
            else if (state == CONF)
                if (0 == strcmp("-c", argv[argi])) {
                    conf = argv[++argi];
                    state = IN;
                }
                else {
                    in.push_back(argv[argi]);
                    state = CONF;
                }
            else if (state == OUT) {
                out = argv[argi];
                state = IN;
            }
            else if (state == IN) {
                in.push_back(argv[argi]);
                state = IN;
            }

            ++argi;
        }
    }

}

int main(int argc, char* argv[]) {
    if (argc > 1 && 0 == strcmp(argv[1], "-h")) {
        cout << "help message" << endl;
        return 0;
    }
    string out = "";
    string conf = "";
    vector<string> input;
    getOpt(out, conf, input, argc, argv);
    if (out == "") {
        cerr << "No output file!" << endl;
        return 1;
    }
    if (conf == "") {
        cerr << "Configuration file is required!" << endl;
        return 1;
    }
    if (0 == input.size()) {
        cerr << "No input files!" << endl;
        return 1;
    }

    int16_t second[44100];
    try {
        SoundProcessor::iSampleStream iss { input[0] };
        SoundProcessor::oSampleStream oss { out };
        size_t size;
        while (0 != (size = iss.read(second, 44100)))
        {
            oss.write(second, size);
        }
        
    }
    catch (const SoundProcessor::wav_failure& e) {
        cerr << e.what() << endl;
        return 2;
    }
    catch (const fstream::failure& e) {
        cerr << e.what() << endl;
        return 1;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 3;
    }

    
    return 0;
}
