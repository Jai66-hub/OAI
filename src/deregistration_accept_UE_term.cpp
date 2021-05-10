#include <header.h>
#include <_5GS/deregistration_accept_UE_term.h>
#include <buffers.h>

namespace _5GS
{

Deregistration_acpt_UE_term::Deregistration_acpt_UE_term()
{
    m_message_type.set(IE::Message_type::Value::Deregistration_accept__UE_terminated_);
}

Deregistration_acpt_UE_term::Deregistration_acpt_UE_term(
     IE::Security_header_type sht,
    IE::Spare_half_Octet sho,
    int placeholder)
{
    m_message_type.set(IE::Message_type::Value::Deregistration_accept__UE_terminated_);
}

int Deregistration_acpt_UE_term::code(std::vector<uint8_t> &data) const
{
    int size = 0;

    try
    {
        /* setting the Extended protocol descriminiation as 5gs_mobility management messages */
        size += Nas::codeProtocolDiscriminator(data, Nas::ProtocolDiscriminator::_5GS_mobility_management_messages);
        // Adding the mandatory parameters header 
        size += codeMMHeader(data);
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

int Deregistration_acpt_UE_term::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    unsigned int offset = 0;
    std::vector<uint8_t> tmp = data;

    offset = _5Gs_moblity_manage::decode(data,out_data);
    
    return offset;
}

std::string Deregistration_acpt_UE_term::to_string() const
{
    std::string str;
    str = "Deregistration_acpt_UE_term";
    str += "(";
    // Header
    str += header_to_string();
    std::cout<<"header_to_string "<<std::endl;
    // Mandatory parameters
    std::cout<<"De-registration-accept UE term finish "<<std::endl;
    // Optional parameters
    return str;
}

}; // namespace _5GS
