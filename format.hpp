#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

#undef _BIG_ENDIAN
// Set Current System Endian
// define _BIG_ENDIAN if in big-endian system
//
#ifdef _BIG_ENDIAN
#define BYTESWAP_16(x) x

#define BYTESWAP_32(x) x

#else
//16λ
#define BYTESWAP_16(x) \
    (uint16_t)((((uint16_t)(x) & 0x00ff) << 8) | \
              (((uint16_t)(x) & 0xff00) >> 8) \
             )

//32λ               
#define BYTESWAP_32(x) \
    (uint32_t)((((uint32_t)(x) & 0xff000000) >> 24) | \
              (((uint32_t)(x) & 0x00ff0000) >> 8) | \
              (((uint32_t)(x) & 0x0000ff00) << 8) | \
              (((uint32_t)(x) & 0x000000ff) << 24) \
              )
#endif

namespace format
{
    static const int UINT8_SIZE = 1;
    static const int UINT16_SIZE = 2;
    static const int UINT32_SIZE = 4;
    static const int INT8_SIZE = 1;
    static const int INT16_SIZE = 2;
    static const int INT32_SIZE = 4;

    template<class T,int C>
    union number_byte {
        T v;
        uint8_t b[C];
    };

    using uint32_byte = number_byte<uint32_t, format::UINT32_SIZE>;
    //using uint24_byte = number_byte<uint32_t, 3>;
    using uint16_byte = number_byte<uint16_t, format::UINT16_SIZE>;
    using uint8_byte = number_byte<uint8_t, format::UINT8_SIZE>;
    using int32_byte = number_byte<int32_t, format::INT32_SIZE>;
    //using int24_byte = number_byte<int32_t, 3>;
    using int16_byte = number_byte<int16_t, format::INT16_SIZE>;
    using int8_byte = number_byte<int8_t, format::INT8_SIZE>;

    enum class error {
        eof,
        parse,
    };

    using mutable_data = std::stringstream;
    using data = std::string;

    class byte_array {
    public:
        byte_array();
        byte_array(format::data& data);
        ~byte_array();

        int m_position{ 0 };

        int get_length() {
            return this->_data.str().size();
        }

        int m_bytes_available{ 0 };

        byte_array& write_byte(uint8_t v) {
            this->_data << v;
            this->m_position += format::UINT8_SIZE;
            return *this;
        }

        byte_array& write(uint8_t v) {
            return this -> write_byte(v);
        }

        byte_array& write(uint16_t v) {
            uint16_byte b;
            b.v = BYTESWAP_16(v);
            for (int i = 0; i < format::UINT16_SIZE; ++i) {
                this->write_byte(b.b[i]);
            }
            return *this;
        }

        byte_array& write(uint32_t v);
        byte_array& write(int8_t v);
        byte_array& write(int16_t v);
        byte_array& write(int32_t v);
        // [TODO]: write int24 uint24
        // create new class int24, class uint24
        byte_array& write(uint8_t *data, int length);

        uint8_t read_byte() {
            uint8_t v = 0;
            this->_data >> v;
            this->m_position -= format::UINT8_SIZE;
            return v;
        }
        uint8_t read_uint8() {
            return this->read_byte();
        }

        uint16_t read_uint16() {
            uint16_byte b;
            for (int i = 0; i < format::UINT16_SIZE; ++i) {
                b.b[i] = this->read_byte();
            }
            b.v = BYTESWAP_16(b.v);
            return b.v;
        }

        uint32_t read_uint32();
        int8_t read_int8();
        int16_t read_int16();
        int32_t read_int32();
        int read_data(uint8_t *buf, int length);

    private:
        std::stringstream _data{std::stringstream(std::ios::in | std::ios::out | std::ios::binary)};
    };

    byte_array::byte_array() {

    }

    byte_array::byte_array(format::data& data) {

    }

    byte_array::~byte_array() {

    }
}