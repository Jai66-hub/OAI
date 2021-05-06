#include <_5GS/ie/EAP_message.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{
// class  Auth_response_param


Auth_response_param::Auth_response_param(m_sLen_Value  in_msParam)
{
     m_present = true;
     Set(in_msParam);
}

void Auth_response_param::Set(m_sLen_Value  in_msParam)
{
    m_lvalue = in_msParam;
}

int Auth_response_param::code_TLV(std::vector<uint8_t> &data,const uint8_t iei) const
{
    raise_exception_if_not_present(className(this));
    int size = 0;

    if(iei <=0 )
    {
        std::cout<<"Iei should not be zero"<<std::endl;
        return -1;
    }

    data.push_back(static_cast<uint8_t>(iei));
    size += 1;
    data.push_back(m_lvalue.m_u8length);
    size += 1;

    for(int i =0; i <  m_lvalue.m_u8length ; i++)
    {
        data.push_back(m_lvalue.m_u8Response[i]);
        size += 1;
    }
    
    return size;
}

int Auth_response_param::decode_TLV(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    int size = 0;
    int ieival = 0;

    if(data.size() <= 0)
    {
        std::cout<<"No data available"<<std::endl;
        return -1;
    }

    ieival = data[0];

    if(ieival <= 0)
    {
        std::cout<<"Inavlid iei value"<<std::endl;
        return -1;
    }

   // IEI value 
    out_data.push_back(data[0]);
    size += 1;

   // lenght
    m_lvalue.m_u8length = data[1];

    if(m_lvalue.m_u8length < 4)
    {
        std::cout<<"Inavlid length value"<<std::endl;
        return -1;        
    }

    out_data.push_back(data[1]);
    size += 1;

    //value

    for(int i = 0 ; i< m_lvalue.m_u8length ; i++)
    {
        m_lvalue.m_u8Response[i] = data[i+2]; 
        out_data.push_back(data[i+2]);
        size += 1;
    }
    
    size += 1;

    return size;
}

std::string Auth_response_param::getName() const
{
    return "Authentication response parameter";
}

std::string Auth_response_param::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    return "Authentication response parameter";
}

} // namespace IE
} // namespace _5GS
