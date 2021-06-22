#include <array>
#include <bit>
#include <cstddef>
#include <limits>
#include <map>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "formatters.hpp"
#include "utility.hpp"

using namespace tools;
using std::string;
using std::unordered_map;
using std::vector;
// class Solution {
// public:
//     class myTree {
//     private:
//         void insertLetterOrPointEdge(char c, myTree* node) {
//             node->edge[ c ] = new myTree{};
//             edge[ c ]->father = node;
//             node = edge[ c ];
//         }
//         void insertLetterStarEdge(char c, myTree* node) {
//             node->edge[ c ] = node;
//         }
//         void insertPointStarEdge(char c, myTree* node) {
//             node->edge[ '.' ] = node;
//             node->edge[ c ] = new myTree{};
//             edge[ c ]->father = node;
//             edge[ c ]->edge[ c ] = edge[ c ]->father;
//             node = edge[ c ];
//         }

//     public:
//         bool state = false;
//         myTree* father;
//         unordered_map<char, myTree*> edge;

//         void getPattern(string pattern) {
//             int n = pattern.size();
//             myTree* node = this;
//             for (int i = 0; i < n - 1;) {
//                 if (pattern[ i ] == '.' && pattern[ i + 1 ] == '*') {
//                     if (i + 2 < n && pattern[ i + 2 ] != '.') {
//                         insertPointStarEdge(pattern[ i + 2 ], node);
//                         i += 3;
//                     } else {
//                     }
//                 } else if (pattern[ i + 1 ] == '*') {
//                     insertLetterStarEdge(pattern[ i ], node);
//                     std::cout<<node<<"    "<<node->edge[ 'a' ];
//                     i += 2;
//                 } else {
//                     insertLetterOrPointEdge(pattern[ i ], node);
//                     i++;
//                 }
//             }
//             if (pattern[ n - 1 ] != '.' && node->edge.find(pattern[ n - 1 ]) == edge.end()) {
//                 node->edge[ pattern[ n - 1 ] ] = new myTree{};
//                 node = edge[ pattern[ n - 1 ] ];
//             }
//             node->state = true;
//             std::cout << node << "    " << node->edge[ 'a' ];
//         }
//         bool isMatch(string s) {
//             myTree* node = this;
//             int n = s.size();
//             for (int i = 0; i < n; i++) {
//                 if (node->edge.count(s[ i ])) {
//                     node = node->edge[ s[ i ] ];
//                 } else if (node->edge.count('.')) {
//                     node = node->edge[ '.' ];
//                 } else {
//                     return false;
//                 }
//             }
//             return node->state;
//         }
//     };
//     bool isMatch(string s, string p) {
//         myTree map{};
//         map.getPattern(p);
//         return map.isMatch(s);
//     }
// };

struct GraphNode {
    constexpr static char Any = std::numeric_limits<char>::max();
    constexpr static char End = std::numeric_limits<char>::min();
    unordered_map<char, GraphNode*> edge;
    auto haveEdge(char ch) {
        auto it = edge.find(ch);
        auto itAny = edge.find(GraphNode::Any);
        return std::make_pair(it != edge.end() ? it->second : nullptr, itAny != edge.end() ? itAny->second : nullptr);
    }
};

class NFAGraph {
    GraphNode* startNode;

    static void addRepeatNode(GraphNode*& thisNode, char ch) {
        auto newNode = new GraphNode;
        newNode->edge[ ch ] = newNode;
        if (ch == '.') {
            thisNode->edge[ GraphNode::Any ] = newNode;
        } else {
            thisNode->edge[ ch ] = newNode;
        }
        thisNode = newNode;
    }

    static void addSingleNode(GraphNode*& thisNode, char ch) {
        auto newNode = new GraphNode;
        if (ch == '.') {
            thisNode->edge[ GraphNode::Any ] = newNode;
        } else {
            thisNode->edge[ ch ] = newNode;
        }
        thisNode = newNode;
    }

    bool dfs(GraphNode* startNode, auto pChar, auto pEnd) {
        if (pChar == pEnd) {
            if (startNode->edge.find(GraphNode::End) != startNode->edge.end()) {
                return true;
            }
            return false;
        }
        auto [ loopEdge, nextEdge ] = startNode->haveEdge(*pChar);
        if (loopEdge == nullptr && nextEdge == nullptr) {
            return false;
        }
        return dfs(loopEdge, pChar + 1, pEnd) || dfs(nextEdge, pChar + 1, pEnd);
    }

public:
    NFAGraph(const std::string& pattern)
        : startNode(new GraphNode) {
        GraphNode* thisNode = startNode;
        for (int i = 0; i < pattern.size(); ++i) {
            if (i + 1 < pattern.size() && pattern[ i + 1 ] == '*') {
                addRepeatNode(thisNode, pattern[ i ]);
                ++i;
            } else {
                addSingleNode(thisNode, pattern[ i ]);
            }
        }
        thisNode->edge[ GraphNode::End ] = nullptr;
    }

    bool match(const std::string& str) {
        return dfs(this->startNode, str.begin(), str.end());
    }
};
static std::array<std::vector<std::string>, 10> generateTable() {
    auto table = std::array<std::vector<std::string>, 10>{};
    auto compute = [](unsigned int turnedOn) {
        vector<std::string> ans;
        for (unsigned int h = 0; h < 12; ++h) {
            for (unsigned int m = 0; m < 60; ++m) {
                if (std::popcount(h) + std::popcount(m) == turnedOn) {
                    ans.push_back(std::to_string(h) + ":" + (m < 10 ? "0" : "") + std::to_string(m));
                }
            }
        }
        return ans;
    };
    for (unsigned int i = 0; i < table.size(); ++i) {
        table[ i ] = compute(i);
    }
    return table;
}
auto table=generateTable();
class Solution {
public:
    vector<string> readBinaryWatch(int turnedOn) {
        return table[turnedOn];
    }
};
int main() {
     tools::println("{}", Solution{}.readBinaryWatch(10)));
}
