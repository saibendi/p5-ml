#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;


TEST(bst_public_test) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);
  cout << "PASS 1" << endl;

  ASSERT_TRUE(tree.find(5) != tree.end());

  cout << "PASS 2" << endl;

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
    cout << "PASS 3.1" << endl;
  ASSERT_TRUE(*tree.max_element() == 7);
    cout << "PASS 3.2" << endl;
  ASSERT_TRUE(*tree.min_element() == 3);
    cout << "PASS 3.3" << endl;
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);
    cout << "PASS 3.4" << endl;

  cout << "PASS 3" << endl;

  cout << "cout << tree.to_string()" << endl;
  cout << tree.to_string() << endl << endl;

  cout << "cout << tree" << endl << "(uses iterators)" << endl;
  cout << tree << endl << endl;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST_MAIN()
