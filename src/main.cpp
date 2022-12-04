#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

namespace {

    void getOpt(ofstream& out, ifstream& conf, vector<ifstream>& in, int argc, char* argv[]) {
        unsigned int argi = 1;
        enum { FIRST, CONF, OUT, IN } state = FIRST;
        while (argi < argc) {
            if (state == FIRST)
                if (0 == strcmp("-c", argv[argi])) {
                    conf.open(argv[++argi], ios::in);
                    state = OUT;
                }
                else {
                    out.open(argv[argi], ios::out | ios::binary);
                    state = CONF;
                }
            else if (state == CONF)
                if (0 == strcmp("-c", argv[argi])) {
                    conf.open(argv[++argi], ios::in);
                    state = IN;
                }
                else {
                    in.push_back(ifstream(argv[argi], ios::binary | ios::in));
                    state = CONF;
                }
            else if (state == OUT) {
                out.open(argv[argi], ios::out | ios::binary);
                state = IN;
            }
            else if (state == IN) {
                in.push_back(ifstream(argv[argi], ios::binary | ios::in));
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
    ofstream out;
    ifstream conf;
    vector<ifstream> input;
    getOpt(out, conf, input, argc, argv);
    try {
        out.exceptions( ios::failbit | ios::badbit );
        conf.exceptions( ios::failbit | ios::badbit );
        for (unsigned int i = 0; i < input.size(); ++i)
            input[i].exceptions( ios::badbit | ios::failbit );
    }
    catch (const ios::failure& e) {
        cerr << "Failed to open files." << endl;
        return 1;
    }
    if (!out.is_open()) {
        cerr << "No output file!" << endl;
        return 1;
    }
    if (!conf.is_open()) {
        cerr << "Configuration file is required!" << endl;
        return 1;
    }
    if (0 == input.size()) {
        cerr << "No input files!" << endl;
        return 1;
    }

    
    
    return 0;
}
