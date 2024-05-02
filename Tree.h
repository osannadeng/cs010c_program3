#ifndef __TREE_H
#define __TREE_H

#include "Node.h"

class Tree {
private:
  Node *root;

public:
  Tree() : root(nullptr) {}
  ~Tree() { delete root; }

  // rule of three
  Tree(const Tree&) = delete;
  Tree& operator=(const Tree&) = delete;

  void insert(const string &);
  void preOrder() const;
  void inOrder() const;
  void postOrder() const;
  void remove(const string &);
  bool search (const string &) const;

private:
  // Add additional functions/variables here
  void insert(const string&, Node*);
  void preOrder(Node* tree) const;
  void inOrder(Node* tree) const;
  void postOrder(Node* tree) const;
  void remove(const string&, Node*);
  bool search(const string&, const Node*) const;
};

#endif