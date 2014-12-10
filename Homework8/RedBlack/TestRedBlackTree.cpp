#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "RedBlackTree.h"

using namespace std;

    // Test program
int main( )
{
  
    const int NEG_INF = -9999;
    RedBlackTree<int> t( NEG_INF );
    int NUMS = 400000;
    const int GAP  =   37;
    int i,c = 0;

    cout << "Checking inserting... (no more output means success)" << endl;
    
    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS ,c++)
        t.insert( i );

    if( NUMS < 40 )
        t.printTree( );
    if( t.findMin( ) != 1 || t.findMax( ) != NUMS - 1 ){
        cout << "FindMin or FindMax error!" << endl;
	return -1;
    }

    for( i = 1; i < NUMS; i++ ){
      if( !t.contains( i ) ){
	cout << "Find error1! Number not in tree!." << endl;
	return -1;
      }
	
    }
    if( t.contains( 0 ) ){
        cout << "Oops!0 should not be in there." << endl;
	return -1;	
    }


    RedBlackTree<int> t2( NEG_INF );
    t2 = t;
    cout << "Checking removing... (no more output means success)" << endl;
    srand(time(NULL));
    for(int i = 0;i < NUMS / 2;){
      //randomly remove 1000 elements.
      int v = rand() % (NUMS-1) + 1;
      if(!t2.contains(v)) {
	       continue;
      }
      i++;
      // cout << i << endl;
      // cout << "Before: ";
      // t2.printTree();
      // cout << endl;
      // cout << "Remove " << v << endl;
      t2.remove(v);
      // cout << "After: ";
      // t2.printTree();
      // cout << endl;
      // cout << "c" << endl;
      if(t2.contains(v)) {
	cout<<"wrong. number still exesists.\n";
	return -1;
      }
    }

    cout << "Test complete..." << endl;
    return 0;
}
