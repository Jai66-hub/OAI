#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>

namespace _5GS
{

namespace IE
{

class Auth_failure_param : public InformationElement
{

public:


    typedef struct{
        uint8_t    m_u8length; 
        uint8_t    m_u8AUTS[14]; 
    }m_sLen_Value;


    explicit Auth_failure_param() = default; 
    explicit Auth_failure_param(m_sLen_Value m_lvalue);

    virtual std::string getName() const;
    virtual std::string valueToString() const;

    void Set(m_sLen_Value  in_msParam);

    /*static std::string value_to_string(const Value value);*/

    int code_TLV(std::vector<uint8_t> &data,const uint8_t iei) const;
    //int decode_TLV(const std::vector<uint8_t> &data);   
    int decode_TLV(const std::vector<uint8_t> &data, std::vector<uint8_t> &out_data);


    m_sLen_Value      m_lvalue;

private:
    

  //  Value uint8_t_to_Value(const uint8_t byte);
};

} // namespace IE

} // namespace _5GS
