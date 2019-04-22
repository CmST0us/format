#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
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
        byte_array() {

        }

        byte_array(const format::data& data) {
            this->_data.write(data.c_str(), static_cast<std::streamsize>(data.size()));
        }

        ~byte_array() {

        }

        int m_position{ 0 };

        int get_length() {
            return static_cast<int>(this->_data.str().size());
        }

        int get_bytes_available() {
            return this->get_length() - this->m_position;
        }

        const format::data get_data() {
            return this->_data.str();
        }

        byte_array& write_byte(uint8_t v) {
            this->_data << v;
            this->m_position += format::UINT8_SIZE;
            return *this;
        }

        byte_array& write(uint8_t v) {
            return this->write_byte(v);
        }

        byte_array& write(uint16_t v) {
            return this->write<uint16_t, format::UINT16_SIZE>(v);
        }

        byte_array& write(uint32_t v) {
            return this->write<uint32_t, format::UINT32_SIZE>(v);
        }

        byte_array& write(int8_t v) {
            return this->write_byte((uint8_t)v);
        }

        byte_array& write(int16_t v) {
            return this->write<int16_t, format::INT16_SIZE>(v);
        }

        byte_array& write(int32_t v) {
            return this->write<int32_t, format::INT32_SIZE>(v);
        }

        template<class T, int size>
        byte_array& write(T v) {
            if (sizeof(T) != size) {
                throw format::error::parse;
            }

            number_byte<T, size> b;
            b.v = v;

            if (size == 4) {
                b.v = BYTESWAP_32(b.v);
            } else if (size == 2) {
                b.v = BYTESWAP_16(b.v);
            }

            for (int i = 0; i < size; ++i) {
                this->write_byte(b.b[i]);
            }
            return *this;
        }

        // [TODO]: write int24 uint24
        // create new class int24, class uint24
        byte_array& write(uint8_t *data, int length) {
            for (int i = 0; i < length; i++) {
                this->write_byte(data[i]);
            }
            return *this;
        }

        byte_array& write(format::data &data) {
            for (auto i = data.begin(); i != data.end(); i++) {
                this->write_byte((uint8_t)*i);
            }
            return *this;
        }

        uint8_t read_byte() {
            uint8_t v = 0;
            this->_data >> v;
            this->m_position += format::UINT8_SIZE;
            return v;
        }

        uint8_t read_uint8() {
            if (this->get_bytes_available() < format::UINT8_SIZE) {
                throw format::error::eof;
            }
            return this->read_byte();
        }

        uint16_t read_uint16() {
            return read<uint16_t, format::UINT16_SIZE>();
        }

        uint32_t read_uint32() {
            return read<uint32_t, format::UINT32_SIZE>();
        }

        int8_t read_int8() {
            if (this->get_bytes_available() < format::INT8_SIZE) {
                throw format::error::eof;
            }
            return (int8_t)this->read_byte();
        }

        int16_t read_int16() {
            return read<int16_t, format::INT16_SIZE>();
        }

        int32_t read_int32() {
            return read<int32_t, format::INT32_SIZE>();
        }

        template<class T, int size>
        T read() {
            if (sizeof(T) != size) {
                throw format::error::parse;
            }

            number_byte<T, size> b;
            
            if (this->get_bytes_available() < size) {
                throw format::error::eof;
            }

            for (int i = 0; i < size; ++i) {
                b.b[i] = this->read_byte();
            }

            if (size == 4) {
                b.v = BYTESWAP_32(b.v);
            } else if (size == 2) {
                b.v = BYTESWAP_16(b.v);
            }

            return (T)b.v;
        }

        format::data read_data(int len) {
            if (this->get_bytes_available() < len) {
                throw format::error::eof;
            }
            char *buf = new char[len];
            ::memset(buf, 0, len);
            this->_data.read(buf, len);
            this->m_position += len;
            format::data d(buf, len);
            delete [] buf;
            return d;
        }

    private:
        std::stringstream _data{std::stringstream(std::ios::in | std::ios::out | std::ios::binary)};
    };
}
