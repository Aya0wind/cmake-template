#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <queue>
#include <string>
#include <set>
#include <vector>
#include <stack>
#include "utility.hpp"


using State = int;

using CharT = char;

using StateSet = std::set<State>;

using NFATransfer = std::array<StateSet, 27>;

using NFATable = std::vector<NFATransfer>;

constexpr inline size_t EPSILION_INDEX = 26;

using DFATransfer = std::array<State, 27>;

using DFATable = std::vector<DFATransfer>;

constexpr inline size_t IS_FINALSTATE_INDEX = 26;

enum class Type : CharT { Single, Repeat };

using Token = std::pair<Type, CharT>;

namespace std {
template <class CharT>
struct [[maybe_unused]] ::std::formatter<Type, CharT> {
    [[maybe_unused]] [[nodiscard, maybe_unused]] auto parse(format_parse_context& ctx) {
        return ctx.end();
    }
    template <typename OutputIt>
    [[nodiscard, maybe_unused]] auto
    format(const Type& value, std::basic_format_context<OutputIt, char>& ctx) const noexcept {
        auto output = ctx.out();
        std::string valueString;
        if (value == Type::Repeat) {
            valueString = "Repeat";
        } else {
            valueString = "Single";
        }
        output = std::copy(valueString.begin(), valueString.end(), output);
        return output;
    }
};

}

class NFA{

    void addSimpleSubGraph(char ch) {
        int lastStateNumber = static_cast<int>(table.size());
        table.emplace_back();
        table.emplace_back();
        auto& head=table[lastStateNumber];
        auto& tail=table[lastStateNumber+1];
        if (ch == '.') {
            std::for_each(head.begin(), head.end()-1,
                [lastStateNumber](auto&states){states.insert(lastStateNumber);}
            );
        }else{
            head[ ch - 'a' ].insert(lastStateNumber+1);
        }
        tail[ EPSILION_INDEX ].insert(lastStateNumber + 2);
    }

    void addRepeatSubGraph(char ch) {
        int lastStateNumber = static_cast<int>(table.size());
        table.emplace_back();
        table.emplace_back();
        table.emplace_back();
        auto& headAndTail = table[lastStateNumber];
        auto& left = table[lastStateNumber+1];
        auto& right = table[lastStateNumber+2];
        if (ch == '.') {
            std::for_each(left.begin(), left.end()-1,
                [lastStateNumber](auto&states){states.insert(lastStateNumber+1);}
            );
        }else{
            left[ ch - 'a' ].insert(lastStateNumber+2);
        }
        right[ EPSILION_INDEX ].insert(lastStateNumber);
        headAndTail[ EPSILION_INDEX ].insert(lastStateNumber + 1);
        headAndTail[ EPSILION_INDEX ].insert(lastStateNumber + 3);
    }

public:
    NFATable table;
    NFA(const std::vector<Token>& tokens){
        for (auto&& token : tokens) {
            if (token.first == Type::Repeat) {
                addRepeatSubGraph(token.second);
            } else {
                addSimpleSubGraph(token.second);
            }
        }
    }

    StateSet epsilonClosure(const State& state){
        StateSet result(table[state][EPSILION_INDEX]);
        std::vector<State> stack(table[state][EPSILION_INDEX].begin(),table[state][EPSILION_INDEX].end());
        while (!stack.empty()) {
            auto next=stack.back();
            stack.pop_back();
            auto newStateSet=epsilonClosure(next);
            for(auto&&e:newStateSet){
                stack.push_back(e);
            }
            result.merge(newStateSet);
        }
        return result;
    }

    StateSet moveClosure(const StateSet& closure,CharT ch){
        StateSet result(table[state][EPSILION_INDEX]);
        std::vector<State> stack(table[state][EPSILION_INDEX].begin(),table[state][EPSILION_INDEX].end());
        while (!stack.empty()) {
            auto next=stack.back();
            stack.pop_back();
            auto newStateSet=epsilonClosure(next);
            for(auto&&e:newStateSet){
                stack.push_back(e);
            }
            result.merge(newStateSet);
        }
        return result;
    }

    State finalState(){
        return static_cast<State>(table.size());
    }

    void debugPrint(){
        for (auto&&line : table) {
            tools::println("{}\n", line);
        }
    }

};


class Regex {
    DFATable dfaTable;


    void initializeDFAFromNFA(NFA&nfa){
        State initialState=0;
        State finalState=nfa.finalState();
        std::queue<StateSet> newStates;
        std::map<StateSet, State> newStateMap;
        newStates.push(nfa.epsilonClosure(initialState));
        while (!newStates.empty()) {
            auto newState = std::move(newStates.front());
            newStates.pop();
            for(char i = 'a';i!='z';++i){
                auto moveClousure=nfa.moveClosure(newState, i);
                if(!moveClousure.empty()){
                    auto it = newStateMap.find(moveClousure);
                    if(it!=newStateMap.end()){
                        newStateMap.emplace(moveClousure,dfaTable.size());
                        newStates.emplace(std::move(moveClousure));
                        auto& thisState =  dfaTable.emplace_back();
                        thisState[i]
                    }else{
                        State state = it->second;
                        
                    }
                    
                   
                }
            }
        }
    }

public:
    Regex(const std::string& pattern) {
        auto tokens = parserTokens(pattern);
        NFA nfa{tokens};
        initializeDFAFromNFA(nfa);
    }

    static std::vector<Token> parserTokens(const std::string& pattern) {
        std::vector<Token> result;
        for (int i = 0; i < pattern.size(); ++i) {
            if (i + 1 < pattern.size() && pattern[ i + 1 ] == '*') {
                result.emplace_back(Type::Repeat, pattern[ i ]);
                ++i;
            } else {
                result.emplace_back(Type::Single, pattern[ i ]);
            }
        }
        return result;
    }

    bool isMatch(const std::string& input){
        return true;
    }

};
