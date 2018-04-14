#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

class DFA {
  public:
    friend istream& operator >>(istream& in, DFA& rhs) {
        int num_states; in >> num_states;
        vector<int> states(num_states);
        for (auto& iter : states) { in >> iter; }
        sort(states.begin(), states.end());
        rhs.delta_.resize(num_states);
        
        // ignore sigma information
        in.ignore(256, '\n');
        in.ignore(256, '\n'); 
        in.ignore(256, '\n');
        
        function<int(const int)> Relabel = [&](const int state) {
            return lower_bound(states.begin(), states.end(), state) - states.begin();    
        };
        
        in >> rhs.initial_state_; rhs.initial_state_ = Relabel(rhs.initial_state_);
        
        int num_final_states; in >> num_final_states;
        for (int i = 0; i < num_final_states; ++i) {
            int state; in >> state;
            rhs.final_states_.insert(Relabel(state));
        }

        int num_transitions; in >> num_transitions;
        for (int _ = 0; _ < num_transitions; ++_) {
            int from, to; char label; in >> from >> label >> to;
            from = Relabel(from); to = Relabel(to);
            rhs.delta_[from][label] = to;
        } 
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
