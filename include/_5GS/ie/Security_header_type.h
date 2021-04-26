#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>

namespace _5GS
{

namespace IE
{

class Security_header_type : public InformationElement
{
public:
    // TS 24.007 - 11.2.3.1b.1

    enum class Value : uint8_t
    {
        //Not security protected 
        Plain_5gs_NAS_msg_not_security_protected,
        // Security protected messages 
        Integrity_protected,
        Integrty_protected_and_ciphered,
        Integrty_protected_new_5gNas_security,
        Integrty_protected_and_ciphered_new_5gNas_security,
        // all other values are reserved
        reserved
    };

    Security_header_type() = default;
    explicit Security_header_type(const Value val);

    void set(const Value value);
    Value get() const;

    int code_V(std::vector<uint8_t> &data) const;
    int decode_V(const std::vector<uint8_t> &data);
    int decode_V(const uint8_t &data);

    virtual std::string getName() const;
    virtual std::string valueToString() const;

    static std::string value_to_string(const Security_header_type::Value value);
    static Value uint8_t_to_Value(const uint8_t &byte);

private:
    Value m_value;
};

} // namespace IE

} // namespace _5GS
