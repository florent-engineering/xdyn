#include "base91.hpp"
#include "generate_test_ship.hpp"
#include "stl_reader.hpp"

VectorOfVectorOfPoints test_ship() // Generated by convert_stl_files_to_code
{
    #include "test_ship_data.hpp"
    return read_binary_stl(base<91>::decode(s));
}
