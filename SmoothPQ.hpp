#ifndef SMOOTHPQ_H
#define SMOOTHPQ_H

#include "Eecs477PQ.hpp"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing priority_queue.
template<typename TYPE, typename COMP_FUNCTOR = std::greater<TYPE>>
class SmoothPQ : public Eecs477PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs477PQ<TYPE, COMP_FUNCTOR>;
    
public:
    // Each node within the pairing heap
    class Node {
    public:
        explicit Node(const TYPE &val)
        : elt{ val }, lmchild{ nullptr }, rmchild{nullptr}, next{ nullptr }
        {}
        
        // Description: Allows access to the element at that Node's position.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        
        friend SmoothPQ;
        
    private:
        TYPE elt;
        Node *lmchild = nullptr;
        Node *rmchild = nullptr;
        Node *next = nullptr;
        Node *prev = nullptr;
        Node * parent = nullptr;
    }; // Node
    
    
    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit SmoothPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
    BaseClass{ comp } {
        this->compare = comp;
        root = nullptr;
        s = 0;
    } // SmoothPQ()
    
    
    // Description: Destructor
    // Runtime: O(n)
    ~SmoothPQ() {
        std::deque <Node*> pq;
        if (root == nullptr) {
            return;
        }
        Node* temp = root;
        pq.push_back(temp); //add node to pq
        root = nullptr;
        while (!pq.empty()) {
            temp = pq.front();
            if (temp->lmchild != nullptr) pq.push_back(temp->lmchild); //adding child
            if (temp->next != nullptr) pq.push_back(temp->next); //adding sibling
            delete temp;
            pq.pop_front();
        }
    } // ~SmoothPQ()
    
    
    // Description: Add a new element to the priority_queue. This has been provided for you,
    //              in that you should implement push functionality in the
    //              addNode function.
    // Runtime: Amortized O(1)
    virtual void push(const TYPE & val) {
        addNode(val);
    } // push()
    
    
    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Runtime: ?
    virtual void pop() {
        Node *x = root->lmchild;
        if (!x) {
            s--;
            return;
        }
        root->lmchild->parent = nullptr;
        root->rmchild->parent = nullptr;
        delete root;
        s--;
        while (x->next != nullptr)
        {
            bool flag = false;
            if (x->elt < x->next->elt)
            {
                x = x->next;
            }
            else
            {
                while (x->prev != nullptr)
                {
                    if (x->prev->elt > x->next->elt)
                    {
                        x = x->prev;
                        x = meld(x, x->next);
                    }
                    else
                    {
                        x = x->next;
                        x = meld(x->prev, x);
                        flag = true;
                        break;
                    }
                }
                if (flag) continue;
                x = meld(x, x->next);
            }
        }
        while (x->prev != nullptr)
        {
            x = x->prev;
            x = meld(x, x->next);
        }
        root = x;
        return;
    } // pop()
    
    
    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return root->getElt();
    } // top()
    
    
    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return s;
    } // size()
    
    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return s == 0;
    } // empty()
    
    //make it the rightmost sibling of root and link with root
    
    void updateElt(Node* node, const TYPE & new_value) {
        if (node == nullptr) return; //no work required
        //SPECIAL CASE: node is root
        if (node == root) {
            root->elt = new_value;
            return;
        }
        node->elt = new_value; //update node elt
        //CASE 1: Node is the left most child and right most child
        if (node->parent && (node->parent)->lmchild == node && node->parent->rmchild == node) {
            root->next = node;
            node->prev = root;
            
            node->parent->lmchild = nullptr;
            node->parent->rmchild = nullptr;
            node->parent = nullptr;
            root = meld(root, root->next);
        } else if (node->parent && (node->parent)->lmchild == node) {
            node->parent->lmchild = node->next;
            node->next->parent = node->parent;
            node->parent = nullptr;
            node->next->prev = nullptr;
            node->next = nullptr;
            //make node right sibling of root
            root->next = node;
            node->prev = root;
            root = meld(root, root->next);
        } else if (node->parent && node->parent->rmchild == node) {
            node->parent->rmchild = node->prev;
            node->prev->parent = node->parent;
            node->parent = nullptr;
            node->prev->next = nullptr;
            node->prev = nullptr;
            //make node right sibling of root
            root->next = node;
            node->prev = root;
            root = meld(root, root->next);
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = nullptr;
            node->next = nullptr;
            //make node right sibling of root
            root->next = node;
            node->prev = root;
            root = meld(root, root->next);
        }
    } // updateElt()
    
    
    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: Amortized O(1)
    Node* addNode(const TYPE & val)
    {
        Node *n = new Node(val);
        if (root != nullptr)
        {
            root = meld(root, n);
        }
        else
        {
            root = n;
        }
        s++;
        return n;
    } // addNode()
    
    //MELD: return the new root
    Node* meld(Node* x, Node* y)
    {
        if (!y) return x;
        if (!x) return y;
        if (this->compare(x->getElt(), y->getElt())) //if y < x (y is the new root)
        {
            if (!x->prev)
            {
                y->prev = nullptr;
            }
            else
            {
                x->prev->next = y;
                y->prev = x->prev;
                x->prev = nullptr;
            }
            if (y->lmchild == nullptr) {
                y->lmchild = x;
                y->rmchild = x;
                x->parent = y;
                //y->prev = nullptr;
                x->next = nullptr;
            } else {
                Node * temp = y->lmchild;
                y->lmchild = x;
                x->parent = y;
                if (temp != y->rmchild) temp->parent = nullptr;
                y->lmchild->next = temp;
                temp->prev = y->lmchild;
                //y->prev = nullptr;
            }
            return y;
        }
        else
        { //x is the root
            if (!y->next)
            {
                x->next = nullptr;
            }
            else
            {
                y->next->prev = x;
                x->next = y->next;
                y->next = nullptr;
            }
            if (x->lmchild == nullptr) {
                x->rmchild = y;
                x->lmchild = y;
                y->parent = x;
                y->prev = nullptr;
                //x->next = nullptr;
            }
            else
            {
                Node * temp = x->rmchild;
                x->rmchild = y;
                y->parent = x;
                if (temp != x->lmchild) temp->parent = nullptr;
                x->rmchild->prev = temp;
                temp->next = x->rmchild;
                //x->next = nullptr;
            }
            return x;
        }
    }
    
private:
    Node* root = nullptr;
    size_t s = 0; //size
};
#endif // SMOOTHPQ_H
