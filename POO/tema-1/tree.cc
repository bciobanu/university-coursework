#include <iostream>
#include <cassert>

#define TNode TreeNode<T>

template<class T> class Tree;

// Implementeaza un nod din arbore
// (!) Operatiile afecteaza intreaga structura a subarborelui
template<class T>
class TreeNode {
  public:
    TreeNode(const T idx=T()) : child_(NULL), next_(NULL), idx_(idx) { } 
    virtual ~TreeNode() {
        delete child_; child_ = NULL;
        delete next_; next_ = NULL;
    }

  private:
    TreeNode(const TreeNode& oth) :  // va initializa intregul subarbore pe baza lui oth 
            child_((oth.child_ != NULL) ? new TreeNode(*oth.child_) : NULL),
            next_((oth.next_ != NULL) ? new TreeNode(*oth.next_) : NULL), 
            idx_(oth.idx_) { }
    TreeNode& operator =(const TreeNode& oth);

    bool operator ==(const TreeNode& oth) const { 
        return idx_ == oth.idx_
            and ((child_ == NULL and oth.child_ == NULL) or (*child_ == *oth.child_))
            and ((next_ == NULL and oth.next_ == NULL) or (*next_ == *oth.next_));
    }
    bool operator !=(const TreeNode& oth) const { return !(*this == oth); }

    friend std::istream& operator >>(std::istream& input_stream, TreeNode& t) {
        int degree;

        input_stream >> t.idx_ >> degree;
        assert(degree >= 0);

        TreeNode** last_link = &t.child_;   // de cine vom lega noul subarborele citit 
        for (int i = 0; i < degree; ++i) {  // la fiecare iteratie
            TreeNode* son = new TreeNode(); assert(input_stream >> *son);
            *last_link = son; 
            last_link = &son->next_;
        }
        return input_stream;
    }

    friend std::ostream& operator <<(std::ostream& output_stream, const TreeNode& t) {
        output_stream << t.idx_;
        for (TreeNode* son = t.child_; son != NULL; son = son->next_) {
            output_stream << ' ' << *son;
        }
        return output_stream;
    }

    void PrintLayers(std::ostream& output_stream=std::cout) { Bfs(output_stream,false); }
    void PrintLeaves(std::ostream& output_stream=std::cout) { Bfs(output_stream, true); }
    
    void Bfs(std::ostream& output_stream, bool restrict_to_leaves) {  // efectueaza o parcurge in latime
        TreeNode** bfq = new TreeNode*[FindWeight()];                 // a arborelui cu radacina in this
        bfq[0] = this;                                                // daca vrem sa afisam doar frunzele
        bool need_printed_space = false;                              // setam restrict_to_leaves=True
        int q_start = 0, q_end = 1;
        while (q_start != q_end) {
            TreeNode* node = bfq[q_start++];
            if (not restrict_to_leaves or node->child_ == NULL) {
                if (need_printed_space) {
                    output_stream << ' ';
                } else {
                    need_printed_space = true;
                }

                output_stream << node->idx_;
            }

            for (TreeNode* iter = node->child_; iter != NULL; iter = iter->next_) {
                bfq[q_end++] = iter;
            }
        }
        delete[] bfq;
    }

    TreeNode* DeleteSubtree() {  // sterge subarborele inradacinat in this si returneaza fratele sau drept 
        TreeNode* ans = next_;
        next_ = NULL;
        delete this;
        return ans;
    }

    void EraseNode(const T idx) {
        if (child_ != NULL) {
            child_->EraseNode(idx);
            if (child_->idx_ == idx) {
                child_ = child_->DeleteSubtree();
            }
        }

        if (next_ != NULL) {
            next_->EraseNode(idx);
            if (next_->idx_ == idx) {
                next_ = next_->DeleteSubtree();
            }
        }
    }

    int FindWeight() const {
        int answer = 1;
        for (TreeNode* iter = child_; iter != NULL; iter = iter->next_) {
            answer += iter->FindWeight();
        } 
        return answer;
    }

    int FindHeight() const {
        int answer = 0;
        for (TreeNode* iter = child_; iter != NULL; iter = iter->next_) {
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

// Implementeaza o structura de arbore cu radacina
// --------------------------------------------------------------------------------------------------------------------------
// I/O se fac cu ajutorul std::istream, respectiv std::ostream
// Input:  incepand cu radacina, vom preciza indicele unui nod (care este de tip T) si gradul acestuia (sa zicem ca este k)
//         iar apoi o sa citim recursiv cei k fii ai acestuia 
// Output: se afiseaza exact dupa structura de la Input
// --------------------------------------------------------------------------------------------------------------------------
// PrintLayers(std::ostream) : afiseaza parcurgerea BFS a arborelui 
// PrintLeaves(std::ostream) : afiseaza frunzele arborelui
// weight()                  : returneaza numarul de noduri din arbore
// height()                  : afiseaza inaltimea arborelui
// --------------------------------------------------------------------------------------------------------------------------
// t1 + t2 = t1 in care vom atasa o copie a lui t2, a carei radacina va fi cel mai din dreapta fiu direct al radacinii lui t1
// t1 == t2 daca au fix aceeasi structura si aceeasi indici
// t1  < t2 daca height(t1) < height(t2)
// t1 -= idx va sterge toti subarborii cu radacina in idx
// --------------------------------------------------------------------------------------------------------------------------
template<class T>
class Tree {
  public:
    Tree() : root_(NULL) { }
    Tree(const Tree& oth) : root_(oth.root_ != NULL ? new TNode(*oth.root_) : NULL) { }
    Tree& operator =(const Tree& oth) { 
        if (oth.root_ != root_) {
            delete root_; 
            root_ = (oth.root_ != NULL) ? new TNode(*oth.root_) : NULL;
        }
        return *this;
    }
    virtual ~Tree() { delete root_; root_ = NULL; }
    
    bool operator ==(const Tree& oth) const { 
        if (root_ == NULL) {
            return oth.root_ == NULL;
        }
        return oth.root_ != NULL and *root_ == *oth.root_; 
    }

    bool operator !=(const Tree& oth) const { return !(*this == oth); }
    bool operator  <(const Tree& oth) const { return height() < oth.height(); }

    friend std::istream& operator >>(std::istream& input_stream, Tree& t) { 
        delete t.root_; t.root_ = new TNode(); 
        return input_stream >> *t.root_; 
    }
    friend std::ostream& operator<<(std::ostream& output_stream, const Tree& t) { 
        return t.root_ != NULL ? output_stream<< *t.root_ : output_stream; 
    }

    void PrintLayers(std::ostream& output_stream=std::cout) { 
        if (root_ != NULL) {
            root_->PrintLayers(output_stream); 
        }
    }

    void PrintLeaves(std::ostream& output_stream=std::cout) { 
        if (root_ != NULL) {
            root_->PrintLeaves(output_stream);
        }
    }
    
    Tree& operator -=(const T idx) {
        if (root_ != NULL) {
            if (root_->idx_ == idx) {
                delete root_; root_ = NULL;
            } else {
                root_->EraseNode(idx);
            }
        }
        return *this;
    }
    Tree operator -(const T idx) const { return Tree(*this) -= idx; }

    // pune o copie a lui rhs ca "cel mai din dreapta" (relativ la next_) fiu direct al radacinii
    Tree& operator +=(const Tree& rhs) {
        assert(root_ != NULL); // ne asiguram sa avem radacina
        Tree rhs_copy(rhs);    // in caz ca distrugem rhs, sa nu distrugem si suma (sau invers)
        TNode* iter = root_->child_;
        if (iter == NULL) {    // daca nu avem vreun fiu, il legam direct ca prim fiu al radacinii
            root_->child_ = rhs_copy.root_;
        } else {               // altfel, cautam cel mai din dreapta fiu existent deja
            while (iter->next_ != NULL) { // si il vom lega ca frate direct al acestuia 
                iter = iter->next_;
            }
            iter->next_ = rhs_copy.root_; 
        }
        rhs_copy.root_ = NULL;
        return *this;
    }
    Tree operator +(const Tree& rhs) const { return Tree(*this) += rhs; }

    int weight() const { return root_ != NULL ? root_->FindWeight() : 0; }
    int height() const { return root_ != NULL ? root_->FindHeight() - 1 : 0; }
  private:
    TNode* root_;
};

int main() {
    Tree<int> a, b;
    std::cin >> a >> b;
    std::cout << a << std::endl << a.height() << std::endl;
    a = a;
    a.PrintLeaves();
    a += b;
    
    std::cout << std::endl << (a + b - 1).height() << ' ' << (a + b).weight() << std::endl;
}
