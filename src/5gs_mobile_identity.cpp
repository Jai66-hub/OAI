#include <_5GS/ie/5gs_mobile_identity.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{

std::string _5GS_mobile_identity::getName() const
{
    return "5GS mobile identity";
}

// Constructors

_5GS_mobile_identity::_5GS_mobile_identity(Identity Identity)
{
    printf("_5GS_mobile_identity(Identity Identity): %x\n", Identity);
    set(Identity);
}

void _5GS_mobile_identity::set(_5GS_mobile_identity::Identity Identity)
{
    m_present = true;
    m_Identity = Identity;
}

// getters & setters

_5GS_mobile_identity::Identity _5GS_mobile_identity::get() const
{
    raise_exception_if_not_present(className(this));
    return m_Identity;
}

// V coders and decoders

uint8_t _5GS_mobile_identity::code_half_V() const
{
    return static_cast<uint8_t>(m_Identity);
}

int _5GS_mobile_identity::code_V(std::vector<uint8_t> &data) const
{
    printf("code_V, _5GS_mobile_identity\n");
    raise_exception_if_not_present(className(this));
    data.push_back(static_cast<uint8_t>(m_Identity));
    return 1;
}

int _5GS_mobile_identity::decode_V(const std::vector<uint8_t> &data)
{
    return decode_V(data[0]);
}

int _5GS_mobile_identity::decode_V(const uint8_t &data)
{
    printf("_5GS_mobile_identity::decode_V\n");
    uint8_t v;
    v = data & 0x07;    
    //m_value = _5GS_mobile_identity::uint8_t_to_Value(v);
    m_Identity = uint8_t_to_Identity(v);
    m_present = true;
    return 1;
}

int _5GS_mobile_identity::decode_LV_E(std::vector<uint8_t> &data)
{
    return decode_LV_E(data[0]);
}

int _5GS_mobile_identity::decode_LV_E(const uint8_t &data)
{
    printf("_5GS_mobile_identity::decode_LV_E\n");
    int size = 0;
    return 6+size;
}

// Converter functions

_5GS_mobile_identity::Identity _5GS_mobile_identity::uint8_t_to_Identity(const uint8_t &byte)
{
    printf("_5GS_mobile_identity t to Identity Byte: %x\n", byte);
    switch (byte)
    {
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::No_identity):
        return _5GS_mobile_identity::Identity::No_identity;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::SUCI):
        return _5GS_mobile_identity::Identity::SUCI;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::_5G_GUTI):
        return _5GS_mobile_identity::Identity::_5G_GUTI;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::IMEI):
        return _5GS_mobile_identity::Identity::IMEI;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::_5G_S_TMSI):
        return _5GS_mobile_identity::Identity::_5G_S_TMSI;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::IMEISV):
        return _5GS_mobile_identity::Identity::IMEISV;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::MAC_address):
        return _5GS_mobile_identity::Identity::MAC_address;
    case static_cast<uint8_t>(_5GS_mobile_identity::Identity::EUI_64):
        return _5GS_mobile_identity::Identity::EUI_64;
    }
    throw NasCodecException(
        std::string(__PRETTY_FUNCTION__) +
        std::string(" Invalid data:\n") + std::to_string(byte));
}

// Strings function

std::string _5GS_mobile_identity::IdentityToString() const
{
    if (not isSet())
    {
        return "-";
    }
    return _5GS_mobile_identity::Identity_to_string(m_Identity);
}

std::string _5GS_mobile_identity::Identity_to_string(const _5GS_mobile_identity::Identity Identity)
{
    switch (Identity)
    {
    case _5GS_mobile_identity::Identity::No_identity:
        return "No identity";
    case _5GS_mobile_identity::Identity::SUCI:
        return "SUCI";
    case _5GS_mobile_identity::Identity::_5G_GUTI:
        return "5G GUTI";
    case _5GS_mobile_identity::Identity::IMEI:
        return "IMEI";
    case _5GS_mobile_identity::Identity::_5G_S_TMSI:
        return "5G S TMSI";
    case _5GS_mobile_identity::Identity::IMEISV:
        return "IMEISV";
    case _5GS_mobile_identity::Identity::MAC_address:
        return "MAC address";
    case _5GS_mobile_identity::Identity::EUI_64:
        return "EUI 64";
    }
    throw std::invalid_argument("Not a Identity");
}

} // namespace IE
} // namespace _5GS
