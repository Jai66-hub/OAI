#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>

namespace _5GS
{

namespace IE
{

class Spare_half_Octet : public InformationElement
{
public:
 
    /* element is filled with spare bits set to zero and is placed in bits 5 to 8 of the octet 
    unless otherwise specified */
    enum class Value : uint8_t
    {
        spare
    };

    Spare_half_Octet() = default;
    explicit Spare_half_Octet(const Value val);

    void set(const Value value);
    Value get() const;

    int code_V(std::vector<uint8_t> &data) const;
    int decode_V(const std::vector<uint8_t> &data);
    int decode_V(const uint8_t &data);

    virtual std::string getName() const;

    virtual std::string valueToString() const;

    static std::string value_to_string(const Spare_half_Octet::Value value);    

    static Value uint8_t_to_Value(const uint8_t &byte);
private:
    Value m_value;
   
};

} // namespace IE

} // namespace _5GS
