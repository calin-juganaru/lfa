#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <functional>

using namespace std;

// ================================================================================================

template <typename T1, typename T2>
struct hash<pair<T1, T2>>
{
    size_t operator()(const pair<T1, T2>& _pair) const
    {
        return hash<T1>()(_pair.first) ^ hash<T2>()(_pair.second);
    }
};

// ================================================================================================

auto contains = [](auto&& container, auto&& value)
{
    return (container.find(value) != end(container));
};

auto answer = [](const bool& value)
{
    cout << ((value) ? "Yes\n" : "No\n");
};

enum class COLOR { WHITE, GRAY, BLACK };

// ================================================================================================

struct dfa
{
private:
    int symbols; string initial_state;
    unordered_set<string> states, final_states;
    unordered_map<string, vector<string>> transitions;
    unordered_set<string> accessible_states, useful_states;

    bool reach_final_state(const string& state, unordered_set<string>& visited);
    bool has_cycles(const string& state, unordered_map<string, COLOR>& visited);
    void next_accessible_states(const string& state);
    void find_accessible_states();
    void find_useful_states();
    void parse_input(const string& file);

public:
    bool accepts_null_string();
    bool empty_language();
    bool finite_language();
    void print_accessible_states();
    void print_useful_states();

    dfa() = default;
    ~dfa() = default;
    dfa(const string& file)
    { parse_input(file); }
};

// ================================================================================================

void dfa::parse_input(const string& file)
{
    auto input = ifstream(file);
    auto character = char();
    auto buffer = string();

    // --------------------------------------------------------------------------------------------

    input >> character >> character;
    while (true)
    {
        input >> character;
        if (character == ',')
        {
            states.insert(move(buffer));
            buffer.clear();
        }
        else if (character == '}')
        {
            states.insert(move(buffer));
            buffer.clear(); break;
        }
        else buffer.push_back(character);
    }

    // --------------------------------------------------------------------------------------------

    input >> character >> character;
    while (character != '}')
        input >> character >> character;

    // --------------------------------------------------------------------------------------------

    input >> character;
    while (true)
    {
        input >> character;
        if (character == ')') break;
        else
        {
            input >> character;
            auto state_1 = string();
            auto state_2 = string();

            while (true)
            {
                input >> character;
                if (character == ',')
                {
                    state_1 = move(buffer);
                    buffer.clear(); break;
                }
                else buffer.push_back(character);
            }

            input >> character >> character;

            while (true)
            {
                input >> character;
                if (character == ')')
                {
                    state_2 = move(buffer);
                    buffer.clear(); break;
                }
                else buffer.push_back(character);
            }

            transitions[state_1].push_back(state_2);
        }
    }

    // --------------------------------------------------------------------------------------------

    input >> character;
    while (true)
    {
        input >> character;
        if (character == ',')
        {
            initial_state = move(buffer);
            buffer.clear(); break;
        }
        else buffer.push_back(character);
    }

    // -------------------------------------------------------------------------------------------

    input >> character;
    while (true)
    {
        input >> character;
        if (character == ',')
        {
            final_states.insert(move(buffer));
            buffer.clear();
        }
        else if (character == '}')
        {
            final_states.insert(move(buffer));
            buffer.clear(); break;
        }
        else buffer.push_back(character);
    }

    // --------------------------------------------------------------------------------------------

    input.close();
}

// ================================================================================================

bool dfa::accepts_null_string()
{
    return contains(final_states, initial_state);
}

// ================================================================================================

bool dfa::reach_final_state(const string& state, unordered_set<string>& visited)
{
    visited.insert(state);
    for (auto&& next_state: transitions[state])
        if (!contains(visited, next_state))
            if (contains(final_states, next_state) ||
                reach_final_state(next_state, visited))
                    return true;
    return false;
}

// ================================================================================================

bool dfa::empty_language()
{
    auto visited = unordered_set<string>();
    return !reach_final_state(initial_state, visited);
}

// ================================================================================================

bool dfa::has_cycles(const string& state, unordered_map<string, COLOR>& visited)
{
    visited[state] = COLOR::GRAY;

    for (auto&& next_state: transitions[state])
    {
        if (!contains(visited, next_state) &&
            has_cycles(next_state, visited)) return true;
        if (visited[next_state] == COLOR::GRAY &&
            contains(useful_states, next_state)) return true;
    }

    visited[state] = COLOR::BLACK;
    return false;
}

// ================================================================================================

void dfa::next_accessible_states(const string& state)
{
    accessible_states.insert(state);
    for (auto&& next_state: transitions[state])
        if (!contains(accessible_states, next_state))
            next_accessible_states(next_state);
}

// ================================================================================================

void dfa::find_accessible_states()
{
    next_accessible_states(initial_state);
}

// ================================================================================================

void dfa::find_useful_states()
{
    find_accessible_states();
    for (auto&& state: accessible_states)
    {
        auto visited = unordered_set<string>();
        if (contains(final_states, state) ||
            reach_final_state(state, visited))
            useful_states.insert(state);
    }
}

// ================================================================================================

bool dfa::finite_language()
{
    auto visited = unordered_map<string, COLOR>();
    if (empty_language()) return true;
    this->find_useful_states();
    return !(has_cycles(initial_state, visited));
}

// ================================================================================================

void dfa::print_accessible_states()
{
    this->find_accessible_states();
    for (auto&& state: accessible_states)
        cout << state << endl;
}

// ================================================================================================

void dfa::print_useful_states()
{
    this->find_useful_states();
    for (auto&& state: useful_states)
        cout << state << endl;
}

// ================================================================================================