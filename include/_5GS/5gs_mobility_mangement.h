#pragma once

#include <_5GS/pdu_5gs.h>

#include <_5GS/ie/Security_header_type.h>
#include <_5GS/ie/Spare_half_octet.h>

namespace _5GS
{
class _5Gs_moblity_manage : public Pdu5gs
{

public:
    // for MM messages
    // TODO private or protected IEs ?
    IE::Security_header_type m_Security_header_type;
    IE::Spare_half_Octet m_Spare_half_octet;

    _5Gs_moblity_manage(const IE::Security_header_type sht = IE::Security_header_type(IE::Security_header_type::Value::Plain_5gs_NAS_msg_not_security_protected),
             const IE::Spare_half_Octet sho = IE::Spare_half_Octet(IE::Spare_half_Octet::Value::spare));

   /* virtual int decode(const std::vector<uint8_t> &data);*/
    virtual int decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);

    int codeMMHeader(std::vector<uint8_t> &data) const;

    virtual std::string header_to_string() const;
};

} // namespace _5GS
