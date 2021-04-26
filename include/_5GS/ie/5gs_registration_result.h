#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>

namespace _5GS
{

namespace IE
{

class _5GS_registration_result : public InformationElement
{

public:

    enum class _5g_reg_value : uint8_t
    {
        _3gpp_access = 1,
        non_3gpp_access,
        _3gpp_access_and_non_3gpp_access,
        reserved = 7
    };

    enum class sms_allowed : uint8_t
    {
        sma_over_nas_not_allowed,
        sma_over_nas_allowed,
    };    

    enum class NSSA_performed : uint8_t
    {
        Network_slice_authentication_authorization_not_performed,
        Network_slice_authentication_authorization_is_performed,
    };       

    enum class Emergency_registered : uint8_t
    {
        Not_registered_for_emergency_services,
        registered_for_emergency_services,
    };       



    // length of 5gs result registration results
    uint8_t m_u85gslength;  //5g resigtration length value
    uint8_t m_value; //5g resigtration result value
    uint8_t m_SMS_value; //SMS allaowed value
    uint8_t m_NSSAA_value; //NSSAA performed value
    uint8_t m_Emg_value; //Emergency registered value


    _5GS_registration_result() = default;
    explicit _5GS_registration_result(const _5g_reg_value regvalue, const sms_allowed smsallow,const NSSA_performed nsasvalue, 
        const Emergency_registered emgvalue); 
        

    void set(const _5g_reg_value regvalue, const sms_allowed smsallow,const NSSA_performed nsasvalue, 
        const Emergency_registered emgvalue);

    virtual std::string getName() const;
    virtual std::string valueToString() const;

    /*static std::string value_to_string(const Value value);*/
    int decode(const std::vector<uint8_t> &data,std::vector<uint8_t> &out_data);

    int code_LV(std::vector<uint8_t> &data) const;
    int decode_LV(const std::vector<uint8_t> &data);   

private:
    _5g_reg_value           m_regvalue;
    sms_allowed             m_smsvalue;
    NSSA_performed          m_nssavalue;
    Emergency_registered    m_emgvalue;

  //  Value uint8_t_to_Value(const uint8_t byte);
};

} // namespace IE

} // namespace _5GS
