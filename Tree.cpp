#include <iostream>
#include "Tree.h"

// inserts key into tree while keeping 2-3 tree structure
void Tree::insert(const string &key) {
    if (root == nullptr) root = new Node(key);  // tree starts empty
    if (search(key)) return;    // if it exists
    insert(key, root);
}

// print tree in preorder format
void Tree::preOrder() const {
    preOrder(root); // call helper
    cout << endl;
}

// print tree in order
void Tree::inOrder() const {
    inOrder(root);  // call helper
    cout << endl;
}

// print tree in postorder format
void Tree::postOrder() const {
    postOrder(root);    // call helper
    cout << endl;
}

// remove key from tree while keeping 2-3 tree structure
void Tree::remove(const string &key) {
    if (!search(key, root)) return;
    remove(key, root);
}

// return whether key is in tree
bool Tree::search (const string &key) const {
    return search(key, root);
}

// helper, recursive preorder that prints tree in preorder format
void Tree::preOrder(Node* tree) const {
    // print small, do left, print large, do middle, do right
    if (tree == nullptr) return;
    cout << tree->small << ", ";
    preOrder(tree->left);
    if (tree->large != "") cout << tree->large << ", ";
    preOrder(tree->middle);
    preOrder(tree->right);
}

// helper, recursive inorder that prints tree in order
void Tree::inOrder(Node* tree) const {
    // do left, print smaller node, do middle, print larger node, do right
    if (tree == nullptr) return;
    inOrder(tree->left);
    cout << tree->small << ", ";
    inOrder(tree->middle);
    if (tree->large != "") cout << tree->large << ", ";
    inOrder(tree->right);
}

// helper, recursive postorder that prints tree in postorder format
void Tree::postOrder(Node* tree) const {
    // do left, do middle, print small, do right, print large
    if (tree == nullptr) return;
    postOrder(tree->left);
    postOrder(tree->middle);
    cout << tree->small << ", ";
    postOrder(tree->right);
    if (tree->large != "") cout << tree->large << ", ";
}

// helper, recursive search that returns true only if key is found in tree
bool Tree::search(const string& key, const Node* curr) const {
    if (curr == nullptr) return false;
    if (curr->small == key || curr->large == key) return true;
    if (key < curr->small) return search(key, curr->left);
    if (curr->large != "" && key > curr->large) return search(key, curr->right);
    return search(key, curr->middle);
}

// helper, recursive function that inserts key into tree while keeping 2-3 tree format
// covers cases covered by zybooks and some other cases
// missing some cases -> ideally would implement rest of the cases, or could throw
void Tree::insert(const string& key, Node* curr) {
    string midKey;
    // do nothing if it already exists
    if (key == curr->small || key == curr->large) return;
    if (key < curr->small) {
        // larger key is empty -> curr has space
        if (curr->large == "") {
            // if no children
            if (!curr->left) {
                curr->large = curr->small;
                curr->small = key;
                return;
            }   // there are children if continues
            // no space in left child
            if (curr->left->large != "") {
                // key is less than smaller key of left child
                if (key < curr->left->small) {
                    if (curr->left->left && curr->left->left->large > key) {
                        insert(key, curr->left);
                        return;
                    }
                    midKey = curr->left->small;
                    curr->left->small = key;
                    curr->large = curr->small;
                    curr->small = midKey;
                    curr->right = curr->middle;
                    curr->middle = new Node(curr->left->large);
                    curr->middle->parent = curr;
                    curr->left->large = "";
                } else if (key > curr->left->large) {
                    // key is greater than larger key of left child
                    if (curr->left->left && curr->left->right->small < key) {
                        insert(key, curr->left);
                        return;
                    }
                    midKey = curr->left->large;
                    curr->left->large = key;
                    curr->large = curr->small;
                    curr->small = midKey;
                    curr->right = curr->middle;
                    curr->middle = new Node(curr->left->large);
                    curr->middle->parent = curr;
                    curr->left->large = "";
                } else {
                    // key is in between
                    curr->large = curr->small;
                    curr->small = key;
                    curr->right = curr->middle;
                    curr->middle = new Node(curr->left->large);
                    curr->middle->parent = curr;
                    curr->left->large = "";
                }
                return;
            }
            // there is space in left child
            insert(key, curr->left);
            return;
        }   // curr is full if continues
        if (curr->left == nullptr) {    // no children
            // root
            if (curr == root) {
                curr->left = new Node(key);
                curr->left->parent = curr;
                curr->middle = new Node(curr->large);
                curr->middle->parent = curr;
                curr->large = "";
                return;
            }
            midKey = curr->small;
            curr->small = key;
            insert(midKey, curr->parent);
            return;
        }
        // if continues, will have children
        // space left in child, no grandchildren
        if (curr->left->large == "" && !curr->left->left) {
            if (key < curr->left->small) {
                curr->left->large = curr->left->small;
                curr->left->small = key;
                return;
            }   // otherwise curr is greater than small
            curr->left->large = key;
            return;
        }
        // left child also full
        insert(key, curr->left);
        return;
    } else if (curr->large != "" && key > curr->large) {    // key is greater than larger key, curr is full
        // if no children
        if (!curr->left) {
            curr->left = new Node(curr->small);
            curr->left->parent = curr;
            curr->middle = new Node(key);
            curr->middle->parent = curr;
            curr->small = curr->large;
            curr->large = "";
            return;
        }   // there will be children if continues
        // no space in right child
        if (curr->right->large != "") {
            // key is less than smaller key of right child
            if (key < curr->right->small) {
                if (curr->right->left && curr->right->left->large > key) {
                    insert(key, curr->right);
                    return;
                }
                midKey = curr->right->small;
                curr->right->small = key;
                insert(midKey, curr->parent);
            } else if (key > curr->right->large) {
                // key is greater than larger key of right child
                if (curr->right->left && curr->right->right->small < key) {
                    insert(key, curr->right);
                    return;
                }
                midKey = curr->right->large;
                curr->right->large = key;
                insert(midKey, curr->parent);
            } else {    // key is in between
                if (curr->middle->large == "") {    // middle is not full
                    insert(key, curr->middle);
                    return;
                }
                Node* left = new Node(curr->small);
                left->left = curr->left;
                left->middle = new Node(curr->middle->small);
                Node* middle = new Node(curr->large);
                middle->left = new Node(curr->middle->large);
                middle->middle = curr->right;
                curr->right = nullptr;
                curr->left = left;
                curr->right = middle;
            }
        }
        // there is space in right child
        insert(key, curr->right);
        return;
    } else {    // greater than smaller key
        if (curr->large == "") {    // curr has space
            if (!curr->left) {  // no children
                curr->large = key;
                return;
            }
            // if key is smaller than smallest key of middle
            if (key < curr->middle->small) {
                // middle has space
                if (curr->middle->large == "") {
                    insert(key, curr->middle);
                    return;
                } // continues if no space in middle child
                curr->large = curr->middle->small;
                curr->right = new Node(curr->middle->large);
                curr->right->parent = curr;
                curr->middle->large = "";
                if (curr->middle->left) return; // no more children
                curr->right->middle = curr->middle->right;
                curr->middle->right = nullptr;
                curr->right->left = new Node(curr->middle->middle->large);
                curr->right->left->parent = curr->right;
                curr->middle->middle->large = "";
                return;
            }   // key is larger than smallest key of middle if continues
            if (curr->middle->large == "") {
                insert(key, curr->middle);
                return;
            }
            // middle child is full
            // key is between middle strings and has no children
            if (key < curr->middle->large && !curr->middle->left) {
                curr->large = key;
                curr->right = new Node(curr->middle->large);
                curr->right->parent = curr;
                curr->middle->large = "";
                return;
            }
            // key is larger than largest key of middle and has no children
            if (!curr->middle->left) {
                curr->large = curr->middle->large;
                curr->middle->large = "";
                curr->right = new Node(key);
                curr->right->parent = curr;
                return;
            }
            curr->large = curr->middle->small;
            curr->right = new Node(curr->middle->large);
            curr->right->parent = curr;
            curr->middle->small = key;
            if (curr->middle->left) return;
            curr->right->right = curr->middle->right;
            curr->right->left = new Node(curr->middle->middle->large);
            curr->right->left = curr->right;
            curr->middle->middle->large = "";
            return;
        }  // between keys if continues
        if (curr == root) {
            // no children
            if (!curr->left) {
                curr->left = new Node(curr->small);
                curr->left->parent = curr;
                curr->middle = new Node(curr->large);
                curr->middle->parent = curr;
                curr->small = key;
                curr->large = "";
                return;
            }
        }
        if (curr->middle) insert(key, curr->middle);
        else {  // no children
            curr->left = new Node(curr->small);
            curr->left->parent = curr;
            curr->middle = new Node(curr->large);
            curr->middle->parent = curr;
        }
    }
}

// helper, recursive function that removes a key from tree while keeping 2-3 tree format
// covers cases covered by zybooks and some others
// missing some cases -> ideally would implement rest of the cases, or could throw
void Tree::remove(const string& key, Node* curr) {
    if (curr == nullptr) return;    // doesn't exist in tree
    // recursive call to left subtree if less than smaller value
    if (key < curr->small) {
        remove(key, curr->left);
        return;
    }
    // recursive call to right subtree if greater than larger value
    // only runs if curr node is full (large also exists)
    if (curr->large != "" && key > curr->large) {
        remove(key, curr->right);
        return;
    }
    // curr node doesn't have given key -> must be in the middle
    if (curr->small != key && curr->large != key) {
        remove(key, curr->middle);
        return;
    }
    // continues if key is in current node
    // if key is the smaller/only value
    if (curr->small == key) {
        // root
        if (curr == root) {
            // only value in root
            if (!root->left) {
                delete root;
                root = nullptr;
                return;
            }
            if (root->large == "") {
                // only one value in each child
                if (root->left->large == "" && root->middle->large == "") {
                    root->small = root->left->small;
                    root->large = root->middle->small;
                    // no more children
                    if (!root->left->left) {
                        delete root->left;
                        root->left = nullptr;
                        delete root->middle;
                        root->middle = nullptr;
                        return;
                    }
                    root->right = root->middle->right;
                    root->middle = root->middle->left;
                    root->middle->large = root->middle->small;
                    root->middle->small = root->left->large;
                    root->left = root->left->left;
                    return;
                }
            }
        }
        // no children
        if (!curr->left) {
            // there is another value
            if (curr->large != "") {
                curr->small = curr->large;
                curr->large = "";
                return;
            }   // if continuing, only one value in curr
            // parent has two values
            if (curr->parent->large != "") {
                // curr is left child
                if (curr == curr->parent->left) {
                    curr->small = curr->parent->small;
                    // parent's middle has two values
                    if (curr->parent->middle->large != "") {
                        curr->parent->small = curr->parent->middle->small;
                        curr->parent->middle->small = curr->parent->middle->large;
                        curr->parent->middle->large = "";
                        return;
                    }
                    // parent's middle has one value
                    curr->parent->small = curr->parent->large;
                    curr->parent->large = "";
                    curr->large = curr->parent->middle->small;
                    curr->parent->middle = curr->parent->right;
                    curr->parent->right = nullptr;
                    return;
                }
                // curr is right child
                if (curr == curr->parent->right) {
                    // parent's middle has two values
                    if (curr->parent->middle->large != "") {
                        curr->parent->large = curr->parent->middle->large;
                        curr->parent->middle->large = "";
                        return;
                    }
                    // parent's middle has one value
                    curr->parent->middle->large = curr->parent->large;
                    curr->parent->large = "";
                    curr->parent->large = nullptr;
                    delete curr;
                    return;
                }
                // curr is middle child
                // parent's left only has one value
                if (curr->parent->left->large == "") {
                    curr->parent->left->large = curr->parent->small;
                    curr->parent->small = curr->parent->large;
                    curr->parent->large = "";
                    curr->parent->middle = curr->parent->right;
                    curr->parent->right = nullptr;
                    delete curr;
                    return;
                }
                // parent's right only has one value
                if (curr->parent->right->large == "") {
                    curr->small = curr->parent->large;
                    curr->parent->large = "";
                    curr->large = curr->parent->right->small;
                    delete curr->parent->right;
                    curr->parent->right = nullptr;
                    return;
                }
                // both right and left are full
                curr->small = curr->parent->small;
                curr->parent->small = curr->parent->left->large;
                curr->parent->left->large = "";
                return;
            }
            // parent has one value
            // if curr is left child
            if (curr == curr->parent->left) {
                // if parent's right only has one value
                if (curr->parent->middle->large == "") {
                    if (curr->parent == root) {
                        curr->parent->large = curr->parent->middle->small;
                        delete root->left;
                        root->left = nullptr;
                        delete root->middle;
                        root->middle = nullptr;
                        return;
                    }
                } else {
                    curr->small = curr->parent->small;
                    curr->parent->small = curr->parent->middle->small;
                    curr->parent->middle->small = curr->parent->middle->large;
                    return;
                }
            }   // if continuing, curr is middle child
            // if parent's left has only one value
            if (curr->parent->left->large == "") {
                if (curr->parent == root) {
                    root->large = root->small;
                    root->small = root->left->small;
                    delete root->left;
                    root->left = nullptr;
                    delete root->middle;
                    root->middle = nullptr;
                    return;
                }
            }
            // parent's left has two values
            curr->small = curr->parent->small;
            curr->parent->small = curr->parent->left->large;
            curr->parent->left->large = "";
            return;
        }
        // there are children
        // curr has two values and both children of left have only one node
        if (curr->large != "" && curr->left->large == "" && curr->middle->large == "") {
            curr->left->large = curr->middle->small;
            curr->middle = curr->right;
            curr->right = nullptr;
            curr->small = curr->large;
            curr->large = "";
            return;
        }
        // curr has two values and at least one child has two nodes
        if (curr->large != "") {
            // left has two values
            if (curr->left->large != "") {
                curr->small = curr->left->large;
                curr->left->large = "";
                return;
            }
            // right has two values
            if (curr->middle->large != "") {
                curr->small = curr->middle->small;
                curr->middle->small = curr->middle->large;
                curr->middle->large = "";
                return;
            }
        }
        // curr only has one value
        if (curr->large == "") {
            // left has two values
            if (curr->left->large != "") {
                curr->small = curr->left->large;
                curr->left->large = "";
                return;
            }
            // middle has two values
            if (curr->middle->large != "") {
                curr->small = curr->middle->small;
                curr->middle->small = curr->middle->large;
                curr->middle->large = "";
                return;
            }
        }
    }
    // if key is larger value (curr will be full)
    else if (curr->large == key) {
        // no children
        if (!curr->left) {
            curr->large = "";
            return;
        }
        // there is one value in each child of right
        if (curr->middle->large == "" && curr->right->large == "") {
            curr->large = "";
            curr->middle->large = curr->right->small;
            return;
        }   // if continuing, two values in at least one child
        // right has two
        if (curr->middle->large == "") {
            curr->large = curr->right->small;
            curr->right->small = curr->right->large;
            curr->right->large = "";
            return;
        }
        // middle has two
        curr->large = curr->middle->large;
        curr->middle->large = "";
        return;
    }
}