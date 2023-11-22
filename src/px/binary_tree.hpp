//
// Created by pyxiion on 22.10.23.
//

#ifndef PX_BINARY_TREE_HPP
#define PX_BINARY_TREE_HPP
#include <string>
#include <memory>
#include <variant>
#include <unordered_map>
#include "memory_stream.hpp"
#include "raw_memory.hpp"
#include "serializable.hpp"
#include "exception.hpp"

namespace px {

  class BinaryTree {
  private:
    static inline constexpr std::string_view kFileHeader = "PXBT"; // PyXiion's Binary Tree

    enum class NodeType : std::uint8_t {
      MAP,
      BOOLEAN,
      BYTE, SHORT, INT, LONG,
      FLOAT, DOUBLE,
      STRING,

      ARRAY
    };

    class IncorrectNodeTypeError : public px::Exception {
    public:
      [[nodiscard]] inline const char *what() const noexcept override {
        return "Incorrect type";
      }
    };

    class Node {
    public:
      typedef std::unique_ptr<Node> Ptr;

    private:
      struct None{};
      using Boolean = bool;
      using Byte    = std::int8_t;
      using Short   = std::int16_t;
      using Int     = std::int32_t;
      using Long    = std::int64_t;
      using Float   = float;
      using Double  = double;

      using String  = std::string;

      using Array   = std::vector<Ptr>;
      using Map     = std::unordered_map<String, Ptr>;

      using Value   = std::variant<Boolean, Byte, Short, Int,
                                   Long, Float, Double, String,
                                   Array, Map, None           >;

    public:
      template<class T = None>
      inline explicit Node(T value = T())
        : m_value(value) {}

      template<class T>
      inline T as() const {
        if (holds_alternative<T>(m_value)) {
          return get<T>(m_value);
        }
        throw px::BinaryTree::IncorrectNodeTypeError();
      }

      explicit operator bool() const;
      Node &operator[](const std::string &key);
      const Node &operator[](const std::string &key) const;

    private:
      std::string m_name;

      Node *m_parent{};
      Value m_value;

      static Node kNoneNode;
    };

  public:
    BinaryTree();
    explicit BinaryTree(const std::string &filename);

    void loadFromFile(const std::string &filename);
    void loadFromMemory(const RawConstMemory<char> &memory);

    void saveToFile(const std::string &filename) const;
    const void *data() const;

  private:

  };

} // px

#endif //PX_BINARY_TREE_HPP
