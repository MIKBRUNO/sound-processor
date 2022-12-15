#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdint>
#include "Processor.hpp"

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
    string out;
    string conf;
    vector<string> input;
    getOpt(out, conf, input, argc, argv);
    if (out.compare("") == 0) {
        cerr << "No output file!" << endl;
        return 1;
    }
    if (conf.compare("") == 0) {
        cerr << "Configuration file is required!" << endl;
        return 1;
    }
    if (0 == input.size()) {
        cerr << "No input files!" << endl;
        return 1;
    }

    try {
        SoundProcessor::Configuration configuration { conf, input };
        SoundProcessor::Processor proc { configuration, out, input[0] };
        proc.run();
    }
    catch (const SoundProcessor::wav_failure& e) {
        cerr << e.what() << endl;
        return 3;
    }
    catch (const fstream::failure& e) {
        cerr << e.what() << endl;
        return 2;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    
    return 0;
}
