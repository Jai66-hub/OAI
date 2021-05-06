#include <_5GS/ie/Partial_tracking_area_identity.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{

partial_tracking_area_identity::partial_tracking_area_identity(PTAI_list TAIvalue)
{
    m_present = true;
    set(TAIvalue);
}

std::string partial_tracking_area_identity::getName() const
{
    return "Partial tracking area identity list";
}

void partial_tracking_area_identity::set(partial_tracking_area_identity::PTAI_list PTAIvalue)
{
    m_PTAIvalue = PTAIvalue;
}

partial_tracking_area_identity::PTAI_list partial_tracking_area_identity::get() const
{
    raise_exception_if_not_present(className(this));
    return m_PTAIvalue;
}

int partial_tracking_area_identity::code_V(std::vector<uint8_t> &data) const
{
    raise_exception_if_not_present(className(this));

    int size = 0 ;
    /* adding up the IE to the element */

    data.push_back(static_cast<uint8_t>(((m_PTAIvalue.m_typeoflist & 0x3 ) << 5) | (m_PTAIvalue.m_noofelements & 0x1F)));
    size += 1;

    switch(m_PTAIvalue.m_typeoflist)
    {
        case 0:

            data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MCC1_MCC2[0]));
            size += 1;
            data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MCC3_MNC3[0]));
            size += 1;
            data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MNC1_MNC2[0]));
            size += 1;        
            
            for(uint16_t uloop =0 ; uloop < m_PTAIvalue.m_noofelements; uloop++)
            {
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[uloop][0]));
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[uloop][1]));
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[uloop][2]));
                size += 3;
            }

        break;
        
        case 1:

            data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MCC1_MCC2[0]));
            size += 1;
            data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MCC3_MNC3[0]));
            size += 1;
            data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MNC1_MNC2[0]));
            size += 1;  

            data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[0][0]));
            data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[0][1]));
            data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[0][2]));
            size += 3;

        break;

        case 2:

            for(uint16_t uloop =0 ; uloop < m_PTAIvalue.m_noofelements; uloop++)
            {

                data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MCC1_MCC2[uloop]));
                size += 1;
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MCC3_MNC3[uloop]));
                size += 1;
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.m_MNC1_MNC2[uloop]));
                size += 1;                

                data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[uloop][0]));
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[uloop][1]));
                data.push_back(static_cast<uint8_t>(m_PTAIvalue.TAC[uloop][2]));
                size += 3;
              
            }

        break;

        default:
            std::cout<<"invalid type value"<<std::endl;
        break;

    }      

    return size;
}


int partial_tracking_area_identity::decode(const std::vector<uint8_t> &data,std::vector<uint8_t> &out_data)
{
    int size = 0 ;
    int locdata = 0;

    if(data.size() <= 0)
    {
        std::cout<<"No data is present"<<std::endl;
        return -1;
    }

    m_PTAIvalue.m_typeoflist = ((data[size] >> 5 ) & 0x3 );
    m_PTAIvalue.m_noofelements = ((data[size]) & 0x1F);

    locdata = ((m_PTAIvalue.m_typeoflist << 5) | m_PTAIvalue.m_noofelements);
    
    out_data.push_back(locdata);
    size += 1;

    switch(m_PTAIvalue.m_typeoflist)
    {
        case 0:

            m_PTAIvalue.m_MCC1_MCC2[0] = data[size];
            out_data.push_back(m_PTAIvalue.m_MCC1_MCC2[0]);
            size += 1;
            m_PTAIvalue.m_MCC3_MNC3[0] = data[size];
            out_data.push_back(m_PTAIvalue.m_MCC3_MNC3[0]);
            size += 1;
            m_PTAIvalue.m_MNC1_MNC2[0] = data[size];
            out_data.push_back(m_PTAIvalue.m_MNC1_MNC2[0]);
            size += 1;        
            
            for(uint16_t uloop =0 ; uloop < m_PTAIvalue.m_noofelements; uloop++)
            {
                m_PTAIvalue.TAC[uloop][0] = data[size];
                out_data.push_back(m_PTAIvalue.TAC[uloop][0]);
                m_PTAIvalue.TAC[uloop][1] = data[size+1];
                out_data.push_back(m_PTAIvalue.TAC[uloop][1]);
                m_PTAIvalue.TAC[uloop][2] = data[size+2];
                out_data.push_back(m_PTAIvalue.TAC[uloop][2]);
                size += 3;
            }

        break;
        
        case 1:
 
            m_PTAIvalue.m_MCC1_MCC2[0] = data[size];
            out_data.push_back(m_PTAIvalue.m_MCC1_MCC2[0]);
            size += 1;
            m_PTAIvalue.m_MCC3_MNC3[0] = data[size];
            out_data.push_back(m_PTAIvalue.m_MCC3_MNC3[0]);
            size += 1;
            m_PTAIvalue.m_MNC1_MNC2[0] = data[size];
            out_data.push_back(m_PTAIvalue.m_MNC1_MNC2[0]);
            size += 1;

            m_PTAIvalue.TAC[0][0] = data[size];
            out_data.push_back(m_PTAIvalue.TAC[0][0]);
            m_PTAIvalue.TAC[0][1] = data[size+1];
            out_data.push_back(m_PTAIvalue.TAC[0][1]);
            m_PTAIvalue.TAC[0][2] = data[size+2];
            out_data.push_back(m_PTAIvalue.TAC[0][2]);
            size += 3;

        break;

        case 2:

            for(uint16_t uloop =0 ; uloop < m_PTAIvalue.m_noofelements; uloop++)
            {
                m_PTAIvalue.m_MCC1_MCC2[uloop] = data[size];
                out_data.push_back(m_PTAIvalue.m_MCC1_MCC2[uloop]);
                size += 1;
                m_PTAIvalue.m_MCC3_MNC3[uloop] = data[size];
                 out_data.push_back(m_PTAIvalue.m_MCC3_MNC3[uloop]);
                size += 1;
                m_PTAIvalue.m_MNC1_MNC2[uloop] = data[size];
                out_data.push_back(m_PTAIvalue.m_MNC1_MNC2[uloop]);
                size += 1;                 

                m_PTAIvalue.TAC[uloop][0] = data[size];
                out_data.push_back(m_PTAIvalue.TAC[uloop][0]);
                m_PTAIvalue.TAC[uloop][1] = data[size+1];
                out_data.push_back(m_PTAIvalue.TAC[uloop][1]);
                m_PTAIvalue.TAC[uloop][2] = data[size+2];
                out_data.push_back(m_PTAIvalue.TAC[uloop][2]);
                size += 3;              
            }

        break;

        default:
            std::cout<<"invalid type value read"<<std::endl;
        break;

    }  

    m_present = true;
    return size;
}


std::string partial_tracking_area_identity::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    /*return partial_tracking_area_identity::value_to_string();*/
    return "Partial tracking identiy list";
}


} // namespace IE
} // namespace _5GS
