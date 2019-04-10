#include "format.hpp"

void test_uint8() {
    format::byte_array b;
    uint8_t v = 1;
    uint8_t v2 = 0;
    b.write(v).write(v2).write(v);
    auto l = b.get_bytes_available();
    std::cout << l;
    uint8_t r = b.read_uint8();
    uint8_t s = b.read_uint8();
    uint8_t t = b.read_uint8();
}

void test_uint16() {
    format::byte_array b;
    uint16_t v = 0x0102;
    uint16_t v1 = 0x0304;
    b.write(v1).write(v).write(v1);
    b.m_position = 0;
    auto l = b.get_bytes_available();
    uint16_t r = b.read_uint16();
    l = b.get_bytes_available();
    uint16_t s = b.read_uint16();
    l = b.get_bytes_available();
    uint16_t t = b.read_uint16();
}

void test_uint32() {
    format::byte_array b;
    uint32_t v = 0x01020304;
    uint32_t v1 = 0x05060708;
    b.write(v1).write(v).write(v1);
    b.m_position = 0;
    auto l = b.get_bytes_available();
    uint32_t r = b.read_uint32();
    l = b.get_bytes_available();
    uint32_t s = b.read_uint32();
    l = b.get_bytes_available();
    uint32_t t = b.read_uint32();
    l = b.get_bytes_available();
}

int main(int argc, char *argv[]) {

    test_uint32();
    
    return 0;
}