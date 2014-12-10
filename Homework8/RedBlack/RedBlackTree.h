#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H


#include <iostream>        // For NULL
using namespace std;

// Red-black tree class
//
// CONSTRUCTION: with negative infinity object also
//               used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class RedBlackTree
{
  public:
    /**
     * Construct the tree.
     * negInf is a value less than or equal to all others.
     */
    explicit RedBlackTree( const Comparable & negInf )
    {
        fatherGet = false;
        nullNode    = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;
        header      = new RedBlackNode( negInf );
        header->left = header->right = nullNode;
    }

    RedBlackTree( const RedBlackTree & rhs )
    {
        fatherGet = false;
        nullNode    = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;
        header      = new RedBlackNode( rhs.header->element );
        header->left = header->right = nullNode;
        *this = rhs;
    }

    ~RedBlackTree( )
    {
        delete nullNode;
        delete header;
    }

    const Comparable & findMin( ) const
    {
      if( isEmpty( ) ) {}
	  

        RedBlackNode *itr = header->right;

        while( itr->left != nullNode )
            itr = itr->left;

        return itr->element;
    }

    const Comparable & findMax( ) const
    {
      if( isEmpty( ) ){}

        RedBlackNode *itr = header->right;

        while( itr->right != nullNode )
            itr = itr->right;

        return itr->element;
    }

    bool contains( const Comparable & x ) const
    {
        nullNode->element = x;
        RedBlackNode *curr = header->right;

        for( ; curr != nullNode; )
        {
            if( x < curr->element )
                curr = curr->left;
            else if( curr->element < x )
                curr = curr->right;
            else
                return curr != nullNode;
        }
        return false;
    }

    bool isEmpty( ) const
    {
        return header->right == nullNode;
    }

    void printTree( ) const
    {
        if( header->right == nullNode )
            cout << "Empty tree" << endl;
        else
            printTree( header->right );
    }

    void makeEmpty( )
    {
        reclaimMemory( header->right );
        header->right = nullNode;
    }

    /**
     * Insert item x into the tree. Does nothing if x already prsent.
     */
    void insert( const Comparable & x )
    {
        current = parent = grand = header;
        nullNode->element = x;

        while( current->element != x )
        {
            great = grand; grand = parent; parent = current;
            current = x < current->element ?  current->left : current->right;

                // Check if two red children; fix if so
            if( current->left->color == RED && current->right->color == RED )
                handleReorient( x );
        }

            // Insertion fails if already present
        if( current != nullNode )
            return;
        current = new RedBlackNode( x, nullNode, nullNode );

            // Attach to parent
        if( x < parent->element )
            parent->left = current;
        else
            parent->right = current;
        handleReorient( x );
    }

    void remove( const Comparable & num )
    {
        if (!fatherGet) {
            setParent(header);
            fatherGet = true;
        }
        RedBlackNode* z = find(num);
        RedBlackNode* y = z;
        RedBlackNode* x = nullNode;
        int yOriginalColor = y->color;
        if (z->left == nullNode) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nullNode) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = treeMin(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->father == z)
                x->father = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->father = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->father = y;
            y->color = z->color;
        }
        if (yOriginalColor == BLACK)
            removeFixUp(x);
        //delete x;
    }

    enum { RED, BLACK };

    const RedBlackTree & operator=( const RedBlackTree & rhs )
    {
        if( this != &rhs )
        {
            makeEmpty( );
            header->right = clone( rhs.header->right );
        }

        return *this;
    }

  private:
    bool fatherGet;
    struct RedBlackNode
    {
        Comparable    element;
        RedBlackNode *left;
        RedBlackNode *right;
        RedBlackNode *father;
        int           color;

        RedBlackNode( const Comparable & theElement = Comparable( ),
                            RedBlackNode *lt = NULL, RedBlackNode *rt = NULL, RedBlackNode *ft = NULL,
                            int c = BLACK )
          : element( theElement ), left( lt ), right( rt ), color( c ), father( NULL ) { }
    };

    RedBlackNode *header;   // The tree header (contains negInf)
    RedBlackNode *nullNode;

        // Used in insert routine and its helpers (logically static)
    RedBlackNode *current;
    RedBlackNode *parent;
    RedBlackNode *grand;
    RedBlackNode *great;

        // Usual recursive stuff
    void reclaimMemory( RedBlackNode *t )
    {
        if( t != t->left )
        {
            reclaimMemory( t->left );
            reclaimMemory( t->right );
            delete t;
        }
    }

    void printTree( RedBlackNode *t ) const
    {
        if( t != t->left )
        {
            cout << t->element << t->color << " ";
            printTree( t->left );
            printTree( t->right );
        }
    }

    RedBlackNode * clone( RedBlackNode * t ) const
    {
        if( t == t->left )  // Cannot test against nullNode!!!
            return nullNode;
        else
            return new RedBlackNode( t->element, clone( t->left ),
                                     clone( t->right ), NULL, t->color );
    }

        // Red-black tree manipulations
    /**
     * Internal routine that is called during an insertion if a node has two red
     * children. Performs flip and rotatons. item is the item being inserted.
     */
    void handleReorient( const Comparable & item )
    {
            // Do the color flip
        current->color = RED;
        current->left->color = BLACK;
        current->right->color = BLACK;

        if( parent->color == RED )   // Have to rotate
        {
            grand->color = RED;
            if( item < grand->element != item < parent->element )
                parent = rotate( item, grand );  // Start dbl rotate
            current = rotate( item, great );
            current->color = BLACK;
        }
        header->right->color = BLACK; // Make root black
    }

    /**
     * Internal routine that performs a single or double rotation.
     * Because the result is attached to the parent, there are four cases.
     * Called by handleReorient.
     * item is the item in handleReorient.
     * theParent is the parent of the root of the rotated subtree.
     * Return the root of the rotated subtree.
     */
    RedBlackNode * rotate( const Comparable & item, RedBlackNode *theParent )
    {
        if( item < theParent->element )
        {
            item < theParent->left->element ?
                rotateWithLeftChild( theParent->left )  :  // LL
                rotateWithRightChild( theParent->left ) ;  // LR
            return theParent->left;
        }
        else
        {
            item < theParent->right->element ?
                rotateWithLeftChild( theParent->right ) :  // RL
                rotateWithRightChild( theParent->right );  // RR
            return theParent->right;
        }
    }

    void rotateWithLeftChild( RedBlackNode * & k2 )
    {
        RedBlackNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    void rotateWithRightChild( RedBlackNode * & k1 )
    {
        RedBlackNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2;
    }

    // RedBlackNode* getParent(RedBlackNode* child) {
    //     if (child == nullNode)
    //         exit(0);
    //     RedBlackNode* index = header;
    //     while (index != nullNode) {
    //         if (index->left == child || index->right == child) 
    //             break;
    //         else if (child->element < index->element)
    //             index = index->left;
    //         else
    //             index = index->right;
    //     }
    //     return index;
    // }

    void transplant(RedBlackNode* u, RedBlackNode* v) {
        if (u == header->right)
            header->right = v;
        else if (u == u->father->left)
            u->father->left = v;
        else
            u->father->right = v;
        v->father = u->father;
    }

    RedBlackNode* find(const Comparable& x) {
        RedBlackNode* index = header;
        while (index != nullNode && index->element != x) {
            if (x < index->element)
                index = index->left;
            else if (x > index->element)
                index = index->right;
            else
                break;
        }
        return index;
    }

    RedBlackNode* treeMin(RedBlackNode* x) {
        while (x->left != nullNode)
            x = x->left;
        return x;
    }

    void leftRotate(RedBlackNode* x) {
        RedBlackNode* y = x->right;
        x->right = y->left;
        y->left->father = x;
        y->father = x->father;
        if (x == x->father->left)
            x->father->left = y;
        else
            x->father->right = y;
        y->left = x;
        x->father = y;
    }

    void rightRotate(RedBlackNode* y) {
        RedBlackNode* x = y->left;
        y->left = x->right;
        x->right->father = y;
        x->father = y->father;
        if (y == y->father->left)
            y->father->left = x;
        else
            y->father->right = x;
        x->right = y;
        y->father = x;
    }

    void removeFixUp(RedBlackNode* x) { 
        while (x != header->right && x->color == BLACK) {
            if (x == x->father->left) {
                RedBlackNode* w = x->father->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->father->color = RED;
                    leftRotate(x->father);
                    w = x->father->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->father;
                }
                else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->father->right;
                    }
                    w->color = x->father->color;
                    x->father->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->father);
                    x = header->right;
                }
            }
            else {
                RedBlackNode* w = x->father->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->father->color = RED;
                    rightRotate(x->father);
                    w = x->father->left;
                }
                if (w->left->color == BLACK && w->right->color ==BLACK) {
                    w->color = RED;
                    x = x->father;
                }
                else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->father->left;
                    }
                    w->color = x->father->color;
                    x->father->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->father);
                    x = header->right;
                }
            }
        }
        x->color = BLACK; 
    }

    void setParent(RedBlackNode* x) {
        if (x == nullNode)
            return;
        x->left->father = x;
        x->right->father = x;
        setParent(x->left);
        setParent(x->right);
    }
};

#endif
