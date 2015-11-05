// runs unit tests on unifuck interpreter

#include <iostream>
#include <string>
#include <map>
#include <regex>

#include "unifuck.h"
#include "unifuck.cpp"

using namespace unifuck;
using namespace _unifuck_internals;

using std::regex;           using std::smatch;
using std::sregex_iterator;
using std::map;             using std::string;
using std::cout;            using std::endl;
using std::clog;
using std::string;

bool test_sanatize_for_regex(){
    clog << "testing string sanatizing" << endl;
    string test_string = "****! What the hell? Is this some sort of (badly "
                         "written) test? Yes\\No.";
    string result_string ="\\*\\*\\*\\*! What the hell\\? Is this some sort"
                          " of \\(badly written\\) test\\? Yes\\\\No\\.";
    bool pass = sanatize_for_regex(test_string) == result_string;
    if(pass){
        clog << "passed" << endl;
    }
    else{
        clog << "failed\nTrial string\n" << test_string <<"result\n"
             << result_string << endl;
    }
    return pass;
}

bool test_generate_key_regex(){//TODO properly test regex special charaters
    clog <<"testing regex generation" << endl;

    // Set up test and expected result
    map<string, instruction> test_language;
    test_language["mary"] = NULL_INSTRUCTION;
    test_language["lamb."] = INCREMENT_POINTER;
    test_language["white"] = DECREMENT_POINTER;
    test_language["snow"] = INCREMENT_DATA;
    
    regex expected_result("(mary)|(lamb\\.)|(white)|(snow)");

    regex resulting_regex = generate_key_regex(test_language);

    string test_string("mary had a little lamb. Its fleece was "
                       "white as snow");

    // test wheter the regex produces gives the correct result
    sregex_iterator test_match(test_string.begin(), test_string.end(), resulting_regex);
    sregex_iterator expected_match(test_string.begin(), test_string.end(),  expected_result);

    bool pass = true;
    for( auto test_check = test_match, expected_check = expected_match;
    test_check != sregex_iterator() && expected_check != sregex_iterator(); 
    ++test_check, ++expected_check){
        if(test_check->str() != expected_check->str()){
            pass = false;
            break;
        }
    }

    // log result
    if(pass){
        clog << "passed" << endl;
    }
    else {
        clog << "failed\nexpected matches\n";
        for(auto m = *expected_match; expected_match != sregex_iterator();
                 m = *(++expected_match))
            clog << m.str() << "-";
        clog << "\nmatches produced\n";
        for(auto m = *test_match; test_match != sregex_iterator(); 
                 m = *(++test_match))
            clog << m.str() << "-";
        clog << endl;
    }
    
    return pass;
}

bool test_run_interpreter(){
    bool pass = true;//TODO write this test
    return pass;
}

int main(int argc, char **argv){
    bool all_passed = true;
    cout << "starting tests"<<endl;

    all_passed &= test_sanatize_for_regex();
    all_passed &= test_generate_key_regex();
    all_passed &= test_run_interpreter();

    cout << "tests complete\n";
    if(all_passed)
        cout << "All tests passed successfully" <<endl;
    else
        cout << "Not all tests were passed. see log for details"; 
    return 0;
}
