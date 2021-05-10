#pragma once


#include <_5GS/ie/Security_header_type.h>
#include <_5GS/ie/Spare_half_octet.h>
#include <_5GS/5gs_mobility_mangement.h>
#include <_5GS/ie/5gmm_cause.h>
#include <_5GS/ie/Auth_failure_param.h>

namespace _5GS
{
class Authentication_failure : public _5Gs_moblity_manage
{

public:
    // Mandatory
    IE::_5GMM_cause             m_obj5gmmcause;
    IE::Auth_failure_param      m_objAuthfailparam;
 
    Authentication_failure();
    // TODO doc
    Authentication_failure(
        IE::Security_header_type sht,
        IE::Spare_half_Octet sho,
        IE::_5GMM_cause _5gmm_cause_obj, 
        IE::Auth_failure_param  Authfailparam,
        int placeholder = 1 // remove me when the last optional ie is implemented
    );

    virtual int code(std::vector<uint8_t> &data) const;
    //virtual int decode(const std::vector<uint8_t> &data);
    virtual int decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);

    std::string to_string() const;

private:
    // Warning: Iei are local to message type 
    enum Iei {
        Auth_failure_param = 0x30
    };
};
} // namespace _5GS