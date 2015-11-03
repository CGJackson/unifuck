// A Ook interpreter written using clusterfuck

#include "clusterfuck.h"

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace clusterfuck;

using std::map;             using std::string;
using std::ifstream;        using std::runtime_error;
using std::cerr;            using std::endl;

int main(int argc, char** argv){
    
    // The Ook language is defined
    map<string, instruction> ook_language;

    ook_language["Ook. Ook?"] = INCREMENT_POINTER;
    ook_language["Ook? Ook."] = DECREMENT_POINTER;
    ook_language["Ook. Ook."] = INCREMENT_DATA;
    ook_language["Ook! Ook!"] = DECREMENT_DATA;
    ook_language["Ook! Ook."] = OUTPUT_DATA;
    ook_language["Ook. Ook!"] = INPUT_DATA;
    ook_language["Ook! Ook?"] = JUMP_FORWARD;
    ook_language["Ook? Ook!"] = JUMP_BACKWARD;

    interpreter ook_intepreter(ook_language);

    if(argc > 1){
        for(int i = 1; i < argc; ++i){
            ifstream source_file(argv[i]);

            if(source_file){
                ook_intepreter.run(source_file);
            }
            else
                cerr << "Unable to open file " << argv[i] << endl;

            source_file.close();
        }
    }    
    else
        ook_intepreter.live_run();

    return 0;
}
