#ifndef MIROIR_HPP

namespace miroir
{
    // A compile-time string type that can be used as a constant expression in non-type template parameters.
    template <unsigned int N>
    struct const_string
    {
        char value[N];

        consteval const_string(const char(&in)[N])
        {
            for (unsigned int i = 0; i < N; i++)
            {
                value[i] = in[i];
            }
        }

        // Returns the character at the given index.
        consteval char operator[](unsigned int index)
        {
            return value[index];
        }
    };

    // Compares two const_string objects of the same size.
    template <unsigned int N>
    consteval bool operator==(const_string<N> left, const_string<N> right)
    {
        for (unsigned int i = 0; i < N; i++)
        {
            if (left.value[i] != right.value[i])
            {
                return false;
            }
        }
        return true;
    }

    // Compares two const_string objects of different sizes...
    // Obviously it returns false.
    template <unsigned int N, unsigned int N2>
    consteval bool operator==(const_string<N> left, const_string<N2> right)
    {
        return false;
    }

    // Compares a const_string object with a fixed length string.
    template <unsigned int N>
    consteval bool operator==(const_string<N> left, const char (&right)[N])
    {
        for (unsigned int i = 0; i < N; i++)
        {
            if (left.value[i] != right[i])
            {
                return false;
            }
        }
        return true;
    }

        // Compares a const_string object with a fixed length string.
    template <unsigned int N, unsigned int N2>
    consteval bool operator==(const_string<N> left, const char (&right)[N2])
    {
        if constexpr (N != N2) return false;
        for (unsigned int i = 0; i < N; i++)
        {
            if (left.value[i] != right[i])
            {
                return false;
            }
        }
        return true;
    }

    // Checks if the given func is a function that accepts the given args and returns a value of a certain type.
    template <typename Func, typename Ret, typename ...Args>
    concept invocable = requires(Func func, Args&& ...args)
    {
        static_cast<Ret(*)(Args...)>(func);
    };

    // Gets the declaring type of the given member.
    template <typename Member>
    struct member_declarant;

    template <typename DeclaringType, typename ReturnType>
    struct member_declarant<ReturnType(DeclaringType::*)>
    {
        using type = DeclaringType;
    };
    template <typename DeclaringType, typename ReturnType, typename ...Args>
    struct member_declarant<ReturnType(DeclaringType::*)(Args...)>
    {
        using type = DeclaringType;
    };

    // Shortcut for member_declarant<Member>::type.
    template <typename Member>
    using member_declarant_t = typename member_declarant<Member>::type;

    // Accepts member pointer as template parameter.
    template <auto MemberPtr>
    using member_ptr_declarant_t = member_declarant_t<decltype(MemberPtr)>;

    // Gets the return type of the given member.
    template <typename Member>
    struct member_return;
    
    template <typename DeclaringType, typename ReturnType>
    struct member_return<ReturnType(DeclaringType::*)>
    {
        using type = ReturnType;
    };
    template <typename DeclaringType, typename ReturnType, typename ...Args>
    struct member_return<ReturnType(DeclaringType::*)(Args...)>
    {
        using type = ReturnType;
    };

    // Shortcut for member_return<Member>::type.
    template <typename Member>
    using member_return_t = typename member_return<Member>::type;

    // Accepts member pointer as template parameter.
    template <auto MemberPtr>
    using member_ptr_return_t = member_return_t<decltype(MemberPtr)>;

    // Stores information about a type's field.
    template <auto FieldPtr, const_string FieldName>
    struct field_info
    {
        // Non-instantiable!
        field_info() = delete;

        using return_type = member_ptr_return_t<FieldPtr>;
        using declaring_type = member_ptr_declarant_t<FieldPtr>;
        using type = decltype(FieldPtr);

        static constexpr auto name = FieldName;
        static constexpr auto ptr  = FieldPtr;

        static constexpr void set(declaring_type& object, return_type value) { object.*FieldPtr = value; }

        static constexpr return_type* get_ptr(declaring_type& object) { return &object.*FieldPtr; }
        static constexpr const return_type* get_ptr(const declaring_type& object) { return &object.*FieldPtr; }

        static constexpr return_type& get_ref(declaring_type& object) { return object.*FieldPtr; }
        static constexpr const return_type& get_ref(const declaring_type& object) { return object.*FieldPtr; }
    };

    // Represents a field_info list.
    template <typename ...FieldsInfo>
    struct field_info_list
    {
        // Non-instantiable!
        field_info_list() = delete;

        static constexpr int count = sizeof...(FieldsInfo);

        template <typename Lambda>
        static constexpr void for_each(Lambda&& lambda)
        {
            (lambda.template operator()<FieldsInfo>(), ...);
        }

        // NOTE: implementation is ugly and might be heavy for compilation?
    private:
        template <const_string Name, int Placeholder, typename ...CurrentFieldInfos>
        struct get_impl;
        template <const_string Name, typename ExtractedFieldInfo, typename ...CurrentFieldsInfo>
        struct get_impl<Name, -1, ExtractedFieldInfo, CurrentFieldsInfo...> :
            get_impl<Name, Name == ExtractedFieldInfo::name, ExtractedFieldInfo, CurrentFieldsInfo...> {};
        template <const_string Name, typename ExtractedFieldInfo, typename ...CurrentFieldsInfo>
        struct get_impl<Name, 1, ExtractedFieldInfo, CurrentFieldsInfo...>
        {
            using type = ExtractedFieldInfo;
        };
        template <const_string Name, typename ExtractedFieldInfo, typename ...CurrentFieldsInfo>
        struct get_impl<Name, 0, ExtractedFieldInfo, CurrentFieldsInfo...> :
            get_impl<Name, -1, CurrentFieldsInfo...> {};

    public:
        template <const_string Name>
        using get = typename get_impl<Name, -1, FieldsInfo...>::type;
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

        static constexpr auto name = MethodName;
        static constexpr auto ptr  = MethodPtr;

        template <typename ...Args>
        static constexpr return_type invoke(declaring_type& object, Args&& ...args)
        {
            return (object.*ptr)(args...);
        }

        template <typename ...Args>
        static constexpr return_type invoke(const declaring_type& object, Args&& ...args)
        {
            return (object.*ptr)(args...);
        }
    };

    // A list of method_info.
    template <typename ...MethodsInfo>
    struct method_info_list
    {
        // Non-instantiable!
        method_info_list() = delete;

        static constexpr auto count = sizeof...(MethodsInfo);

        template <typename Lambda>
        static constexpr void for_each(Lambda&& lambda)
        {
            (lambda.template operator()<MethodsInfo>(), ...);
        }

        // Todo: implementation is ugly and might be slow at compile-time? maybe review it one day.
    private:
        template <const_string Name, auto Placeholder, typename ...CurrentMethodsInfo>
        struct get_impl;
        template <const_string Name, typename ExtractedMethodInfo, typename ...CurrentMethodsInfo>
        struct get_impl<Name, -1, ExtractedMethodInfo, CurrentMethodsInfo...> :
            get_impl<Name, Name == ExtractedMethodInfo::name, ExtractedMethodInfo, CurrentMethodsInfo...> {};
        template <const_string Name, typename ExtractedFieldInfo, typename ...CurrentMethodsInfo>
        struct get_impl<Name, true, ExtractedFieldInfo, CurrentMethodsInfo...>
        {
            using type = ExtractedFieldInfo;
        };
        template <const_string Name, typename ExtractedMethodInfo, typename ...CurrentMethodsInfo>
        struct get_impl<Name, false, ExtractedMethodInfo, CurrentMethodsInfo...> :
            get_impl<Name, -1, CurrentMethodsInfo...> {};
    public:
        template <const_string Name>
        using get = typename get_impl<Name, -1, MethodsInfo...>::type;
    };

    // Stores information about a type.
    template <typename T, const_string Name = "", typename FieldInfoList = field_info_list<>, typename MethodInfoList = method_info_list<>>
    struct type_info;
    template <typename T, const_string Name, typename ...FieldsInfo, typename ...MethodsInfo>
    struct type_info<T, Name, field_info_list<FieldsInfo...>, method_info_list<MethodsInfo...>>
    {
        // Non-instantiable!
        type_info() = delete;

        static constexpr auto name = Name;

        using fields_info = field_info_list<FieldsInfo...>;

        using methods_info = method_info_list<MethodsInfo...>;
    };

    template <typename T, const_string Name = "", typename FieldInfoList = field_info_list<>, typename MethodInfoList = method_info_list<>>
    struct type_info_builder;
    template <typename T, const_string Name, typename ...FieldsInfo, typename ...MethodsInfo>
    struct type_info_builder<T, Name, field_info_list<FieldsInfo...>, method_info_list<MethodsInfo...>>
    {
        using result = type_info<T, Name, field_info_list<FieldsInfo...>, method_info_list<MethodsInfo...>>;

        template <const_string Name>
        using set_name = type_info_builder<T, Name, field_info_list<FieldsInfo...>, method_info_list<MethodsInfo...>>;

        template <auto FieldPtr, const_string FieldName>
        using add_field = type_info_builder<T, Name, field_info_list<FieldsInfo..., field_info<FieldPtr, FieldName>>, method_info_list<MethodsInfo...>>;

        template <auto MethodPtr, const_string MethodName>
        using add_method = type_info_builder<T, Name, field_info_list<FieldsInfo...>, method_info_list<MethodsInfo..., method_info<MethodPtr, MethodName>>>;
    };

    // Tells if the given type holds type information.
    template <typename T>
    concept holds_type_info = requires { typename T::type_info; };

    // Used to fetch reflection data from a type `T`.
    // Specializes this class if reflection data can only be located outside the struct/class: can be useful when reflecting 3rd party types.
    template <typename T>
    struct type_info_provider; /*
    {
        using data = type_info<T>::...;
    }; */

    // Tells if the given type exposes type information via `type_info_provider` struct.
    template <typename T>
    concept provides_type_info = requires { typename type_info_provider<T>::data; };

    // Tells if the given type is reflected in miroir.
    template <typename T>
    concept reflected_type = holds_type_info<T> || provides_type_info<T>;

    // Gets type information of a given type.
    template <typename T, bool HasTypeInfo = holds_type_info<T>, bool ProvidesTypeInfo = provides_type_info<T>>
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
    using get_field_info = typename get_type_info<T>::fields_info::template get<Name>;

    // Returns the number of fields information present in a type.
    template <reflected_type T>
    constexpr auto field_count()
    {
        return get_type_info<T>::fields_info::count;
    }

    // Iterates through every field of a reflected type.
    template <reflected_type T, typename Lambda>
    constexpr void for_each_field(Lambda&& lambda)
    {
        get_type_info<T>::fields_info::for_each(lambda);
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
    using get_method_info = typename get_type_info<T>::methods_info::template get<Name>;

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

#define MIROIR_HPP
#endif