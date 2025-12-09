#pragma once

#include <functional>
#include <map>

template <
    class AbstractProduct,
    class IdentifierType,
    class ProductCreator = std::function<AbstractProduct*()>
>
class Factory {
public:
  using CreatorType = ProductCreator;

  bool Register(const IdentifierType& id, CreatorType creator) {
    auto res = associations_.emplace(id, std::move(creator));
    return res.second;
  }

  bool Unregister(const IdentifierType& id) {
    return associations_.erase(id) > 0;
  }

  AbstractProduct* CreateObject(const IdentifierType& id) {
    auto it = associations_.find(id);
    if (it != associations_.end()) {
      return (it->second)();
    }
    return nullptr;
  }

private:
  std::map<IdentifierType, CreatorType> associations_;
};
