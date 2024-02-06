#ifndef MIROIR_TYPE_HPP
    #define MIROIR_TYPE_HPP

#include "field.hpp"
#include "method.hpp"

namespace miroir
{
    // Represents a type that can store a type information.
    template <typename T>
    concept type_info_interface = requires
    {
        T::name;
    
        typename T::field_infos;
        typename T::method_infos;
    };

    // Stores information about a type.
    template <typename T, const_string Name = "", typename FieldInfoList = field_info_list<>, typename MethodInfoList = method_info_list<>>
    struct type_info;
    template <typename T, const_string Name, typename ...FieldInfos, typename ...MethodInfos>
    struct type_info<T, Name, field_info_list<FieldInfos...>, method_info_list<MethodInfos...>>
    {
        // Non-instantiable!
        type_info() = delete;
        
        static constexpr auto name{Name};
        static constexpr auto name_raw{Name.value};

        using field_infos = field_info_list<FieldInfos...>;

        using method_infos = method_info_list<MethodInfos...>;

        // Helper alias used to build your `type_info`.
        // Specifies your `type_info`'s name.
        // Usage: type_info::with_name<"A_Name">;
        template <const_string TypeName>
        using with_name = type_info<T, TypeName, field_infos, method_infos>;

        // Helper alias used to build your `type_info`.
        // Adds a `field_info` to your `type_info`.
        // Usage: type_info::with_field<&struct::field, "FieldName">;
        template <auto MemberPtr, const_string MemberName>
        using with_field = type_info<T, Name, field_info_list<FieldInfos..., field_info<MemberPtr, MemberName>>>;

        // Helper alias used to build your `type_info`.
        // Adds a `method_info` to your `type_info`.
        // Usage: type_info::with_method<&struct::method, "MethodName">;
        template <auto MethodPtr, const_string MethodName>
        using with_method = type_info<T, Name, field_infos, method_info_list<MethodInfos..., method_info<MethodPtr, MethodName>>>;
    };

    // Tells if the given type holds type information.
    template <typename T>
    concept has_type_info = type_info_interface<typename T::type_info>;

    // Used to fetch reflection data from a type `T`.
    // Specializes this class if reflection data needs to be located outside the type: can be useful when reflecting 3rdParty types.
    template <typename T>
    struct type_info_provider; /*
    {
        using data = type_info<T>::...;
    }; */

    // Tells if the given type exposes type information via `type_info_provider` struct.
    template <typename T>
    concept provides_type_info = type_info_interface<typename type_info_provider<T>::data>;

    // Tells if the given type is reflected in miroir.
    template <typename T>
    concept reflected_type = has_type_info<T> || provides_type_info<T>;

    // Gets type information of a given type.
    // Usage: `miroir::get_type_info<T>`
    template <typename T, bool HasTypeInfo = has_type_info<T>, bool ProvidesTypeInfo = provides_type_info<T>>
    struct get_type_info_impl;
    template <typename T>
    struct get_type_info_impl<T, true, false>
    {
        using data = typename T::type_info;
    };
    template <typename T>
    struct get_type_info_impl<T, false, true>
    {
        using data = typename type_info_provider<T>::data;
    };
    template <typename T> struct get_type_info_impl<T, true, true>;
    template <typename T> struct get_type_info_impl<T, false, false>;
    template <typename T> using get_type_info = typename get_type_info_impl<T>::data;

    // Returns the name of a reflected type.
    template <reflected_type T>
    constexpr auto get_type_name()
    {
        return get_type_info<T>::name;
    }

    // Returns a field info from the given reflected type and field name.
    template <reflected_type T, const_string Name>
    using get_field_info = typename get_type_info<T>::field_infos::template get<Name>;

    // Returns the number of fields present inside a type info.
    template <type_info_interface TypeInfo>
    constexpr auto fields_size()
    {
        return TypeInfo::field_infos::size;
    };

    // Returns the number of fields information present in a type.
    template <reflected_type T>
    constexpr auto fields_size()
    {
        return fields_size<get_type_info<T>>();
    }

    // Iterates through every field of a type info.
    template <type_info_interface TypeInfo, typename Lambda>
    constexpr void for_each_fields(Lambda&& lambda)
    {
        TypeInfo::field_infos::for_each(lambda);
    }
    
    // Iterates through every field of a reflected type.
    template <reflected_type T, typename Lambda>
    constexpr void for_each_fields(Lambda&& lambda)
    {
        for_each_fields<get_type_info<T>>(lambda);
    }

    // Sets data to a reflected object.
    template <const_string FieldName, reflected_type T, typename Value>
    constexpr void set(T& object, Value value) 
    { 
        get_field_info<T, FieldName>::set(object, value);
    }

    // Gets data from a reflected object.
    template <const_string FieldName, reflected_type T>
    constexpr auto get(T& object) 
    { 
        return get_field_info<T, FieldName>::get(object);
    }
    template <const_string FieldName, reflected_type T>
    constexpr auto get(const T& object) 
    { 
        return get_field_info<T, FieldName>::get(object);
    }

    // Returns a method info from the given reflected type and method name.
    template <reflected_type T, const_string Name>
    using get_method_info = typename get_type_info<T>::method_infos::template get<Name>;

    // Invokes a method from a reflected object.
    template <const_string MethodName, reflected_type T, typename ...Args>
    constexpr auto invoke(T& obj, Args&& ...args)
    {
        return get_method_info<T, MethodName>::invoke(obj, args...);
    }
    template <const_string MethodName, reflected_type T, typename ...Args>
    constexpr auto invoke(const T& obj, Args&& ...args)
    {
        return get_method_info<T, MethodName>::invoke(obj, args...);
    }
}

#endif // MIROIR_TYPE_HPP