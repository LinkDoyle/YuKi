#pragma once
#include <algorithm>
#include <forward_list>
#include <functional>

namespace yuki {

template <typename T>
class Property;

class PropertyBase {
 public:
  PropertyBase() = default;
  virtual ~PropertyBase() {
    for (const auto& observer : observers_) {
      auto& dependencies = observer->dependencies_;
      dependencies.remove(this);
    }
  }

  void notify() { notify(this); }
  virtual void evaluate(PropertyBase* source) = 0;
  void unbind() {
    for (auto dependency : dependencies_) {
      auto& observers = dependency->observers_;
      observers.remove(this);
    }
  }

 protected:
  void notify(PropertyBase* source) {
    for (auto observer : observers_) {
      observer->evaluate(source);
    }
  }
  void bind(PropertyBase* property) {
    dependencies_.emplace_front(property);
    property->observers_.emplace_front(this);
  }

 protected:
  std::forward_list<PropertyBase*> observers_;
  std::forward_list<PropertyBase*> dependencies_;
  template <typename T>
  friend class Property;
};

template <typename T>
class Property : public PropertyBase {
 public:
  using Binding = std::function<T()>;
  Property() = default;
  ~Property() = default;
  template <typename U>
  Property(const Property<U>& value) {
    value_ = value.get();
  }
  Property(const T& t) : value_(t) {}
  Property(T&& value) : value_(std::move(value)) {}

  // setters
  void operator=(const T& t) {
    value_ = t;
    notify();
  }
  void operator=(T&& t) {
    value_ = std::move(t);
    notify();
  }

  // getters
  const T& get() const { return value_; }
  const T& operator()() const { return get(); }
  operator const T&() const { return get(); }

  // bind
  template <typename U>
  void bind(Property<U>& value) {
    PropertyBase::bind(&value);
    operator=(value.get());
    binding_ = [&]() { return value.get(); };
  }
  template <typename... Args>
  void bind(Binding binding, Property<Args>&... args) {
    { [[maybe_unused]] int unused[] = {0, ((void)PropertyBase::bind(&args), 0)...}; }
    binding_ = binding;
    evaluate(this);
  }

  // evaluate
  virtual void evaluate(PropertyBase* source) override {
    if (binding_) {
      value_ = binding_();
      if (source != this) {
        notify(source);
      }
    }
  }

 protected:
  T value_;
  Binding binding_;
};

}  // namespace yuki
