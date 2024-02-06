#ifndef MIROIR_METHOD_HPP
    #define MIROIR_METHOD_HPP

#include "const_string.hpp"
#include "invocable.hpp"
#include "member.hpp"

namespace miroir
{
    // Represents a type that can store information about a type's method.
    // Todo: to be implemented.
    template <typename T>
    concept method_info_interface = member_info_interface<T> and requires
    {
        true;
    };

    // Stores information about a type's method.
    template <auto MethodPtr, const_string MethodName>
    struct method_info
    {
        // Non-instantiable!
        ~method_info() = delete;

        // The return type of the method.
        using return_type = member_ptr_return_t<MethodPtr>;
        
        // The type that declares the method.
        using declaring_type = member_ptr_declarant_t<MethodPtr>;

        // The signature of the method.
        using type = decltype(MethodPtr);
        
        static constexpr auto name{MethodName};
        static constexpr auto name_raw{MethodName.value};
        static constexpr auto raw_ptr{MethodPtr};
        
        template <typename ...Args>
        static constexpr return_type invoke(declaring_type& object, Args&& ...args)
        {
            return (object.*raw_ptr)(args...);
        }
        
        template <typename ...Args>
        static constexpr return_type invoke(const declaring_type& object, Args&& ...args)
        {
            return (object.*raw_ptr)(args...);
        }
    };

    // A meta container of types that follows `method_info_interface` concept.
    template <method_info_interface ...MethodInfos>
    struct method_info_list
    {
        // Non-instantiable!
        method_info_list() = delete;

        // The number of types respecting the `method_info_interface` concept in the template.
        static constexpr auto size{sizeof...(MethodInfos)};

        template <typename Lambda>
        static constexpr void for_each(Lambda&& lambda)
        {
            (lambda.template operator()<MethodInfos>(), ...);
        }
    
        // method_info_list::get.
        // Returns the method corresponding to the given name.
        // Usage: `method_info_list::get<"name">`
        // Todo: I do not like current implementation. Maybe review it someday.
      private:
        template <const_string Name, auto Placeholder, method_info_interface ...CurrentMethodInfos>
        struct get_impl;
        template <const_string Name, method_info_interface ExtractedMethodInfo, method_info_interface ...CurrentMethodInfos>
        struct get_impl<Name, -1, ExtractedMethodInfo, CurrentMethodInfos...> :
            get_impl<Name, Name == ExtractedMethodInfo::name, ExtractedMethodInfo, CurrentMethodInfos...> {};
        template <const_string Name, method_info_interface ExtractedFieldInfo, method_info_interface ...CurrentMethodInfos>
        struct get_impl<Name, true, ExtractedFieldInfo, CurrentMethodInfos...>
        {
            using type = ExtractedFieldInfo;
        };
        template <const_string Name, method_info_interface ExtractedMethodInfo, method_info_interface ...CurrentMethodInfos>
        struct get_impl<Name, false, ExtractedMethodInfo, CurrentMethodInfos...> :
            get_impl<Name, -1, CurrentMethodInfos...> {};
      public:
        template <const_string Name>
        using get = typename get_impl<Name, -1, MethodInfos...>::type;
    };
}

#endif // MIROIR_METHOD_HPP