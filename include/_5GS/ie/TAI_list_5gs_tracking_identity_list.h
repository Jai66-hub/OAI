#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>
#include <_5GS/ie/Partial_tracking_area_identity.h>

namespace _5GS
{

namespace IE
{

class TAI_list_5gs_tracking_identity : public InformationElement
{

public:

    partial_tracking_area_identity m_chk;

    explicit TAI_list_5gs_tracking_identity() = default; 
    explicit TAI_list_5gs_tracking_identity(IE::partial_tracking_area_identity::PTAI_list m_partialtrkidentity);

    virtual std::string getName() const;
    virtual std::string valueToString() const;

    /*static std::string value_to_string(const Value value);*/

    int code_TLV(std::vector<uint8_t> &data,const uint8_t iei) const;
    //int decode_TLV(const std::vector<uint8_t> &data);   
    int decode_TLV(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);

private:
    

  //  Value uint8_t_to_Value(const uint8_t byte);
};

} // namespace IE

} // namespace _5GS
