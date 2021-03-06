#ifndef _NOT_ANOTHER_CLUSTERFUCK
#define _NOT_ANOTHER_CLUSTERFUCK

#include <list>
#include <map>
#include <iostream>
#include <regex>

namespace unifuck{
enum instruction   {NULL_INSTRUCTION, INCREMENT_POINTER, DECREMENT_POINTER,
                    INCREMENT_DATA, DECREMENT_DATA, 
                    OUTPUT_DATA, INPUT_DATA,
                    JUMP_FORWARD, JUMP_BACKWARD
                   };

}

namespace _unifuck_internals {

typedef char cell_data; // The data type used by the program data cells
typedef std::list<cell_data> data_cells;


typedef std::list<unifuck::instruction> script;
typedef script::const_iterator code_point;
}

namespace unifuck{
class interpreter {
public:
    // constructs an interpreter for a language spesified as a map from 
    // keywords to instructions. An istream and/or stream can also be
    // spesified to be used for the language's io operations, using
    // stdin and stdout by default
    interpreter(std::map<std::string,instruction>, std::istream& = std::cin,
                                           std::ostream& = std::cout);
    // reads and runs source code given as either an string or input stream
    void run(std::istream &);
    void run(std::string);
    // runs the interpreter as an interactive prompt
    void live_run();
private:
    std::istream &input_stream;
    std::ostream &output_stream = std::cout;

    std::map<std::string, instruction> keywords;
    _unifuck_internals::data_cells program_data;
    _unifuck_internals::data_cells::iterator data_pointer;

    void reset_data();
    
    inline void null_instruction();
    inline void increment_pointer();
    inline void decrement_pointer();
    inline void increment_data();
    inline void decrement_data();
    inline void output_data();
    inline void input_data();
    inline void jump_forward(_unifuck_internals::code_point&,
                        const _unifuck_internals::code_point&) const;
    inline void jump_backward(_unifuck_internals::code_point&, 
                        const _unifuck_internals::code_point&) const;

    _unifuck_internals::script read_code(const std::string &) const;
    inline instruction get_current_instruction(
                        const _unifuck_internals::code_point&) const;
    inline instruction get_next_instruction(
                        _unifuck_internals::code_point &, 
                        const _unifuck_internals::code_point&) const;
    inline instruction get_previous_instruction(
                        _unifuck_internals::code_point&, 
                        const _unifuck_internals::code_point&) const;
    const std::regex keyword_regex;
};

}

#endif
