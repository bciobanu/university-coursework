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

    TreeNode(const TreeNode& oth) :  // va initializa intregul subarbore pe baza lui oth 
            child_((oth.child_ != NULL) ? new TreeNode(*oth.child_) : NULL),
            next_((oth.next_ != NULL) ? new TreeNode(*oth.next_) : NULL), 
            idx_(oth.idx_) { }

    TreeNode& operator =(const TreeNode& oth) { // va copia intregul subarbore
        child_ = (oth.child_ != NULL) ? new TreeNode(*oth.child_) : NULL;
        next_  = (oth.next_ != NULL) ? new TreeNode(*oth.next_) : NULL;
        idx_   = oth.idx_;
        return *this;
    }
    
    virtual ~TreeNode() {  // sterge intregul subarbore
        if (child_ != NULL) {
            delete child_;
            child_ = NULL;
        }
        if (next_ != NULL) {
            delete next_;
            next_ = NULL;
        }
    }

    bool operator ==(const TreeNode& oth) const { 
        return idx_ == oth.idx_
            and ((child_ == NULL and oth.child_ == NULL) or (*child_ == *oth.child_))
            and ((next_ == NULL and oth.next_ == NULL) or (*next_ == *oth.next_));
    }
    bool operator !=(const TreeNode& oth) const { return !(*this == oth); }
    
    friend std::istream& operator >>(std::istream& input_stream, TreeNode& t) {
        int degree;

        input_stream >> t.idx() >> degree;
        assert(degree >= 0);

        TreeNode** last_link = &t.child();  // de cine vom lega noul subarborele citit 
        for (int i = 0; i < degree; ++i) {  // la fiecare iteratie
            TreeNode* son = new TreeNode(); input_stream >> *son;
            *last_link = son; 
            last_link = &son->next();
        }
        return input_stream;
    }

    friend std::ostream& operator <<(std::ostream& output_stream, TreeNode& t) {
        output_stream << t.idx();
        for (TreeNode* son = t.child(); son != NULL; son = son->next()) {
            output_stream << ' ' << *son;
        }
        return output_stream;
    }
    
    void PrintLayers(std::ostream& output_stream=std::cout) { Bfs(output_stream,false); }
    void PrintLeaves(std::ostream& output_stream=std::cout) { Bfs(output_stream, true); }

    TreeNode*& child() { return child_; }
    TreeNode*& next()  { return next_; }
    T& idx()           { return idx_; }

  private:
    void Bfs(std::ostream& output_stream, bool restrict_to_leaves) {  // efectueaza o parcurge in latime
        TreeNode** bfq = new TreeNode*[FindWeight()];                 // a arborelui cu radacina in this
        bfq[0] = this;                                                // daca vrem sa afisam doar frunzele
        bool need_printed_space = false;                              // setam restrict_to_leaves=True
        int q_start = 0, q_end = 1;
        while (q_start != q_end) {
            TreeNode* node = bfq[q_start++];
            if (not restrict_to_leaves or node->child() == NULL) {
                if (need_printed_space) {
                    output_stream << ' ';
                } else {
                    need_printed_space = true;
                }

                output_stream << node->idx();
            }

            for (TreeNode* iter = node->child(); iter != NULL; iter = iter->next()) {
                bfq[q_end++] = iter;
            }
        }
        delete[] bfq;
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
// --------------------------------------------------------------------------------------------------------------------------
template<class T>
class Tree {
  public:
    Tree() : root_(new TNode()) { }
    Tree(const Tree& oth) : root_(new TNode(*oth.root_)) { }
    Tree& operator =(const Tree& oth) { return root_ = new TNode(*oth.root_), *this; }
    virtual ~Tree() { delete root_; }
    
    bool operator ==(const Tree& oth) const { return *root_ == *oth.root_; }
    bool operator !=(const Tree& oth) const { return *root_ != *oth.root_; }
    bool operator  <(const Tree& oth) const { return height() < oth.height(); }

    friend std::istream& operator >>(std::istream& input_stream, const Tree& t) { return input_stream >> *t.root_; }
    friend std::ostream& operator<<(std::ostream& output_stream, const Tree& t) { return output_stream<< *t.root_; }

    void PrintLayers(std::ostream& output_stream=std::cout) { root_->PrintLayers(output_stream); }
    void PrintLeaves(std::ostream& output_stream=std::cout) { root_->PrintLeaves(output_stream); }

    // pune o copie a lui rhs ca "cel mai din dreapta" (relativ la next_) fiu direct al radacinii
    Tree& operator +=(const Tree& rhs) {
        Tree rhs_copy(rhs);    // in caz ca distrugem rhs, sa nu distrugem si suma (sau invers)
        TNode* iter = root_->child();
        if (iter == NULL) {    // daca nu avem vreun fiu, il legam direct ca prim fiu al radacinii
            root_->child() = rhs_copy.root_;
        } else {               // altfel, cautam cel mai din dreapta fiu existent deja
            while (iter->next() != NULL) { // si il vom lega ca frate direct al acestuia 
                iter = iter->next();
            }
            iter->next() = rhs_copy.root_; 
        }
        rhs_copy.root_ = NULL;
        return *this;
    }
    Tree operator +(const Tree& rhs) const { return Tree(*this) += rhs; }

    int weight() const { return root_->FindWeight(); }
    int height() const { return root_->FindHeight() - 1; }
  private:
    TNode* root_;
};

int main() {
    Tree<int> a, b;
    std::cin >> a >> b;
    std::cout << a << std::endl << a.height() << std::endl;
    a.PrintLeaves();
    std::cout << std::endl << (a + b).height() << ' ' << (a + b).weight() << std::endl;
}