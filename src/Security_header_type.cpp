#include <_5GS/ie/Security_header_type.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{

Security_header_type::Security_header_type(Security_header_type::Value value)
{
    set(value);
}

std::string Security_header_type::getName() const
{
    return "Security header type";
}

void Security_header_type::set(Security_header_type::Value id)
{
    m_present = true;
    m_value = id;
}

Security_header_type::Value Security_header_type::get() const
{
    raise_exception_if_not_present(className(this));
    return m_value;
}

int Security_header_type::code_V(std::vector<uint8_t> &data) const
{
    raise_exception_if_not_present(className(this));
    data.push_back(static_cast<uint8_t>(m_value));
    return 1;
}

int Security_header_type::decode_V(const std::vector<uint8_t> &data)
{
    if(data.size() < 2)
    {
        std::cout<<"No security header data present"<<std::endl;
        return -1; // error 
    }

    return decode_V(data[1]);
}

int Security_header_type::decode_V(const uint8_t &data)
{
    m_value = Security_header_type::uint8_t_to_Value(data);
    m_present = true;
 
    return static_cast<uint8_t>(m_value);
}

Security_header_type::Value Security_header_type::uint8_t_to_Value(const uint8_t &byte)
{
    if (byte > static_cast<uint8_t>(Security_header_type::Value::reserved))
    {
        return Security_header_type::Value::reserved;
    }
    return static_cast<Security_header_type::Value>(byte);
}

std::string Security_header_type::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    return Security_header_type::value_to_string(m_value);
}

std::string Security_header_type::value_to_string(Security_header_type::Value value)
{
    switch (value)
    {
    case Security_header_type::Value::Plain_5gs_NAS_msg_not_security_protected:
        return "Plain 5GS NAS message, not security protected";
    case Security_header_type::Value::Integrity_protected:
        return "Integrity protected";
    case Security_header_type::Value::Integrty_protected_and_ciphered:
        return "Integrity protected and ciphered";
    case Security_header_type::Value::Integrty_protected_new_5gNas_security:
        return "Integrity protected with new 5G NAS security context";
    case Security_header_type::Value::Integrty_protected_and_ciphered_new_5gNas_security:
        return "Integrity protected and ciphered with new 5G NAS security context";
    // All other value are "Reserved"
    default:
        return "Reserved";
    }
    throw std::invalid_argument("Not a value");
}

} // namespace IE
} // namespace _5GS
