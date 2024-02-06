#ifndef MIROIR_MEMBER_HPP
    #define MIROIR_MEMBER_HPP

namespace miroir
{
        // Represents a type that can store information about a type's member.
    template <typename T>
    concept member_info_interface = requires
    {
        // The return type of the member.
        typename T::return_type;
        
        // The type that holds the member.
        typename T::declaring_type;
        
        // The full-type of the member.
        typename T::type;
        
        // The member's name.
        T::name;
        
        // The member's memory address.
        T::raw_ptr;
    };

    // Gets the declaring type of the given `Member`.
    // Usage: `member_declarant<decltype(&struct::member)>::type`
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

    // Alias for `member_declarant<...>::type`.
    // Usage: `member_declarant_t<decltype(&struct::member)>`
    template <typename Member>
    using member_declarant_t = typename member_declarant<Member>::type;

    // Alias for `member_declarant_t<...>`.
    // Usage: `member_ptr_declarant_t<&struct::member>`
    template <auto MemberPtr>
    using member_ptr_declarant_t = member_declarant_t<decltype(MemberPtr)>;

    // Gets the return type of the given `Member`.
    // Usage: `member_return<decltype(&struct::member)>::type`
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

    // Alias for `member_return<...>::type`.
    // Usage: `member_return<decltype(&struct::member)>`
    template <typename Member>
    using member_return_t = typename member_return<Member>::type;

    // Alias for `member_return_t<...>`.
    // Usage: `member_ptr_return_t<&struct::member>`
    template <auto MemberPtr>
    using member_ptr_return_t = member_return_t<decltype(MemberPtr)>;
}

#endif // MIROIR_MEMBER_HPP