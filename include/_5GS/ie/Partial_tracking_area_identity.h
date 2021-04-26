#include <information_element.h>

namespace _5GS
{

namespace IE
{

class partial_tracking_area_identity : public InformationElement
{
public:
    // TS 24.501 - 9.11.4.7.2

    // partial tracking identity list value 
    typedef struct {

        uint8_t m_typeoflist; // 00 , 01 , 10
        uint8_t m_noofelements; // max 16 elements 
        uint8_t m_MCC1_MCC2[16];
        uint8_t m_MCC3_MNC3[16];
        uint8_t m_MNC1_MNC2[16];
        uint8_t TAC[16][3]; 
    
    } PTAI_list;


    explicit partial_tracking_area_identity() = default;
    explicit partial_tracking_area_identity(PTAI_list TAIvalue);
      
    void set(PTAI_list TAIvalue);
    PTAI_list get() const ;

    int code_V(std::vector<uint8_t> &data) const;
   // int decode_V(const std::vector<uint8_t> &data);
    int decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);

  //  static std::string value_to_string(const Value value);
    virtual std::string valueToString() const;
    virtual std::string getName() const;


public:
    PTAI_list m_PTAIvalue;
};

} // namespace IE

} // namespace _5GS