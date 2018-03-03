#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <set>
using namespace std;

const int kSigma = 26 + 1;
const char kLambda = 'z' + 1;

class NFA {
  public:
    void AddTransition(const int s0, const int s1, const char key) {
        transitions_[FindStateIndex(s0)][Normalize(key)].insert(FindStateIndex(s1));
    }

    void MarkFinalState(const int s) { 
        is_final_state_[FindStateIndex(s)] = true; 
    }

    void SetInitialState(const int s) { 
        initial_state_ = FindStateIndex(s); 
    }

    bool Accepts(const string& word) const {
        set<int> state = move(LambdaClosure({initial_state_}));
        for (auto&& ch : word) {
            if (state.empty()) {
                return false;
            }

            set<int> next_state;
            for (auto&& iter : state) {
                for (auto&& adj : transitions_[iter][Normalize(ch)]) {
                    next_state.insert(adj);
                }
            }
            state = move(LambdaClosure(next_state));
        }
        for (auto&& iter : state) {
            if (is_final_state_[iter]) {
                return true;
            }
        }
        return false;
    }
  private:
    set<int> LambdaClosure(const set<int>& states) const {
        set<int> result;
        function<void(const int)> Df = [&](const int state) {
            if (result.find(state) != result.end()) {
                return;
            }

            result.insert(state);
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
        transitions_.emplace_back();
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

    vector<array<set<int>, kSigma>> transitions_;
    vector<bool> is_final_state_;
    map<int, int> states_idx_;
    int initial_state_;
};

int main() {
    NFA automata;
    automata.AddTransition(1, 2, 'l');
    automata.AddTransition(1, 2, kLambda);
    automata.MarkFinalState(2);
    automata.SetInitialState(1);

    cout << automata.Accepts("l") << endl;
    cout << automata.Accepts("") << endl;
    cout << automata.Accepts("abc") << endl;
    return 0;
}