#ifndef _NOT_ANOTHER_CLUSTERFUCK
#define _NOT_ANOTHER_CLUSTERFUCK

#include <list>
#include <map>
#include <iostream>
#include <regex>

namespace clusterfuck{
enum instruction   {NULL_INSTRUCTION, INCREMENT_POINTER, DECREMENT_POINTER,
                    INCREMENT_DATA, DECREMENT_DATA, 
                    OUTPUT_DATA, INPUT_DATA,
                    JUMP_FORWARD, JUMP_BACKWARD
                   };

}

namespace _clusterfuck_internals {

typedef char cell_data;
typedef std::list<cell_data> data_cells;


typedef std::list<clusterfuck::instruction> script;
typedef script::const_iterator code_point;
}

namespace clusterfuck{
class interpreter {
public:
    interpreter(std::map<std::string,instruction>, std::istream& = std::cin,
                                           std::ostream& = std::cout);
    void run(std::istream &);
    void run(std::string);
    void live_run();
private:
    std::istream &input_stream;
    std::ostream &output_stream = std::cout;

    std::map<std::string, instruction> keywords;
    _clusterfuck_internals::data_cells program_data;
    _clusterfuck_internals::data_cells::iterator data_pointer;

    void reset_data();
    
    inline void null_instruction();
    inline void increment_pointer();
    inline void decrement_pointer();
    inline void increment_data();
    inline void decrement_data();
    inline void output_data();
    inline void input_data();
    inline void jump_forward(_clusterfuck_internals::code_point&,
                        const _clusterfuck_internals::code_point&) const;
    inline void jump_backward(_clusterfuck_internals::code_point&, 
                        const _clusterfuck_internals::code_point&) const;

    _clusterfuck_internals::script read_code(const std::string &) const;
    inline instruction get_current_instruction(
                        const _clusterfuck_internals::code_point&) const;
    inline instruction get_next_instruction(
                        _clusterfuck_internals::code_point &, 
                        const _clusterfuck_internals::code_point&) const;
    inline instruction get_previous_instruction(
                        _clusterfuck_internals::code_point&, 
                        const _clusterfuck_internals::code_point&) const;
    const std::regex keyword_regex;
};

}

#endif
