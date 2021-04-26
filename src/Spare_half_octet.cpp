#include <_5GS/ie/Spare_half_octet.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{

Spare_half_Octet::Spare_half_Octet(Spare_half_Octet::Value value)
{
    set(value);
}

std::string Spare_half_Octet::getName() const
{
    return "PDU session identity";
}

void Spare_half_Octet::set(Spare_half_Octet::Value id)
{
    m_present = true;
    m_value = id;
}

Spare_half_Octet::Value Spare_half_Octet::get() const
{
    raise_exception_if_not_present(className(this));
    return m_value;
}

int Spare_half_Octet::code_V(std::vector<uint8_t> &data) const
{
    raise_exception_if_not_present(className(this));
    data.push_back(static_cast<uint8_t>(m_value));
    return 1;
}

int Spare_half_Octet::decode_V(const std::vector<uint8_t> &data)
{
    return decode_V(data[2]);
}

int Spare_half_Octet::decode_V(const uint8_t &data)
{
    m_value = Spare_half_Octet::uint8_t_to_Value(data);
    m_present = true;
    return static_cast<uint8_t>(m_value);
}

Spare_half_Octet::Value Spare_half_Octet::uint8_t_to_Value(const uint8_t &byte)
{
    return static_cast<Spare_half_Octet::Value>(byte);
}

std::string Spare_half_Octet::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    return Spare_half_Octet::value_to_string(m_value);
}

std::string Spare_half_Octet::value_to_string(Spare_half_Octet::Value value)
{
    switch (value)
    {
    case Spare_half_Octet::Value::spare:
        return "spare";
    }
    throw std::invalid_argument("Not a value");
}


} // namespace IE
} // namespace _5GS
