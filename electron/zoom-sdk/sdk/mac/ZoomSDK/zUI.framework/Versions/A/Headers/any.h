// See http://www.boost.org/libs/any for Documentation.

#ifndef ZUI_ANY_INCLUDED
#define ZUI_ANY_INCLUDED

#include <algorithm>
#include <typeinfo>
#include <string>

namespace zui
{
    template<bool B, typename T = void> 
    using disable_if = std::enable_if<!B, T>;

    template <typename T>
    struct add_reference_impl
    {
      typedef T& type;
    };

    template <class T> struct add_reference
    {
      typedef typename add_reference_impl<T>::type type;
    };

    class any
    {
    public: // structors

        any()
          : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
          : content(new zholder<
                typename std::remove_cv<typename std::decay<const ValueType>::type>::type
            >(value))
        {
        }

        any(const any & other)
          : content(other.content ? other.content->clone() : 0)
        {
        }

        // Move constructor
        any(any&& other) noexcept
          : content(other.content)
        {
            other.content = 0;
        }

        // Perfect forwarding of ValueType
        template<typename ValueType, 
          typename = typename disable_if<std::is_same<any&, ValueType>::value>::type, // disable if ValueType is any&
          typename = typename disable_if<std::is_const<ValueType>::value>::type> // disable if ValueType is const
        any(ValueType&& value)
          : content(new zholder< typename std::decay<ValueType>::type >(static_cast<ValueType&&>(value)))
        {
        }

        ~any() noexcept
        {
            delete content;
        }

    public: // modifiers

        any & swap(any & rhs) noexcept
        {
            std::swap(content, rhs.content);
            return *this;
        }


        any & operator=(const any& rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        // move assignment
        any & operator=(any&& rhs) noexcept
        {
            rhs.swap(*this);
            any().swap(rhs);
            return *this;
        }

        // Perfect forwarding of ValueType
        template <class ValueType>
        any & operator=(ValueType&& rhs)
        {
            any(static_cast<ValueType&&>(rhs)).swap(*this);
            return *this;
        }

        bool operator==(const any& other) const
        {
          if (empty() && other.empty())
            return true;
          if (empty() || other.empty())
            return false;
          return content->equals(*other.content);
        }

        bool operator!=(const any& other) const
        {
          return !(*this == other);
        }

    public: // queries

        bool empty() const noexcept
        {
            return !content;
        }

        bool has_value() const noexcept
        {
          return content;
        }

        void clear() noexcept
        {
            any().swap(*this);
        }

        const std::type_info & type() const noexcept
        {
            return content ? content->type() : typeid(void);
        }

    private: // types
        class placeholder
        {
        public: // structors
            virtual ~placeholder(){}

        public: // queries

            virtual const std::type_info& type() const noexcept = 0;

            virtual placeholder * clone() const = 0;

            virtual bool equals(const placeholder& other) const = 0;

            virtual any any_value() const = 0;
        };

        template<typename ValueType>
        class zholder final
          : public placeholder
        {
        public: // structors

            zholder(const ValueType & value)
              : held(value)
            {
            }

            zholder(ValueType&& value)
              : held(static_cast< ValueType&& >(value))
            {
            }

        public: // queries

            virtual const std::type_info& type() const noexcept
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return new zholder(held);
            }

            virtual bool equals(const placeholder& other) const
            {
#ifndef TEST_ZUI_ANY
              return compareValues(held, other.any_value());
#else
              return true;
#endif
            }

            virtual any any_value() const
            {
              return any(held);
            }

        public: // representation

            ValueType held;

        private: // intentionally left unimplemented
            zholder & operator=(const zholder &);
        };

    private: // representation

        template<typename ValueType>
        friend ValueType * any_cast(any *) noexcept;

        template<typename ValueType>
        friend ValueType * unsafe_any_cast(any *) noexcept;

        placeholder * content;
    };
 
    inline void swap(any & lhs, any & rhs) noexcept
    {
        lhs.swap(rhs);
    }

    class bad_any_cast : public std::bad_cast
    {
    public:
        virtual const char * what() const noexcept
        {
            return "boost::bad_any_cast: "
                   "failed conversion using boost::any_cast";
        }
    };

    template<typename ValueType>
    ValueType * any_cast(any * operand) noexcept
    {
        if (!operand)
            return 0;
        
      std::string valueTypeName = typeid(ValueType).name();
      std::string operandTypeName = operand->type().name();

        return valueTypeName == operandTypeName
            ? std::addressof(
                static_cast<any::zholder<typename std::remove_cv<ValueType>::type> *>(operand->content)->held
              )
            : 0;
    }

    template<typename ValueType>
    inline const ValueType * any_cast(const any * operand) noexcept
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
    }

    template<typename ValueType>
    typename std::enable_if<std::is_same<ValueType, any>::value, ValueType>::type
    any_cast(any& operand)
    {
      return operand;
    }

    template<typename ValueType>
    typename std::enable_if<!std::is_same<ValueType, any>::value, ValueType>::type
    any_cast(any & operand)
    {
        using nonref = typename std::remove_reference<ValueType>::type;

        nonref * result = any_cast<nonref>(std::addressof(operand));
        if (!result)
        {
#ifdef TEST_ZUI_ANY
          throw(bad_any_cast());
#endif // TEST_ZUI_ANY
          static nonref fake{};
          return fake;
        }

        // Attempt to avoid construction of a temporary object in cases when 
        // `ValueType` is not a reference. Example:
        // `static_cast<std::string>(*result);` 
        // which is equal to `std::string(*result);`
        typedef typename std::conditional<
            std::is_reference<ValueType>::value,
            ValueType,
            typename add_reference<ValueType>::type
        >::type ref_type;

        return static_cast<ref_type>(*result);
    }

    template<typename ValueType>
    inline ValueType any_cast(const any & operand)
    {
        typedef typename std::remove_reference<ValueType>::type nonref;
        return any_cast<const nonref &>(const_cast<any &>(operand));
    }

    template<typename ValueType>
    inline ValueType any_cast(any&& operand)
    {
      static_assert(
            std::is_rvalue_reference<ValueType&&>::value /*true if ValueType is rvalue or just a value*/
            || std::is_const< typename std::remove_reference<ValueType>::type >::value,
            "boost::any_cast shall not be used for getting nonconst references to temporary objects" 
        );
        return any_cast<ValueType>(operand);
    }

    // Note: The "unsafe" versions of any_cast are not part of the
    // public interface and may be removed at any time. They are
    // required where we know what type is stored in the any and can't
    // use typeid() comparison, e.g., when our types may travel across
    // different shared libraries.
    template<typename ValueType>
    inline ValueType * unsafe_any_cast(any * operand) noexcept
    {
        return std::addressof(
            static_cast<any::zholder<ValueType> *>(operand->content)->held
        );
    }

    template<typename ValueType>
    inline const ValueType * unsafe_any_cast(const any * operand) noexcept
    {
        return unsafe_any_cast<ValueType>(const_cast<any *>(operand));
    }
}
#endif
