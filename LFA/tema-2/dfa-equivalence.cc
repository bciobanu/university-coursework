#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <functional>

using namespace std;

class DFA {
  public:
    friend istream& operator >>(istream& in, DFA& rhs) {
        int num_states, num_transitions, num_final; 
        in >> num_states >> num_transitions >> num_final;

        rhs.final_states_.clear();
        rhs.delta_.clear();
        rhs.delta_.resize(num_states);

        for (int i = 0; i < num_final; ++i) {
            int state; in >> state;
            rhs.final_states_.insert(state);
        }

        for (int i = 0; i < num_transitions; ++i) {
            int s1, s2; char ch; cin >> s1 >> ch >> s2;
            rhs.delta_[s1][ch] = s2;
        }
        
        in >> rhs.initial_state_;

        return in;
    }

    bool Equivalent(const DFA& rhs) const {
        return SubsetOf(rhs) and rhs.SubsetOf(*this);
    }
    
    int size() const {
        return (int)delta_.size();
    }

  private:
    bool SubsetOf(const DFA& rhs) const {
        return not Intersection(rhs.Complement()).ReachesFinalState();
    }

    bool ReachesFinalState() const {
        vector<bool> marked(size());
        function<bool(const int)> Dfs = [&](const int node) {
            if (final_states_.find(node) != final_states_.end()) {
                return true;
            }

            marked[node] = true;
            for (auto&& it : delta_[node]) {
                if (not marked[it.second] and Dfs(it.second)) {
                    return true;
                }
            }
            return false;
        };

        return Dfs(initial_state_);
    }

    DFA Intersection(const DFA& rhs) const {
        int n = size(), m = rhs.size();
        DFA result;
        result.delta_.resize(n * m);
        result.initial_state_ = initial_state_ * m + rhs.initial_state_;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                for (auto&& iter : delta_[i]) {
                    if (rhs.delta_[j].find(iter.first) == rhs.delta_[j].end()) {
                        continue;
                    }
                    
                    result.delta_[i * m + j][iter.first] = iter.second * m + rhs.delta_[j].at(iter.first);
                }
                
                if (final_states_.find(i) != final_states_.end()
                        and rhs.final_states_.find(j) != rhs.final_states_.end()) {
                    result.final_states_.insert(i * m + j);        
                }
            }
        }
        return result;
    }

    DFA Complement() const {
        DFA result(*this);
        result.final_states_.clear();
        for (int i = 0; i < size(); ++i) {
            if (final_states_.find(i) == final_states_.end()) {
                result.final_states_.insert(result.final_states_.end(), i);
            }
        }
        return result;
    }

    set<int> final_states_;
    vector<map<char, int>> delta_;
    int initial_state_;
};

int main() {
    DFA a, b; cin >> a >> b;
    cout << a.Equivalent(b) << endl;
}

/*
3 6 2
1 2
0 a 2
0 b 1
1 a 1
1 b 2
2 a 2
2 b 2
0
2 4 1
1
0 a 1
0 b 1
1 a 1
1 b 1
0
*/
