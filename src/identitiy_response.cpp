#include <header.h>
#include <_5GS/identitiy_response.h>
#include <buffers.h>

namespace _5GS
{

Identity_response::Identity_response()
{
    m_message_type.set(IE::Message_type::Value::Identity_response);
}

Identity_response::Identity_response(
     IE::Security_header_type sht,
    IE::Spare_half_Octet sho,
    int placeholder)
{
    m_message_type.set(IE::Message_type::Value::Identity_response);
}

int Identity_response::code(std::vector<uint8_t> &data) const
{
    int size = 0;

    try
    {
        /* setting the Extended protocol descriminiation as 5gs_mobility management messages */
        size += Nas::codeProtocolDiscriminator(data, Nas::ProtocolDiscriminator::_5GS_mobility_management_messages);
        // Adding the mandatory parameters header 
        size += codeMMHeader(data);

        /**** Add IE for the 5gs mobile identity *****/
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

int Identity_response::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    unsigned int offset = 0;
    std::vector<uint8_t> tmp = data;

    offset = _5Gs_moblity_manage::decode(data,out_data);

    /****** 5gs mobile identiy decode need to be added ****/

    return offset;
}

std::string Identity_response::to_string() const
{
    std::string str;
    str = "Identity_response";
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
