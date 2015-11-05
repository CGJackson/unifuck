// Contains an interpreter class for brainfuck synonyms

#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <regex>

#include "clusterfuck.h"

using namespace clusterfuck;
using namespace _clusterfuck_internals;

using std::logic_error; using std::domain_error;
using std::range_error;
using std::map;         using std::string;
using std::list;        using std::find;
using std::regex;       using std::sregex_iterator;
using std::cin;         using std::cout;
using std::getline;     using std::istream;
using std::ostream;     using std::flush;
using std::endl;        using std::cerr;


// Takes a string and adds escapes to regex special characters, so that it
// can be used as a search pattern
string sanatize_for_regex(string text){
    static const list<char> SPECIAL_CHARACTERS = {
            '^','$','\\','.','*','+','?','(',')','[',']','{','}','|'
                                                 };
    for(auto it = text.begin(); it < text.end(); ++it){
        if(find(SPECIAL_CHARACTERS.begin(), SPECIAL_CHARACTERS.end(), *it)
            != SPECIAL_CHARACTERS.end()){
            it = text.insert(it, '\\') + 1;
            
        }
    }
    return text;
}

// Takes a map and produces a regex that accepts its keys
template <class mapped_type>
regex generate_key_regex(const map<string, mapped_type> &key_map){
    list<string> key_patterns;
    string pattern;
    for(auto &value: key_map){
        key_patterns.push_back(sanatize_for_regex(value.first));
    }
    for(auto it = key_patterns.begin(); it != key_patterns.end();){
        pattern += "(" + *it + ")";

        if(++it != key_patterns.end())
            pattern += "|";
    }
    
    return regex(pattern);
}


interpreter::interpreter(map<string,instruction> language_specs, 
                                istream &i_stream,
                                ostream &o_stream): 
                                input_stream(i_stream),
                                output_stream(cout),
                                keywords(language_specs),
                                program_data(1),
                                data_pointer(program_data.begin()),
                                keyword_regex(generate_key_regex(keywords))
    {}

// A series of functions to exercute the keywords of the language
inline void interpreter::null_instruction(){
    throw logic_error("Null instruction");
}

inline void interpreter::increment_pointer(){
    if(++data_pointer == program_data.end()){
        program_data.push_back('\0');
        data_pointer = program_data.end();
        --data_pointer;
        
    }
    return;
}

inline void interpreter::decrement_pointer(){
    if(data_pointer == program_data.begin()){
        throw range_error("Cannot access data cell before the first");
    }
    else{
        --data_pointer;
    }
    return;
}

inline void interpreter::increment_data(){
    ++(*data_pointer);
}

inline void interpreter::decrement_data(){
    --(*data_pointer);
}

inline void interpreter::output_data(){
    output_stream << *data_pointer << flush;
}

inline void interpreter::input_data(){
    input_stream >> *data_pointer;
}

inline void interpreter::jump_forward(code_point& code, 
                                      const code_point& end) const{
    if(!*data_pointer){
        int internal_brackets = 0;
        for(instruction next_instruction = get_next_instruction(code, end);
                (internal_brackets != 0) || 
                next_instruction != JUMP_BACKWARD;
                next_instruction = get_next_instruction(code, end)){
        
            if (code == end)
                throw domain_error("unmatched jump forward");
            
            if (next_instruction == JUMP_FORWARD){
                ++internal_brackets;
            }
            else if(next_instruction == JUMP_BACKWARD){
                --internal_brackets;
            }
        }
    }
    return;
}

inline void interpreter::jump_backward(code_point& code, 
                                       const code_point& begin) const{
    if(*data_pointer){
        int internal_brackets = 0;
        for(instruction previous_instruction = 
                get_previous_instruction(code, begin);
                (internal_brackets != 0) || 
                previous_instruction != JUMP_FORWARD;
                previous_instruction=get_previous_instruction(code, begin)){
        
            if (code == begin)
                throw domain_error("unmatched jump begin");
            
            if (previous_instruction == JUMP_BACKWARD){
                ++internal_brackets;
            }
            else if(previous_instruction == JUMP_FORWARD){
                --internal_brackets;
            }
        }
    }
    
    return;
}

script interpreter::read_code(const string &code) const{
    script code_instructions;
    
    const sregex_iterator MATCH_END;
    for(sregex_iterator 
            keyword_matches(code.begin(),code.end(),keyword_regex); 
            keyword_matches != MATCH_END; ++keyword_matches){
        code_instructions.push_back(keywords.at(keyword_matches->str()));
    }
    return code_instructions;
}

//returns the current instruction at a particular control point
inline instruction interpreter::get_current_instruction(
                                    const code_point &control_point) const{
    return *control_point;
}

// returns the instruction pf control_point  and then increments on to the
// next code point, unless control_point == end, in which case the 
// NULL_INSTRUCTION is returned
inline instruction interpreter::get_next_instruction(
            code_point &control_point, const code_point &end) const{
    if(++control_point != end){
        return *(control_point);
    }
    else
        return NULL_INSTRUCTION;
}

// returns the instruction of control_point and decrements the
// code point, unless control_point == begin, in which case the 
// NULL_INSTRUCTION is returned
inline instruction interpreter::get_previous_instruction(
            code_point& control_point, const code_point& begining) const{
    
    if(control_point != begining){
        return *(--control_point);
    }
    else
        return NULL_INSTRUCTION;
}


void interpreter::run(string code){

    const script code_instructions = read_code(code);
    
    if( code_instructions.size() == 0){
        throw domain_error("Source code was empty");
    }

    const code_point  CODE_START = code_instructions.begin();
    const code_point  CODE_END = code_instructions.end();

    code_point control_point = CODE_START;

   for(instruction current_instruction = 
                get_current_instruction(control_point);
                current_instruction != NULL_INSTRUCTION;
                current_instruction = 
                        get_next_instruction(control_point, CODE_END)){

        switch (current_instruction){
            case NULL_INSTRUCTION:
                null_instruction();
                break;
            case INCREMENT_POINTER:
                increment_pointer();
                break;
            case DECREMENT_POINTER:
                decrement_pointer();
                break;
            case INCREMENT_DATA:
                increment_data();
                break;
            case DECREMENT_DATA:
                decrement_data();
                break;
            case OUTPUT_DATA:
                output_data();
                break;
            case INPUT_DATA:
                input_data();
                break;
            case JUMP_FORWARD:
                jump_forward(control_point, CODE_END);
                break;
            case JUMP_BACKWARD:
                jump_backward(control_point, CODE_START);
                break;
            default:
                throw logic_error("Current instruction not recognised");
        }
            #ifdef DEBUG
            std::cerr << std::endl << '\t';
            for(auto a : program_data)
                std::cerr <<(a < 100 ?( a < 10 ? "  ":" "):"") << (int) a;
            std::cerr << std::endl << '\t';
            int i=0;
            for(auto it = program_data.begin(); it != data_pointer; ++it)
                ++i;
            for(int j = 0; j < i; ++j)
                std::cerr << "   ";
            std::cerr << " ^" << std::endl;
            #endif
    } 
    //std::cerr << *data_pointer << std::endl;
    reset_data();
    output_stream << endl;
    return;
}

//TODO fix this mess
void interpreter::run(istream &code){
    string source, line;
    while(getline(code, line))
        source += line;
    
    run(source);
    return;
}

// Allows the interpreter to be run as an interactive prompt
void interpreter::live_run(){
    string line;
    while(getline(cin, line))
        run(line);
    return;
}

void interpreter::reset_data(){
    program_data = data_cells(1);
    data_pointer = program_data.begin();
}
