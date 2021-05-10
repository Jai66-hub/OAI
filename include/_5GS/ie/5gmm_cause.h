#pragma once

#include <iostream>
#include <vector>
#include <information_element.h>

namespace _5GS
{

namespace IE
{

class _5GMM_cause : public InformationElement
{
public:
    // TS 24.007 - 11.2.3.1b.1

    enum class Value : uint8_t
    {
            Illegal_UE_																=0b0000011,				
            PEI_not_accepted_                                                       =0b0000101,
            Illegal_ME_                                                             =0b0000110,
            _5GS_services_not_allowed_                                              =0b0000111,
            UE_identity_cannot_be_derived_by_the_network_                           =0b0001001,
            Implicitly_deregistered_                                                =0b0001010,
            PLMN_not_allowed_                                                       =0b0001011,
            Tracking_area_not_allowed_                                              =0b0001100,
            Roaming_not_allowed_in_this_tracking_area_                              =0b0001101,
            No_suitable_cells_in_tracking_area_                                     =0b0001111,
            MAC_failure_                                                            =0b0010100,
            Synch_failure_                                                          =0b0010101,
            Congestion_                                                             =0b0010110,
            UE_security_capabilities_mismatch_                                      =0b0010111,
            Security_mode_rejected_unspecified_                                     =0b0011000,
            Non_5G_authentication_unacceptable_                                     =0b0011010,
            N1_mode_not_allowed_                                                    =0b0011011,
            Restricted_service_area_                                                =0b0011100,
            Redirection_to_EPC_required_                                            =0b0011111,
            LADN_not_available_                                                     =0b0101011,
            No_network_slices_available_                                            =0b0111110,
            Maximum_number_of_PDU_sessions_reached_                                 =0b1000001,
            Insufficient_resources_for_specific_slice_and_DNN_                      =0b1000011,
            Insufficient_resources_for_specific_slice_                              =0b1000101,
            ngKSI_already_in_use_                                                   =0b1000111,
            Non_3GPP_access_to_5GCN_not_allowed_                                    =0b1001000,
            Serving_network_not_authorized_                                         =0b1001001,
            Temporarily_not_authorized_for_this_SNPN_                               =0b1001010,
            Permanently_not_authorized_for_this_SNPN_                               =0b1001011,
            Not_authorized_for_this_CAG_or_authorized_for__CAG_cells_only_          =0b1001100,
            Wireline_access_area_not_allowed_                                       =0b1001101,
            Payload_was_not_forwarded_                                              =0b1011010,
            DNN_not_supported_or_not_subscribed_in_the_slice_                       =0b1011011,
            Insufficient_user_plane_resources_for_the_PDU_session_                  =0b1011100,
            Semantically_incorrect_message_                                         =0b1011111,
            Invalid_mandatory_information_                                          =0b1100000,
            Message_type_non_existent_or_not_implemented_                           =0b1100001,
            Message_type_not_compatible_with_the_protocol_state_                    =0b1100010,
            Information_element_non_existent_or_not_implemented_                    =0b1100011,
            Conditional_IE_error_                                                   =0b1100100,
            Message_not_compatible_with_the_protocol_state_                         =0b1100101,
            Protocol_error_unspecified_                                             =0b1101111
    };

    _5GMM_cause() = default;
    explicit _5GMM_cause(const Value val);

    void set(const Value value);
    Value get() const;

    int code_V(std::vector<uint8_t> &data) const;
    int decode_V(const std::vector<uint8_t> &data);
    int decode_V(const uint8_t &data);

    virtual std::string getName() const;
    virtual std::string valueToString() const;

    static std::string value_to_string(const _5GMM_cause::Value value);
    static Value uint8_t_to_Value(const uint8_t &byte);

private:
    Value m_value;
};

} // namespace IE

} // namespace _5GS
