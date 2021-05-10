#include <header.h>
#include <_5GS/Authentication_failure.h>
#include <buffers.h>

namespace _5GS
{

Authentication_failure::Authentication_failure()
{
    m_message_type.set(IE::Message_type::Value::Authentication_failure);
}

Authentication_failure::Authentication_failure(
     IE::Security_header_type sht,
    IE::Spare_half_Octet sho,
    IE::_5GMM_cause _5gmm_obj,
    IE::Auth_failure_param  Authfailparam,
    int placeholder)
    :m_objAuthfailparam(Authfailparam)
{
    m_message_type.set(IE::Message_type::Value::Authentication_failure);
}

int Authentication_failure::code(std::vector<uint8_t> &data) const
{
    int size = 0;

    try
    {
        /* setting the Extended protocol descriminiation as 5gs_mobility management messages */
        size += Nas::codeProtocolDiscriminator(data, Nas::ProtocolDiscriminator::_5GS_mobility_management_messages);
        // Adding the mandatory parameters header 
        size += codeMMHeader(data);
        // Adding the required IE's 
        size += m_obj5gmmcause.code(data, InformationElement::Format::V);
        size += m_objAuthfailparam.code(data, InformationElement::Format::TLV, static_cast<uint8_t>(Iei::Auth_failure_param));
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

int Authentication_failure::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    unsigned int offset = 0;
    std::vector<uint8_t> tmp = data;

    offset = _5Gs_moblity_manage::decode(data,out_data);

    tmp.erase(tmp.begin(), tmp.begin()+offset);
    
    out_data.push_back(m_obj5gmmcause.decode(tmp,InformationElement::Format::V));
    offset += 1;

    tmp.erase(tmp.begin(), tmp.begin()+offset);
    
    offset += m_objAuthfailparam.decode_TLV(tmp,out_data);    

    return offset;
}

std::string Authentication_failure::to_string() const
{
    std::string str;
    str = "Authentication_failure";
    str += "(";
    // Header
    str += header_to_string();
    std::cout<<"header_to_string "<<std::endl;
    // Mandatory parameters
    str += ", " + m_obj5gmmcause.to_string();
    str += ", " + m_objAuthfailparam.to_string();
    std::cout<<"Auth failure finish "<<std::endl;
    // Optional parameters
    return str;
}

}; // namespace _5GS
