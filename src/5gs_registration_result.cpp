#include <_5GS/ie/5gs_registration_result.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{

_5GS_registration_result::_5GS_registration_result(_5GS_registration_result::_5g_reg_value regvalue,_5GS_registration_result::sms_allowed smsvalue, 
        _5GS_registration_result::NSSA_performed nssavalue,_5GS_registration_result::Emergency_registered emgvalue)
{

    m_SMS_value = 0;
    m_value = 0;
    m_NSSAA_value = 0;
    m_Emg_value = 0;

    set(regvalue,smsvalue,nssavalue,emgvalue);

 
}


std::string _5GS_registration_result::getName() const
{
    return "5G registrstion result";
}

void _5GS_registration_result::set(_5GS_registration_result::_5g_reg_value regvalue,_5GS_registration_result::sms_allowed smsvalue, 
        _5GS_registration_result::NSSA_performed nssavalue,_5GS_registration_result::Emergency_registered emgvalue)
{
    m_present = true;
    m_regvalue = regvalue;
    m_smsvalue  = smsvalue;
    m_nssavalue = nssavalue;
    m_emgvalue = emgvalue;

    /* added for debug */
    m_value = static_cast<uint8_t>(m_regvalue);
    m_SMS_value = static_cast<uint8_t>(m_smsvalue);
    m_NSSAA_value =  static_cast<uint8_t>(m_nssavalue);
    m_Emg_value = static_cast<uint8_t>(m_emgvalue);

}


int _5GS_registration_result::code_LV(std::vector<uint8_t> &data) const
{

    raise_exception_if_not_present(className(this));

    uint8_t regval =0, smsval =0, nssaval =0, emgval =0;

    int size = 0;
    int locdata = 0 ;

    // Adding the lenght to the encode 
    data.push_back(1);
    size += 1;

    regval = static_cast<uint8_t>(m_regvalue);
    smsval = static_cast<uint8_t>(m_smsvalue);
    nssaval = static_cast<uint8_t>(m_nssavalue);
    emgval = static_cast<uint8_t>(m_emgvalue);

    locdata = (( (emgval & 0x1) << 5 ) | ((nssaval & 0x1) << 4) | ((smsval & 0x1) << 3) | (regval));
    // FIXME add coders for simple type
    data.push_back(locdata);
    size += 1;

    return size;
}

int _5GS_registration_result::decode(const std::vector<uint8_t> &data,std::vector<uint8_t> &out_data)
{

    int size = 0 , outsize =0;
    int locdata = 0;
    size = decode_LV(data);
    outsize = out_data.size();
        
    // add length value
    out_data.push_back(1);

    locdata = (( (m_Emg_value & 0x1) << 5 ) | ((m_NSSAA_value & 0x1) << 4) | ((m_SMS_value & 0x1) << 3) | (m_value));

    out_data.push_back(locdata);
    
    return size;
}


int _5GS_registration_result::decode_LV(const std::vector<uint8_t> &data)
{
    int size = 0;
    // skip length
    m_value = (data[1] & 0x7);
    m_SMS_value = ((data[1] >> 3) & 0x1);
    m_NSSAA_value = ((data[1] >> 4) & 0x1);
    m_Emg_value = ((data[1] >> 5) & 0x1);

    size += 1;
     // check size
    if (size != data[0])
    {
        throw NasCodecException(std::string(__PRETTY_FUNCTION__) +
                                std::string(" Invalid length: found ") +
                                std::to_string(data[0]) +
                                std::string(" expected ") +
                                std::to_string(size));
    }

// Add lenght value 
    size += 1;

    return size;
}


std::string _5GS_registration_result::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    std::string str;
    str += "(";
    
    switch(m_value)
    {
        case 1:
            str += "3GPP access";
            break;
        case 2:
            str += "non-3GPP access";
            break;
        case 3:
            str += "3GPP access and non-3gpp access";
            break;
        case 7:
            str += "Reserved";
            break;
        default:
            str += "3GPP access";
            break;
    }
    str += ", " ;

    switch(m_SMS_value)
    {
        case 0:
            str += "SMS over nas not allowed";
            break;
        case 1:
            str += "SMS over nas allowed";
            break;
    }
    str += ", " ;

    switch(m_NSSAA_value)
    {
        case 0:
            str +=  "Network slice authentication authorization not performed";
            break;
        case 1:
            str += "Network slice authentication authorization is performed";
            break;
    }
    str += ", " ;

    switch(m_Emg_value)
    {
        case 0:
            str += "Not registered for emergency services";
            break;
        case 1:
            str += "registered for emergency services";
            break;
    }

    str += ")";

    return str;
}


}; // namespace IE
}; // namespace _5GS
