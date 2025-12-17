#pragma once

#include <functional>
#include <map>
#include <memory>
#include <exception>

template <class IdentifierType, class ProductType>
class DefaultFactoryError {
public:
  class Exception : public std::exception {
  public:
    Exception(const IdentifierType& unknownId) : unknownId_(unknownId) {}
    virtual const char* what() const noexcept override {
      return "unknown type ident";
    }
    const IdentifierType& GetId() const { return unknownId_; }
  private:
    IdentifierType unknownId_;
  };

protected:
  static std::unique_ptr<ProductType> OnUnknownType(const IdentifierType& id) {
    throw Exception(id);
  }
};

template <
  class AbstractProduct,
  class IdentifierType,
  class ProductCreator = std::function<std::unique_ptr<AbstractProduct>()>,
  template <typename, class> class FactoryErrorPolicy = DefaultFactoryError
>
class Factory : public FactoryErrorPolicy<IdentifierType, AbstractProduct> {
public:
  using CreatorType = ProductCreator;

  bool Register(const IdentifierType& id, CreatorType creator) {
    auto res = associations_.emplace(id, std::move(creator));
    return res.second;
  }

  bool Unregister(const IdentifierType& id) {
    return associations_.erase(id) > 0;
  }

  std::unique_ptr<AbstractProduct> CreateObject(const IdentifierType& id) {
    auto it = associations_.find(id);
    if (it != associations_.end()) {
      return (it->second)();
    }
    return FactoryErrorPolicy<IdentifierType, AbstractProduct>::OnUnknownType(id);
  }

private:
  std::map<IdentifierType, CreatorType> associations_;
};
