#include "dfa.hpp"

int main(int argc, char* argv[])
{
    switch (auto homework = dfa("dfa"); argv[1][1])
    {
        case 'e': answer(homework.accepts_null_string()); break;
        case 'v': answer(homework.empty_language()); break;
        case 'f': answer(homework.finite_language()); break;
        case 'a': homework.print_accessible_states(); break;
        case 'u': homework.print_useful_states(); break;
        default: break;
    }
}