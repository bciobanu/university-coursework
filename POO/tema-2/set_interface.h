#ifndef _SET_INTERFACE_H
#define _SET_INTERFACE_H

namespace avl {

template<typename Key, typename Comp>
class SetInterface {
 public:
  virtual ~SetInterface() {}

  virtual void Insert(const Key) = 0;
  virtual bool Find(const Key) const = 0;

  virtual void Erase(const Key) = 0;

  virtual bool Empty() const = 0;
  virtual size_t Size() const = 0;

  virtual void Clear() = 0;
};

}

#endif  // _SET_INTERFACE_H