//dong
#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>

namespace _5GS
{

namespace IE
{

class _5GS_mobile_identity : public InformationElement
{

public:
    // TS 24.501- 9.11.4.11
    enum class Identity : uint8_t
    {
        No_identity = 0,
        SUCI = 1,
        _5G_GUTI = 2,
        IMEI = 3,
        _5G_S_TMSI  = 4,
        IMEISV = 5,
        MAC_address = 6,
        EUI_64  = 7
    };

    _5GS_mobile_identity() = default;
    explicit _5GS_mobile_identity(const Identity Identity);

    void set(Identity id);
    Identity get() const;

    virtual std::string getName() const;
    virtual std::string IdentityToString() const;

    static std::string Identity_to_string(const Identity Identity);
    static Identity uint8_t_to_Identity(const uint8_t &byte);

    virtual uint8_t code_half_V() const;

protected:
    int code_V(std::vector<uint8_t> &data) const;
    int decode_V(const std::vector<uint8_t> &data); // FIXME;
    int decode_V(const uint8_t &data);
    int decode_LV_E(std::vector<uint8_t> &data);
    int decode_LV_E(const uint8_t &data);

private:
    Identity m_Identity;
};

} // namespace IE

} // namespace _5GS
