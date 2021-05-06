#include <header.h>
#include <_5GS/registration_accecpt.h>
#include <buffers.h>

namespace _5GS
{

Registration_accept::Registration_accept()
{
    m_message_type.set(IE::Message_type::Value::Registration_accept);
}

Registration_accept::Registration_accept(
     IE::Security_header_type sht,
    IE::Spare_half_Octet sho,
    IE::_5GS_registration_result regresult,
    IE::TAI_list_5gs_tracking_identity TAI_listresult,
    int placeholder)
    // initializers
    : _5Gs_moblity_manage::_5Gs_moblity_manage(sht, sho),
      m_5gs_registrtn_result(regresult),m_TAI_list_identity(TAI_listresult)
{
    m_message_type.set(IE::Message_type::Value::Registration_accept);
}

int Registration_accept::code(std::vector<uint8_t> &data) const
{
    int size = 0;

    try
    {
        /* setting the Extended protocol descriminiation as 5gs_mobility management messages */
        size += Nas::codeProtocolDiscriminator(data, Nas::ProtocolDiscriminator::_5GS_mobility_management_messages);
        // Adding the mandatory parameters header 
        size += codeMMHeader(data);

        std::cout<<data<<std::endl;
        // Adding the required IE's 
        size += m_5gs_registrtn_result.code(data, InformationElement::Format::LV);
        size += m_TAI_list_identity.code(data, InformationElement::Format::TLV, static_cast<uint8_t>(Iei::TAI_list));
        // add optional parameters
    }
    catch (const std::exception &exception)
    {
        std::string test = std::string("Error coding ") + className(this);
        throw NasCodecException(
            stack_and_format_exception(
                exception.what(),
                std::string("Error coding ") + className(this)));
    }
    return size;
}

//int Registration_accept::decode(const std::vector<uint8_t> &data)
int Registration_accept::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    unsigned int offset = 0;
    std::vector<uint8_t> tmp = data;

    offset = _5Gs_moblity_manage::decode(data,out_data);

    tmp.erase(tmp.begin(), tmp.begin()+offset);
    
    offset += m_5gs_registrtn_result.decode(tmp,out_data);

    tmp.erase(tmp.begin(), tmp.begin()+2);
    
    offset += m_TAI_list_identity.decode_TLV(tmp,out_data);
  
    return offset;
}

std::string Registration_accept::to_string() const
{
    std::string str;
    str = "Registration_accept";
    str += "(";
    // Header
    str += header_to_string();
    // Mandatory parameters
    str += ", " + m_5gs_registrtn_result.to_string();
    str += ", " + m_TAI_list_identity.to_string();
    std::cout<<"TAI list finish "<<std::endl;
    // Optional parameters
    return str;
}

}; // namespace _5GS
