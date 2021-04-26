#include <header.h>
#include <buffers.h>

int Nas::codeProtocolDiscriminator(std::vector<uint8_t> &data, const Nas::ProtocolDiscriminator protocol)
{
    data.push_back(static_cast<uint8_t>(protocol));
    return 1;
}

int Nas::codeSecurityHeaderType(std::vector<uint8_t> &data, const Nas::SecurityHeaderType security)
{
    data.push_back(static_cast<uint8_t>(security));
    return 1;
}

int Nas::codeSparehalfoctet(std::vector<uint8_t> &data,const Nas::Sparehalfoctet halfoctet)
{
    data.push_back(static_cast<uint8_t>(halfoctet));
    return 1;
}

// No exception, sends reserved when unknown
Nas::ProtocolDiscriminator Nas::decodeProtocolDiscriminator(const std::vector<uint8_t> &data)
{
    try
    {
        return Nas::uint8_t_to_ProtocolDiscriminator(data[0]);
    }
    catch (const std::invalid_argument &exception)
    {
        return Nas::ProtocolDiscriminator::reserved;
    }
    return Nas::ProtocolDiscriminator::reserved;
}


Nas::SecurityHeaderType Nas::decodeSecurityHeaderType(const std::vector<uint8_t> &data)
{

    return Nas::uint8_t_to_Securityheadertype(data[1]);
}

Nas::Sparehalfoctet Nas::decodeSparehalfoctet(const std::vector<uint8_t> &data)
{
    return Nas::uint8_t_to_Sparehalfoctet(data[2]);
}

// sends invalid argument if no match
// FIXME What about reserved = 254 ??
Nas::ProtocolDiscriminator Nas::uint8_t_to_ProtocolDiscriminator(uint8_t byte)
{
    uint8_t pd = byte & 0x0f;
    // Extended Protocol discriminator
    if (pd == 0b1110)
    {
        switch (byte)
        {
        case static_cast<uint8_t>(Nas::ProtocolDiscriminator::_5GS_session_management_messages):
            return Nas::ProtocolDiscriminator::_5GS_session_management_messages;
        case static_cast<uint8_t>(Nas::ProtocolDiscriminator::_5GS_mobility_management_messages):
            return Nas::ProtocolDiscriminator::_5GS_mobility_management_messages;
        default:
            throw std::invalid_argument("Can't decode protocol discriminator");
        }
    }
    // Standard Protocol discriminator
    switch (pd)
    {
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::group_call_control):
        return Nas::ProtocolDiscriminator::group_call_control;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::broadcast_call_control):
        return Nas::ProtocolDiscriminator::broadcast_call_control;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::EPS_session_management_messages):
        return Nas::ProtocolDiscriminator::EPS_session_management_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::call_control_call_related_SS_messages):
        return Nas::ProtocolDiscriminator::call_control_call_related_SS_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::GPRS_Transparent_Transport_Protocol):
        return Nas::ProtocolDiscriminator::GPRS_Transparent_Transport_Protocol;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::mobility_management_messages):
        return Nas::ProtocolDiscriminator::mobility_management_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::radio_resources_management_messages):
        return Nas::ProtocolDiscriminator::radio_resources_management_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::EPS_mobility_management_messages):
        return Nas::ProtocolDiscriminator::EPS_mobility_management_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::GPRS_mobility_management_messages):
        return Nas::ProtocolDiscriminator::GPRS_mobility_management_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::SMS_messages):
        return Nas::ProtocolDiscriminator::SMS_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::GPRS_session_management_messages):
        return Nas::ProtocolDiscriminator::GPRS_session_management_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::non_call_related_SS_messages):
        return Nas::ProtocolDiscriminator::non_call_related_SS_messages;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::Location_services_specified_in_3GPP_TS_44_072__8a_):
        return Nas::ProtocolDiscriminator::Location_services_specified_in_3GPP_TS_44_072__8a_;
    case static_cast<uint8_t>(Nas::ProtocolDiscriminator::used_by_tests_procdures_described_in_3GPP_TS_44_014__5a__3GPP_TS_34_109__17a___3GPP_TS_36_509__26__and_3GPP_TS_38_509__29__):
        return Nas::ProtocolDiscriminator::used_by_tests_procdures_described_in_3GPP_TS_44_014__5a__3GPP_TS_34_109__17a___3GPP_TS_36_509__26__and_3GPP_TS_38_509__29__;
    
    }
    throw std::invalid_argument("Can't decode protocol discriminator");
}

Nas::SecurityHeaderType Nas::uint8_t_to_Securityheadertype(uint8_t byte)
{

    // Standard Protocol discriminator
    switch (byte)
    {
    case static_cast<uint8_t>(Nas::SecurityHeaderType::Plain_5GS_NAS_message__not_security_protected):
        return Nas::SecurityHeaderType::Plain_5GS_NAS_message__not_security_protected;
    case static_cast<uint8_t>(Nas::SecurityHeaderType::Integrity_protected_with_new_5G_NAS_security_context):
        return Nas::SecurityHeaderType::Integrity_protected_with_new_5G_NAS_security_context;
    case static_cast<uint8_t>(Nas::SecurityHeaderType::Integrity_protected_and_cyphered_with_new_5G_NAS_security_context):
        return Nas::SecurityHeaderType::Integrity_protected_and_cyphered_with_new_5G_NAS_security_context;
    case static_cast<uint8_t>(Nas::SecurityHeaderType::Integrity_protected_and_cyphered):
        return Nas::SecurityHeaderType::Integrity_protected_and_cyphered;
    case static_cast<uint8_t>(Nas::SecurityHeaderType::Integrity_protected):
        return Nas::SecurityHeaderType::Integrity_protected;
    }    
    throw std::invalid_argument("Can't decode security header type");
}


Nas::Sparehalfoctet Nas::uint8_t_to_Sparehalfoctet(uint8_t byte)
{

    // Standard Protocol discriminator
    switch (byte)
    {
    case static_cast<uint8_t>(Nas::Sparehalfoctet::spare):
        return Nas::Sparehalfoctet::spare;
    }    
    throw std::invalid_argument("Can't decode spare half octet");
}
