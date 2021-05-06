#include <_5GS/ie/TAI_list_5gs_tracking_identity_list.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{
// class  TAI_list_5gs_tracking_identity


TAI_list_5gs_tracking_identity::TAI_list_5gs_tracking_identity(IE::partial_tracking_area_identity::PTAI_list value) : m_chk(value)
{
     m_present = true;
}

int TAI_list_5gs_tracking_identity::code_TLV(std::vector<uint8_t> &data,const uint8_t iei) const
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
    data.push_back(7);
    size += 1;
    //size += m_fields.m_partialtrkidentity.code_V(data);
    size += m_chk.code_V(data);

    
    return size;
}

//int TAI_list_5gs_tracking_identity::decode_TLV(const std::vector<uint8_t> &data)
int TAI_list_5gs_tracking_identity::decode_TLV(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data)
{
    int size = 0;
    std::vector<uint8_t> tmp = data;

    if(data.size() <= 0)
    {
        std::cout<<"No data available"<<std::endl;
        return -1;
    }

   // IEI value 
    out_data.push_back(data[0]);

    size += 1;
    // Length value 
    out_data.push_back(data[1]);

    size += 1;

    //removing the IEI value and Length value from the array
    tmp.erase(tmp.begin(), tmp.begin()+ 2);

    size += m_chk.decode(tmp,out_data);

    return size;
}

std::string TAI_list_5gs_tracking_identity::getName() const
{
    return "TAI list identity";
}

std::string TAI_list_5gs_tracking_identity::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    return "TAI list identity(" +
           m_chk.valueToString() +
           ")";
}

} // namespace IE
} // namespace _5GS
