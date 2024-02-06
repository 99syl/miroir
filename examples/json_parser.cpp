#include <concepts>     //> std::default_initializable.
#include <iostream>     //> std::cout.

struct json;
// #include <your_favorite_json_library.h>

#include "ostream.hpp"

template <std::default_initializable T> 
T deserialize(const json& json)
{
    T result {};
    miroir::for_each_field<T>([&result, &json]<typename FieldInfo>()
    {
        if constexpr (miroir::reflected_type<typename FieldInfo::return_type>)
        {
            using field_return_type = typename FieldInfo::return_type;
            FieldInfo::set(result,
                           deserialize<field_return_type>(
                               json[field_return_type::type_info::name.value]));
        }
        else
        {
            FieldInfo::set(result, json[FieldInfo::name.value]);
        }
    });
    return result;
}

// Serialize a C++ object of the given reflected type `T` into a valid JSON representation.
template <miroir::reflected_type T>
json serialize(const T& in)
{
    json result;
    miroir::for_each_field<T>([&in, &result]<typename FieldInfo>()
    {
        if constexpr (miroir::reflected_type<typename FieldInfo::return_type>)
        {
            result[FieldInfo::name.value] = serialize(FieldInfo::get(in));
        }
        else
        {
            result[FieldInfo::name.value] = FieldInfo::get(in);
        }
    });
    return result;
}

struct character_data
{
    int exp;
    int enemies_killed;

    // Generates reflection data for `character_data`.
    using type_info = miroir::type_info_builder<character_data>
        ::set_name<"character_data">
        
        ::set_field<&character_data::exp, "exp">
        ::set_field<&character_data::enemies_killed, "enemies_killed">

        ::result;
};

struct game_save
{
    long            time_played;
    character_data  character_data;

    // Generates reflection data for `game_save`.
    using type_info = miroir::type_info_builder<game_save>
        ::set_name<"game_save">

        ::add_field<&game_save::time_played, "time_played">
        ::add_field<&game_save::character_data, "character_data">
        
        ::result;
};

int main()
{
    auto json_data = R"(
    {
        "time_played": 238290,
        "character_data":
        {
            "exp": 2389043,
            "enemies_killed": 12000
        }
    })"_json;
    std::default_initializable auto save = deserialize<game_save>(json_data);
    auto json_save = serialize<game_save>(save);
    return 0;
}
