// A brainfuck interpreter written using unifuck

#include "unifuck.h"

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace unifuck;

using std::map;             using std::string;
using std::ifstream;        using std::runtime_error;
using std::cerr;            using std::endl;

int main(int argc, char** argv){
    
    // The brainfuck language is defined
    map<string, instruction> brainfuck_language;

    brainfuck_language[">"] = INCREMENT_POINTER;
    brainfuck_language["<"] = DECREMENT_POINTER;
    brainfuck_language["+"] = INCREMENT_DATA;
    brainfuck_language["-"] = DECREMENT_DATA;
    brainfuck_language["."] = OUTPUT_DATA;
    brainfuck_language[","] = INPUT_DATA;
    brainfuck_language["["] = JUMP_FORWARD;
    brainfuck_language["]"] = JUMP_BACKWARD;

    interpreter brainfuck_intepreter(brainfuck_language);

    if(argc > 1){
        for(int i = 1; i < argc; ++i){
            ifstream source_file(argv[i]);

            if(source_file){
                brainfuck_intepreter.run(source_file);
            }
            else
                cerr << "Unable to open file " << argv[i] << endl;

            source_file.close();
        }
    }    
    else
        brainfuck_intepreter.live_run();

    return 0;
}
