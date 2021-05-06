#pragma once


#include <_5GS/ie/Security_header_type.h>
#include <_5GS/ie/Spare_half_octet.h>
#include <_5GS/5gs_mobility_mangement.h>
#include <_5GS/ie/Authentication_response_param.h>

namespace _5GS
{
class Authentication_response : public _5Gs_moblity_manage
{

public:
    // Mandatory
    IE::Auth_response_param m_Auth_resp_param;
 
    Authentication_response();
    // TODO doc
    Authentication_response(
        IE::Security_header_type sht,
        IE::Spare_half_Octet sho, 
        IE::Auth_response_param Auth_resp_param,
        int placeholder = 1 // remove me when the last optional ie is implemented
    );

    virtual int code(std::vector<uint8_t> &data) const;
    //virtual int decode(const std::vector<uint8_t> &data);
    virtual int decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);

    std::string to_string() const;

private:
    // Warning: Iei are local to message type 
    enum Iei {
        Auth_resp_param = 0x2D
    };
};
} // namespace _5GS