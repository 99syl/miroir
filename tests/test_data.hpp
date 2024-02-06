#ifndef MIROIR_TESTS_TEST_DATA_HPP
#define MIROIR_TESTS_TEST_DATA_HPP

#include "../miroir.hpp"

// Invalid reflected type:
struct not_reflected_character
{
    float hp;
    float mana;
    
    using type_Info = miroir::type_info_builder<not_reflected_character>
        ::add_field<&not_reflected_character::hp, "hp">
        ::add_field<&not_reflected_character::mana, "mana">
        ::result;
};

// Reflected types:
#define MIROIR_TESTS_REFLECTED_CHARACTER_TYPE_NAME "reflected_character"
struct reflected_character
{
    float hp;
    float mana;
    
    using type_info = miroir::type_info_builder<reflected_character>
        ::set_name<MIROIR_TESTS_REFLECTED_CHARACTER_TYPE_NAME>
        ::add_field<&reflected_character::hp, "hp">
        ::add_field<&reflected_character::mana, "mana">
        ::result;
};

#define MIROIR_TESTS_REFLECTED_CHARACTER_ARRAY_TYPE_NAME "reflected_character_array"
struct reflected_character_array
{
    reflected_character characters[5];
    
    using type_info = miroir::type_info_builder<reflected_character>
        ::set_name<MIROIR_TESTS_REFLECTED_CHARACTER_ARRAY_TYPE_NAME>
        ::add_field<&reflected_character_array::characters, "characters">
        ::result;
};

// Type reflected outside its declaration:
#define MIROIR_TESTS_REFLECTED_POS2D_TYPE_NAME "reflected_pos2d"
struct reflected_pos2d
{
    int x;
    int y;
    
    void show() {};
};
template <> struct miroir::type_info_provider<reflected_pos2d>
{
    using data = miroir::type_info_builder<reflected_pos2d>
        ::set_name<MIROIR_TESTS_REFLECTED_POS2D_TYPE_NAME>
        ::add_field<&reflected_pos2d::x, "x">
        ::add_field<&reflected_pos2d::y, "y">
        ::add_method<&reflected_pos2d::show, "show">
        ::result;
};

#endif // MIROIR_TESTS_TEST_DATA_HPP