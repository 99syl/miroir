#include <concepts>     //> std::default_initializable.
#include <iostream>     //> std::cout.

#include <nlohmann/json.hpp>

#include <miroir/type.hpp>
#include <miroir/ostream.hpp>

// Deserializes a JSON representation into a valid C++ object of the given reflected type `T`.
template <std::default_initializable T> 
[[nodiscard]] T deserialize(const nlohmann::json& json)
{
    T result {};
    miroir::for_each_fields<T>([&result, &json] <miroir::field_info_interface FieldInfo>()
                               {
                                   if constexpr (miroir::reflected_type<typename FieldInfo::return_type>)
                                   {
                                       using field_return_type = typename FieldInfo::return_type;
                                       FieldInfo::set(result,
                                                      deserialize<field_return_type>(
                                                          json[field_return_type::type_info::name_raw]));
                                   }
                                   else
                                   {
                                       FieldInfo::set(result, json[FieldInfo::name_raw]);
                                   }
                               });
    return result;
}

// Serialize a C++ object of the given reflected type `T` into a valid JSON representation.
template <miroir::reflected_type T>
[[nodiscard]] nlohmann::json serialize(const T& in)
{
    nlohmann::json result;
    miroir::for_each_fields<T>([&in, &result] <miroir::field_info_interface FieldInfo>()
                               {
                                   if constexpr (miroir::reflected_type<typename FieldInfo::return_type>)
                                   {
                                       result[FieldInfo::name_raw] = serialize(FieldInfo::get(in));
                                   }
                                   else
                                   {
                                       result[FieldInfo::name_raw] = FieldInfo::get(in);
                                   }
                               });
    return result;
}

struct character_data
{
    int exp;
    int enemies_killed;

    // Generates reflection data for `character_data`.
    using type_info = miroir::type_info<character_data>
        ::with_name<"character_data">
        
        ::with_field<&character_data::exp, "exp">
        ::with_field<&character_data::enemies_killed, "enemies_killed">;
};

struct game_save
{
    long            time_played;
    character_data  character_data;

    // Generates reflection data for `game_save`.
    using type_info = miroir::type_info<game_save>
        ::with_name<"game_save">

        ::with_field<&game_save::time_played, "time_played">
        ::with_field<&game_save::character_data, "character_data">;
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

    std::cout << "Json data: " << json_data.dump() << std::endl;
    std::cout << "C++ data deserialized from json above: " << save << std::endl;
    std::cout << "Json data serialized from C++ data: " << json_save << std::endl;
    
    return 0;
}
