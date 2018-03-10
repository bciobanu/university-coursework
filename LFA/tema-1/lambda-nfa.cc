#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

const int kSigma = 26 + 1;
const char kLambda = 'z' + 1;

class NFA {
  public:
    void AddTransition(const int s0, const int s1, const char key) {
        auto& vec = transitions_[FindStateIndex(s0)][Normalize(key)];
        if (find(vec.begin(), vec.end(), FindStateIndex(s1)) == vec.end()) {
            vec.push_back(FindStateIndex(s1));
        }
    }

    void MarkFinalState(const int s) { 
        is_final_state_[FindStateIndex(s)] = true; 
    }

    void SetInitialState(const int s) { 
        initial_state_ = FindStateIndex(s); 
    }

    bool Accepts(const string& word) const {
        vector<int> state = LambdaClosure({initial_state_});
        for (auto&& ch : word) {
            if (state.empty()) {
                return false;
            }

            vector<int> next_state;
            for (auto&& iter : state) {
                for (auto&& adj : transitions_[iter][Normalize(ch)]) {
                    if (find(next_state.begin(), next_state.end(), adj) == next_state.end()) {
                        next_state.push_back(adj);
                    }
                }
            }
            state = LambdaClosure(next_state);
        }
        for (auto&& iter : state) {
            if (is_final_state_[iter]) {
                return true;
            }
        }
        return false;
    }
  private:
    vector<int> LambdaClosure(const vector<int>& states) const {
        vector<int> result;
        function<void(const int)> Df = [&](const int state) {
            if (find(result.begin(), result.end(), state) != result.end()) {
                return;
            }

            result.push_back(state);
            for (auto&& adj : transitions_[state][Normalize(kLambda)]) {
                Df(adj);
            }
        };

        for (auto&& state : states) {
            Df(state);
        }
        return result;
    }

    int Normalize(const char key) const {
        return key - 'a';
    }

    int AddNewState() {
        const int res = static_cast<int>(transitions_.size());
        transitions_.push_back(vector<vector<int>>(kSigma));
        is_final_state_.emplace_back(false);
        return res;
    }

    int FindStateIndex(const int s) {
        auto states_idx_ptr = states_idx_.find(s);
        if (states_idx_ptr != states_idx_.end()) {
            return states_idx_ptr->second;
        }
        return states_idx_[s] = AddNewState();
    }

    vector<vector<vector<int>>> transitions_;
    vector<bool> is_final_state_;
    map<int, int> states_idx_;
    int initial_state_;
};

void SkipInputRow() {
    cin.ignore(256, '\n');
}

NFA Read() {
    for (int i = 0; i < 4; ++i) {
        SkipInputRow();
    }
    
    NFA automata;
    int initial_state; cin >> initial_state;
    automata.SetInitialState(initial_state);
    
    int num_final_states; cin >> num_final_states;
    for (int i = 0; i < num_final_states; ++i) {
        int final_state_idx; cin >> final_state_idx;
        automata.MarkFinalState(final_state_idx);
    }
    int num_transitions; cin >> num_transitions;
    for (int i = 0; i < num_transitions; ++i) {
        int s0, s1; char ch; cin >> s0 >> ch >> s1;
        if (ch == '.') {
            ch = kLambda;
        }

        automata.AddTransition(s0, s1, ch);
    }
    return automata;
}

int main() {
    NFA automata = Read();
    int num_queries; cin >> num_queries;
    while (num_queries--> 0) {
        string word; cin >> word;
        if (automata.Accepts(word)) {
            cout << "DA\n";
        } else {
            cout << "NU\n";
        }
    }
    return 0;
}
