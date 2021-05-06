#include <_5GS/5gs_mobility_mangement.h>

namespace _5GS
{

_5Gs_moblity_manage::_5Gs_moblity_manage(IE::Security_header_type sht,
                   IE::Spare_half_Octet sho)
{
       m_Security_header_type = sht;
       m_Spare_half_octet = sho;
}

int _5Gs_moblity_manage::codeMMHeader(std::vector<uint8_t> &data) const
{
       int size = 0;
       size += m_Security_header_type.code(data, InformationElement::Format::V);
       size += m_Spare_half_octet.code(data, InformationElement::Format::V);
       size += m_message_type.code(data, InformationElement::Format::V);
       return size;
}

std::string _5Gs_moblity_manage::header_to_string() const
{
       std::string str;
       // TODO add security header

       // FIXME factorize
       str += m_Security_header_type.to_string() +
              ", ";

       str += m_Spare_half_octet.to_string() +
              ", ";
       str += m_message_type.to_string();

       return str;
}

//int _5Gs_moblity_manage::decode(const std::vector<uint8_t> &data)
int _5Gs_moblity_manage::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
       // TODO security header
       int offset = 1;
       int size = offset;


       if(data.size() <= 0)
       {
              std::cout<<"There is no data present "<<std::endl;
              return -1;
       }

       // XXX tmp: copy for easier writing or slice for performance ?
       std::vector<uint8_t> tmp = data;
       
       // FIXME factorize
       out_data.push_back(m_Security_header_type.decode(data,InformationElement::Format::V));
       offset += size;

       out_data.push_back(m_Spare_half_octet.decode(data,InformationElement::Format::V));
       offset += size;

       tmp.erase(tmp.begin(), tmp.begin() + offset);
       out_data.push_back(m_message_type.decode(tmp, InformationElement::Format::V));
       offset += size;

       return offset;
}

}; // namespace _5GS
