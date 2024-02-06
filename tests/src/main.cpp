#include <cstdlib>
#include <type_traits>
#include <iostream>

#include "test_data.hpp"

void _ensure(bool expr, int line)
{
    if (!expr)
    {
        std::cout << "Test at line " << line << " failed." << std::endl;
        std::exit(1);
    }
}

#define ensure(expr)    \
    _ensure(expr, __LINE__);

void test_const_string()
{
    constexpr miroir::const_string test{"ConstStringTest"};
    constexpr miroir::const_string test2{"ConstStringTest2"};

    ensure(test == "ConstStringTest");
    ensure(test == test);
    ensure(!(test != "ConstStringTest"));
    ensure(test != test2);
    ensure(!(test2 == test));
}

void test_fields_size()
{
    ensure(miroir::fields_size<reflected_character>() == 2);
    ensure(miroir::fields_size<reflected_character_array>() == 1);
    ensure(miroir::fields_size<reflected_pos2d>() == 2);
}

void test_for_each_fields()
{
    int reflected_character_iteration_count = 0;
    miroir::for_each_fields<reflected_character>(
        [&reflected_character_iteration_count]<miroir::field_info_interface FieldInfo>()
        {
            reflected_character_iteration_count++;
        });
    ensure(reflected_character_iteration_count == 2);
    
    int reflected_character_array_iteration_count = 0;
    miroir::for_each_fields<reflected_character_array>(
        [&reflected_character_array_iteration_count]<miroir::field_info_interface FieldInfo>()
        {
            reflected_character_array_iteration_count++;
        });
    ensure(reflected_character_array_iteration_count == 1);
    
    int reflected_pos2_iteration_count = 0;
    miroir::for_each_fields<reflected_pos2d>(
        [&reflected_pos2_iteration_count]<miroir::field_info_interface FieldInfo>()
        {
            reflected_pos2_iteration_count++;
        });
    ensure(reflected_pos2_iteration_count == 2);
}

void test_get_field_info_reflected_character()
{
    ensure((miroir::get_field_info<reflected_character, "hp">::name == "hp"));
    ensure((miroir::get_field_info<reflected_character, "hp">::raw_ptr == &reflected_character::hp));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character, "hp">::return_type, float>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character, "hp">::declaring_type, reflected_character>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character, "hp">::type, decltype(&reflected_character::hp)>));
    
    ensure((miroir::get_field_info<reflected_character, "mana">::name == "mana"));
    ensure((miroir::get_field_info<reflected_character, "mana">::raw_ptr == &reflected_character::mana));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character, "mana">::return_type, float>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character, "mana">::declaring_type, reflected_character>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character, "mana">::type, decltype(&reflected_character::mana)>));
}

void test_get_field_info_reflected_character_array()
{
    ensure((miroir::get_field_info<reflected_character_array, "characters">::name == "characters"));
    ensure((miroir::get_field_info<reflected_character_array, "characters">::raw_ptr == &reflected_character_array::characters));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character_array, "characters">::return_type, reflected_character[5]>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character_array, "characters">::declaring_type, reflected_character_array>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_character_array, "characters">::type, decltype(&reflected_character_array::characters)>));
}

void test_get_field_info_reflected_pos2d()
{
    ensure((miroir::get_field_info<reflected_pos2d, "x">::name == "x"));
    ensure((miroir::get_field_info<reflected_pos2d, "x">::raw_ptr == &reflected_pos2d::x));
    ensure((std::is_same_v<miroir::get_field_info<reflected_pos2d, "x">::return_type, int>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_pos2d, "x">::declaring_type, reflected_pos2d>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_pos2d, "x">::type, decltype(&reflected_pos2d::x)>));
    
    ensure((miroir::get_field_info<reflected_pos2d, "y">::name == "y"));
    ensure((miroir::get_field_info<reflected_pos2d, "y">::raw_ptr == &reflected_pos2d::y));
    ensure((std::is_same_v<miroir::get_field_info<reflected_pos2d, "y">::return_type, int>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_pos2d, "y">::declaring_type, reflected_pos2d>));
    ensure((std::is_same_v<miroir::get_field_info<reflected_pos2d, "y">::type, decltype(&reflected_pos2d::y)>));
}

void test_get_method_info_reflected_pos2d()
{
    ensure((miroir::get_method_info<reflected_pos2d, "show">::name == "show"));
    ensure((miroir::get_method_info<reflected_pos2d, "show">::raw_ptr == &reflected_pos2d::show));
}

void test_reflected_type_concept()
{
    ensure(!miroir::reflected_type<not_reflected_character>);
    ensure(miroir::reflected_type<reflected_character>);
    ensure(miroir::reflected_type<reflected_character_array>);
    ensure(miroir::reflected_type<reflected_pos2d>);
}

void test_type_info()
{
    ensure(miroir::get_type_info<reflected_character>::name == MIROIR_TESTS_REFLECTED_CHARACTER_TYPE_NAME);
    ensure(miroir::get_type_info<reflected_character_array>::name == MIROIR_TESTS_REFLECTED_CHARACTER_ARRAY_TYPE_NAME);
}

int main()
{
    test_const_string();
    test_fields_size();
    test_for_each_fields();
    test_get_field_info_reflected_character();
    test_get_field_info_reflected_character_array();
    test_get_field_info_reflected_pos2d();
    test_get_method_info_reflected_pos2d();
    test_reflected_type_concept();
    test_type_info();

    return 0;
}