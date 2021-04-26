#pragma once


#include <_5GS/ie/Security_header_type.h>
#include <_5GS/ie/Spare_half_octet.h>
#include <_5GS/5gs_mobility_mangement.h>
#include <_5GS/ie/5gs_registration_result.h>
#include <_5GS/ie/TAI_list_5gs_tracking_identity_list.h>

namespace _5GS
{
class Registration_accept : public _5Gs_moblity_manage
{

public:
    // Mandatory
    IE::_5GS_registration_result m_5gs_registrtn_result;
    IE::TAI_list_5gs_tracking_identity m_TAI_list_identity;
 
    Registration_accept();
    // TODO doc
    Registration_accept(
        IE::Security_header_type sht,
        IE::Spare_half_Octet sho, 
        IE::_5GS_registration_result regresult,
        IE::TAI_list_5gs_tracking_identity TAI_listresult,
       // IE::partial_tracking_area_identity::PTAI_list PTAI_listresult,
        int placeholder = 1 // remove me when the last optional ie is implemented
    );

    virtual int code(std::vector<uint8_t> &data) const;
    //virtual int decode(const std::vector<uint8_t> &data);
    virtual int decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);

    std::string to_string() const;

private:
    // Warning: Iei are local to message type 
    enum Iei {
        TAI_list = 0x54
    };
};
} // namespace _5GS