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

void test_data() {
    uint8_t bin[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00, 0x01 };
    format::data data((const char *)bin, sizeof(bin));
    format::byte_array b(data);
    auto l = b.get_length();
    auto ll = b.get_bytes_available();
    format::data rdata = b.read_data(10);
    auto p = rdata.data();
    for (int i = 0; i < 10; i++) {
        printf("%X ", p[i]);
    }
}

void test_decode() {
    uint8_t bin[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00, 0x01 };
    format::data data((const char *)bin, sizeof(bin));
    format::byte_array b(data);

    auto aa = b.read_uint8();
    auto bb = b.read_uint16();
    auto cc = b.read_uint32();

}
int main(int argc, char *argv[]) {

    test_decode();

    return 0;
}