//
// Created by pyxiion on 22.10.23.
//

#ifndef PX_SERIALIZABLE_HPP
#define PX_SERIALIZABLE_HPP

namespace px {
  class MemoryStream;

  class Serializable {
  public:
    virtual void serialize(MemoryStream &memoryStream) const = 0;
    virtual void deserialize(MemoryStream &memoryStream) = 0;
  };

} // px

#endif //PX_SERIALIZABLE_HPP
