#ifndef MIROIR_FIELD_HPP
    #define MIROIR_FIELD_HPP

#include "const_string.hpp"
#include "invocable.hpp"
#include "member.hpp"

namespace miroir
{
    // Represents a type that can store information about a type's field.
    template <typename T>
    concept field_info_interface = member_info_interface<T> and requires
    {
        { T::set } -> invocable<void, typename T::declaring_type&, typename T::return_type>;
    
        { static_cast<typename T::return_type& (*)(typename T::declaring_type&)>(T::get) }
            -> invocable<typename T::return_type&, typename T::declaring_type&>;
        
        { static_cast<const typename T::return_type& (*)(const typename T::declaring_type&)>(T::get) }
            -> invocable<const typename T::return_type&, const typename T::declaring_type&>;
    };

    // Stores information about a type's field.
    template <auto FieldPtr, const_string FieldName>
    struct field_info
    {
        // Non-instantiable!
        field_info() = delete;

        using return_type    = member_ptr_return_t<FieldPtr>;
        using declaring_type = member_ptr_declarant_t<FieldPtr>;
        using type           = decltype(FieldPtr);

        static constexpr auto name{FieldName};
        static constexpr auto name_raw{FieldName.value};
        static constexpr auto raw_ptr{FieldPtr};

        static constexpr void set(declaring_type& object, return_type value) { object.*FieldPtr = value; }

        static constexpr return_type&       get(declaring_type& object) { return object.*FieldPtr; }
        static constexpr const return_type& get(const declaring_type& object) { return object.*FieldPtr; }
    };

    // A meta container of types that follows `field_info_interface` concept.
    template <field_info_interface ...FieldInfos>
    struct field_info_list
    {
        // Non-instantiable!
        field_info_list() = delete;

        // The number of types respecting the `field_info_interface` concept in the template.
        static constexpr auto size{sizeof...(FieldInfos)};
        
        template <typename Lambda>
        static constexpr void for_each(Lambda&& lambda)
        {
            (lambda.template operator()<FieldInfos>(), ...);
        }
  
        // field_info_list::get<const_string Name>
        // Returns the field corresponding to the given name.
        // Usage: `field_info_list::get<"name">`
        // Todo: implementation is ugly (placeholder)? maybe review it one day.
      private:
        template <const_string Name, auto Placeholder, field_info_interface ...CurrentFieldInfos>
        struct get_impl;
        template <const_string Name, field_info_interface ExtractedFieldInfo, field_info_interface ...CurrentFieldInfos>
        struct get_impl<Name, -1, ExtractedFieldInfo, CurrentFieldInfos...> :
            get_impl<Name, Name == ExtractedFieldInfo::name, ExtractedFieldInfo, CurrentFieldInfos...> {};
        template <const_string Name, field_info_interface ExtractedFieldInfo, field_info_interface ...CurrentFieldInfos>
        struct get_impl<Name, true, ExtractedFieldInfo, CurrentFieldInfos...>
        {
            using type = ExtractedFieldInfo;
        };
        template <const_string Name, field_info_interface ExtractedFieldInfo, field_info_interface ...CurrentFieldInfos>
        struct get_impl<Name, false, ExtractedFieldInfo, CurrentFieldInfos...> :
            get_impl<Name, -1, CurrentFieldInfos...> {};
      public:
        template <const_string Name>
        using get = typename get_impl<Name, -1, FieldInfos...>::type;
    };
}

#endif // MIROIR_FIELD_HPP