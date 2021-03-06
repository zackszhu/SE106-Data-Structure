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
        nullNode    = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;
        header      = new RedBlackNode( negInf );
        header->left = header->right = nullNode;
    }

    RedBlackTree( const RedBlackTree & rhs )
    {
        nullNode    = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;
        header      = new RedBlackNode( rhs.header->element );
        header->left = header->right = nullNode;
        *this = rhs;
    }

    ~RedBlackTree( )
    {
        makeEmpty( );
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
        RedBlackNode* z = find(num);
        RedBlackNode* y = z;
        RedBlackNode* x = nullNode;
        int yOriginalColor = y->color;
        if (z->left == nullNode && z->right == nullNode)
            if (z == getParent(z)->left)
                getParent(z)->left = nullNode;
            else
                getParent(z)->right = nullNode;
        else if (z->left == nullNode) {
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
            if (getParent(y) != z) {
                transplant(y, y->right);
                y->right = z->right;
            }
            transplant(z, y);
            y->left = z->left;
            y->color = z->color;
        }
        if (yOriginalColor == BLACK && x != nullNode)
            removeFixUp(x);
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
    struct RedBlackNode
    {
        Comparable    element;
        RedBlackNode *left;
        RedBlackNode *right;
        int           color;

        RedBlackNode( const Comparable & theElement = Comparable( ),
                            RedBlackNode *lt = NULL, RedBlackNode *rt = NULL,
                            int c = BLACK )
          : element( theElement ), left( lt ), right( rt ), color( c ) { }
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
                                     clone( t->right ), t->color );
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

    RedBlackNode* getParent(RedBlackNode* child) {
        if (child == nullNode)
            exit(0);
        RedBlackNode* index = header;
        while (index != nullNode) {
            if (index->left == child || index->right == child) 
                break;
            else if (child->element < index->element)
                index = index->left;
            else
                index = index->right;
        }
        return index;
    }

    void transplant(RedBlackNode* u, RedBlackNode* v) {
        if (getParent(u) == header)
            header->right = v;
        else if (u == getParent(u)->left)
            getParent(u)->left = v;
        else
            getParent(u)->right = v;
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

    void removeFixUp(RedBlackNode* x) { 
        while (x != header->right && x->color == BLACK && x!= nullNode && x!= header) {
            RedBlackNode* xp = getParent(x);
            if (x == xp->left) {
                RedBlackNode* w = xp->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    xp->color = RED;
                    rotateWithRightChild(xp);
                    w = xp->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = xp;
                }
                else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateWithLeftChild(w);
                        w = xp->right;
                    }
                    w->color = xp->color;
                    xp->color = BLACK;
                    w->right->color = BLACK;
                    rotateWithRightChild(xp);
                    x = header->right;
                }
            }
            else {
                RedBlackNode* w = xp->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    xp->color = RED;
                    rotateWithLeftChild(xp);
                    w = xp->left;
                }
                if (w->left->color == BLACK && w->right->color ==BLACK) {
                    w->color = RED;
                    x = xp;
                }
                else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateWithRightChild(w);
                        w = xp->left;
                    }
                    w->color = xp->color;
                    xp->color = BLACK;
                    w->left->color = BLACK;
                    rotateWithLeftChild(xp);
                    x = header->right;
                }
            }
        }
        x->color = BLACK; 
    }
};

#endif
