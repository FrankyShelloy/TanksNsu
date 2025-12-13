#pragma once

#include <map>
#include <memory>
#include <functional>
#include <typeindex>
#include "Factory.hpp"

template <
    class AbstractProduct,
    class ProductCreator = std::function<std::unique_ptr<AbstractProduct>(const AbstractProduct*)>,
    template<typename, class> class FactoryErrorPolicy = DefaultFactoryError
>
class CloneFactory {
public:
  using CreatorType = ProductCreator;
  using TypeKey = std::type_index;

  bool Register(const std::type_info& ti, CreatorType creator) {
    TypeKey key(ti);
    auto res = associations_.emplace(key, std::move(creator));
    return res.second;
  }

  bool Unregister(const std::type_info& ti) {
    TypeKey key(ti);
    return associations_.erase(key) > 0;
  }

  std::unique_ptr<AbstractProduct> CreateObject(const AbstractProduct* model) {
    if (!model) {
      return FactoryErrorPolicy<TypeKey, AbstractProduct>::OnUnknownType(TypeKey(typeid(void)));
    }
    TypeKey key(typeid(*model));
    auto it = associations_.find(key);
    if (it != associations_.end()) {
      return (it->second)(model);
    }
    return FactoryErrorPolicy<TypeKey, AbstractProduct>::OnUnknownType(key);
  }

private:
  std::map<TypeKey, CreatorType> associations_;
};
