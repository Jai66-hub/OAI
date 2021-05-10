#include <header.h>
#include <_5GS/Authentication_reject.h>
#include <buffers.h>

namespace _5GS
{

Authentication_reject::Authentication_reject()
{
    m_message_type.set(IE::Message_type::Value::Authentication_reject);
}

Authentication_reject::Authentication_reject(
     IE::Security_header_type sht,
    IE::Spare_half_Octet sho,
    int placeholder)
{
    m_message_type.set(IE::Message_type::Value::Authentication_reject);
}

int Authentication_reject::code(std::vector<uint8_t> &data) const
{
    int size = 0;

    try
    {
        /* setting the Extended protocol descriminiation as 5gs_mobility management messages */
        size += Nas::codeProtocolDiscriminator(data, Nas::ProtocolDiscriminator::_5GS_mobility_management_messages);
        // Adding the mandatory parameters header 
        size += codeMMHeader(data);
        // Adding the required IE's 
        /* EAP message need to be added *****/
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

int Authentication_reject::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    unsigned int offset = 0;
    std::vector<uint8_t> tmp = data;

    offset = _5Gs_moblity_manage::decode(data,out_data);
    
    /**** EAP message need to be added ****/

    return offset;
}

std::string Authentication_reject::to_string() const
{
    std::string str;
    str = "Authentication_reject";
    str += "(";
    // Header
    str += header_to_string();
    std::cout<<"header_to_string "<<std::endl;
    // Mandatory parameters
    std::cout<<"Auth failure finish "<<std::endl;
    // Optional parameters
    return str;
}

}; // namespace _5GS
