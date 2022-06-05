#ifndef HOOVY_HFF_DETAIL__RAII_RAII_FOR_ARGS_HPP
#define HOOVY_HFF_DETAIL__RAII_RAII_FOR_ARGS_HPP


#include <stdexcept>
#include <utility>


namespace hff::detail_::raii
{

struct empty_tag
{
};


inline constexpr empty_tag empty;


template<typename T, typename... Args>
struct raii_for_args
{
  template<int (*Create)(T **, Args...)>
  static inline T *
  to_p_ret_create(Args ...args)
  {
    T *pp = nullptr;

    int res = Create(&pp, std::forward<Args>(args)...);
    if (res < 0)
      return nullptr;

    return pp;
  }

  template<void (*Free)(T *)>
  static inline void
  to_p_free(T **ptr)
  {
    Free(*ptr);
    *ptr = nullptr;
  }

  template<int (*Free)(T **)>
  static inline void
  to_p_free_ignore_ret(T **ptr)
  {
    Free(ptr);
  }

  // I'm unsure if using std::forward is necessary.
  // In this context I think std::forward does nothing since i use
  // `Args` and not `Args&&`.
  // On the other hand std::move'ing everything should work and I can't think
  // of any drawback.
  // That said, since `Create` functions always reference a C API function,
  // they will always have simple types as parameters (no refs or benefits from moving)
  // so I will opt to just not use std::move.
  template<T *(*Create)(Args...), void (*Free)(T **)>
  class type
  {
    T *object_ = nullptr;

    inline void
    free_current();

  public:
    type(empty_tag);
    type(Args ...args);
    ~type();

    type(type const &rhs) = delete;
    type &
    operator=(type const &rhs) = delete;

    type(type &&rhs);
    type &
    operator=(type &&rhs);

    void
    reset(Args...);

    T &
    get();
    T const &
    get() const;
  };

  template<T *(*Create)(Args...), void (*Free)(T *)>
  using type_af = type<Create, to_p_free<Free>>;

  template<int (*Create)(T **, Args...), void (*Free)(T *)>
  using type_ac_af = type<to_p_ret_create<Create>, to_p_free<Free>>;
};


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
raii_for_args<T, Args...>::type<Create, Free>::type(empty_tag)
    : object_{nullptr}
{
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
raii_for_args<T, Args...>::type<Create, Free>::type(Args... args)
    : object_{Create(args...)}
{
  if (object_ == nullptr)
    throw std::runtime_error("Could not create object.");
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
raii_for_args<T, Args...>::type<Create, Free>::~type()
{
  free_current();
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
raii_for_args<T, Args...>::type<Create, Free>::type(type &&rhs)
{
  std::swap(object_, rhs.object_);
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
auto
raii_for_args<T, Args...>::type<Create, Free>::operator=(type &&rhs) -> type &
{
  object_ = rhs.object_;
  rhs.object_ = nullptr;

  return *this;
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
void
raii_for_args<T, Args...>::type<Create, Free>::free_current()
{
  // TODO: Maybe I should not check if all free funcs are NoOp.
  // I could also make a wrapper like to_p_free(), so some are checked and some are not.
  if (object_ != nullptr)
    Free(&object_);
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
void
raii_for_args<T, Args...>::type<Create, Free>::reset(Args ...args)
{
  free_current();
  object_ = Create(args...);
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
T &
raii_for_args<T, Args...>::type<Create, Free>::get()
{
  return *object_;
}


template<typename T, typename... Args>
template<T *(*Create)(Args...), void (*Free)(T **)>
T const &
raii_for_args<T, Args...>::type<Create, Free>::get() const
{
  return *object_;
}

} // namespace hff::detail_::raii

#endif