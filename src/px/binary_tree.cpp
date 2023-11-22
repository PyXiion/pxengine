//
// Created by pyxiion on 22.10.23.
//

#include "binary_tree.hpp"
#include <fstream>

namespace px {
  BinaryTree::BinaryTree() = default;

  BinaryTree::BinaryTree(const std::string &filename) {
    loadFromFile(filename);
  }

  void BinaryTree::loadFromFile(const std::string &filename) {
    std::ifstream ifs(filename);
    char header[kFileHeader.length()];
    ifs >> header;
    if (header != kFileHeader) {
      throw std::runtime_error("Invalid file header.");
    }
  }

  void BinaryTree::loadFromMemory(const RawConstMemory<char> &memory) {

  }

  BinaryTree::Node BinaryTree::Node::kNoneNode{};

  BinaryTree::Node::operator bool() const {
    return not holds_alternative<None>(m_value);
  }

  BinaryTree::Node &BinaryTree::Node::operator[](const std::string &key) {
    if (holds_alternative<Map>(m_value)) {
      auto &map = get<Map>(m_value);
      auto it = map.find(key);
      if (it != map.end())
        return *(it->second);
    }
    return kNoneNode;
  }

  const BinaryTree::Node &BinaryTree::Node::operator[](const std::string &key) const {
    if (holds_alternative<Map>(m_value)) {
      auto &map = get<Map>(m_value);
      auto it = map.find(key);
      if (it != map.end())
        return *(it->second);
    }
    return kNoneNode;
  }
} // px