#include <iostream>
#include <cassert>

#define TNode TreeNode<T>

template<class T> class Tree;

template<class T>
class TreeNode {
  public:
    TreeNode(const T idx=T()) : child_(nullptr), next_(nullptr), idx_(idx) { } 
    TreeNode(const TreeNode& oth) : child_((oth.child_ != nullptr) ? new TreeNode(*oth.child_) : nullptr),
            next_((oth.next_ != nullptr) ? new TreeNode(*oth.next_) : nullptr), idx_(oth.idx_) { }
    TreeNode& operator =(const TreeNode& oth) {
        child_ = (oth.child_ != nullptr) ? new TreeNode(*oth.child_) : nullptr;
        next_  = (oth.next_ != nullptr) ? new TreeNode(*oth.next_) : nullptr;
        idx_   = oth.idx_;
        return *this;
    }

    virtual ~TreeNode() {
        if (child_ != nullptr) {
            delete child_;
            child_ = nullptr;
        }
        if (next_ != nullptr) {
            delete next_;
            next_ = nullptr;
        }
    }

    bool operator ==(const TreeNode& oth) const {
        return idx_ == oth.idx_
            && child_ == oth.child_ && next_ == oth.next_;
    } 
    bool operator !=(const TreeNode& oth) const { return !(*this == oth); }

    friend std::istream& operator >>(std::istream& input_stream, TreeNode& t) {
        int degree;

        input_stream >> t.idx() >> degree;
        assert(degree >= 0);

        TreeNode** sons = new TreeNode*[degree];
        for (int i = 0; i < degree; ++i) {
            sons[i] = new TreeNode(); input_stream >> *sons[i];
        }
        for (int i = degree - 1; i >= 0; --i) {
            sons[i]->next() = t.child();
            t.child() = sons[i];
        }
        delete[] sons;
        return input_stream;
    }

    friend std::ostream& operator <<(std::ostream& output_stream, TreeNode& t) {
        output_stream << t.idx();
        for (TreeNode* son = t.child(); son != nullptr; son = son->next()) {
            output_stream << ' ' << *son;
        }
        return output_stream;
    }
    
    friend void PrintLayers(std::ostream& output_stream, TreeNode& t) {
        Bfs(output_stream, t);
    }

    friend void PrintLeaves(std::ostream& output_stream, TreeNode& t) {
        Bfs(output_stream, t, true);
    }

    TreeNode*& child() { return child_; }
    TreeNode*& next() { return next_; }
    T& idx() { return idx_; }
  private:
    friend void Bfs(std::ostream& output_stream, TreeNode& t, bool restrict_to_leaves=false) {
        const int n = t.FindWeight();
        TreeNode** bfq = new TreeNode*[n];
        bfq[0] = &t;

        int q_start = 0, q_end = 1;
        int num_printed = 0;
        while (q_start != q_end) {
            TreeNode* node = bfq[q_start++];
            if (not restrict_to_leaves or node->child() == nullptr) {
                if (num_printed != 0) {
                    output_stream << ' ';
                }
                output_stream << node->idx();
                ++num_printed;
            }
            for (TreeNode* iter = node->child(); iter != nullptr; iter = iter->next()) {
                bfq[q_end++] = iter;
            }
        }
        output_stream << std::endl;
        delete[] bfq;
    }
    
    int FindWeight() const {
        int answer = 1;
        for (TreeNode* iter = child_; iter != nullptr; iter = iter->next_) {
            answer += iter->FindWeight();
        } 
        return answer;
    }

    int FindHeight() const {
        int answer = 0;
        for (TreeNode* iter = child_; iter != nullptr; iter = iter->next_) {
            const int son_height = iter->FindHeight();
            if (answer < son_height) {
                answer = son_height;
            }
        }
        return 1 + answer;
    }

    friend class Tree<T>;
    TreeNode* child_;
    TreeNode* next_;
    T idx_;
};

template<class T>
class Tree {
  public:
    Tree() : root_(new TNode()) { }
    Tree(const Tree& oth) : root_(new TNode(*oth.root_)) { }
    Tree& operator =(const Tree& oth) { return root_ = new TNode(*oth.root_), *this; }

    virtual ~Tree() { delete root_; }
    
    bool operator ==(const Tree& oth) const { return root_ == oth.root_; }
    bool operator !=(const Tree& oth) const { return root_ != oth.root_; }
    bool operator <(const Tree& oth) const { return height() < oth.height(); }

    friend std::istream& operator >>(std::istream& input_stream, Tree& t) {
        input_stream >> *t.root_;
        return input_stream;
    }

    friend std::ostream& operator<<(std::ostream& output_stream, const Tree& t) {
        output_stream << *t.root_;
        return output_stream;
    }

    friend void PrintLayers(std::ostream& output_stream, Tree& t) {
        PrintLayers(output_stream, *t.root_);
    }

    friend void PrintLeaves(std::ostream& output_stream, Tree& t) {
        PrintLeaves(output_stream, *t.root_);
    }

    Tree& operator +=(const Tree& rhs) {
        Tree rhs_copy(rhs);    // in caz ca distrugem rhs, sa nu distrugem si suma (sau invers)
        TNode* brother = root_->child();
        if (brother == nullptr) {
            root_->child() = rhs_copy.root_;
        } else {
            while (brother->next() != nullptr) {
                brother = brother->next();
            }
            brother->next() = rhs_copy.root_;
        }
        rhs_copy.root_ = nullptr;
        return *this;
    }

    Tree operator +(const Tree& rhs) const { return Tree(*this) += rhs; }

    int weight() const {
        return root_->FindWeight();
    }

    int height() const {
        return root_->FindHeight() - 1;
    }
  private:
    TNode* root_;
};

int main() {
    Tree<int> a, b;
    std::cin >> a >> b;
    std::cout << a << ' ' << b << std::endl;
    
    PrintLayers(std::cout, a);
    PrintLeaves(std::cout, a);
    std::cout << a.height() << std::endl;
    std::cout << (a + b) << std::endl;
    if (auto c = a + b; c != a) {
        std::cout << "OK!" << std::endl;
    }
}