#include <header.h>
#include <_5GS/Authentication_response.h>
#include <buffers.h>

namespace _5GS
{

Authentication_response::Authentication_response()
{
    m_message_type.set(IE::Message_type::Value::Authentication_response);
}

Authentication_response::Authentication_response(
     IE::Security_header_type sht,
    IE::Spare_half_Octet sho,
    IE::Auth_response_param Auth_resp_param,
    int placeholder)
    // initializers
    :m_Auth_resp_param(Auth_resp_param)
{
    m_message_type.set(IE::Message_type::Value::Authentication_response);
}

int Authentication_response::code(std::vector<uint8_t> &data) const
{
    int size = 0;

    try
    {
        /* setting the Extended protocol descriminiation as 5gs_mobility management messages */
        size += Nas::codeProtocolDiscriminator(data, Nas::ProtocolDiscriminator::_5GS_mobility_management_messages);
        // Adding the mandatory parameters header 
        size += codeMMHeader(data);

       // std::cout<<data<<std::endl;
        // Adding the required IE's 
        size += m_Auth_resp_param.code(data, InformationElement::Format::TLV, static_cast<uint8_t>(Iei::Auth_resp_param));
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

int Authentication_response::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    unsigned int offset = 0;
    std::vector<uint8_t> tmp = data;

    offset = _5Gs_moblity_manage::decode(data,out_data);

    tmp.erase(tmp.begin(), tmp.begin()+offset);
    
    offset += m_Auth_resp_param.decode_TLV(tmp,out_data);
  
    return offset;
}

std::string Authentication_response::to_string() const
{
    std::string str;
    str = "Authentication_response";
    str += "(";
    // Header
    str += header_to_string();
    std::cout<<"header_to_string "<<std::endl;
    // Mandatory parameters
    str += ", " + m_Auth_resp_param.to_string();
    std::cout<<"Auth response finish "<<std::endl;
    // Optional parameters
    return str;
}

}; // namespace _5GS
