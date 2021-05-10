#include <_5GS/ie/5gmm_cause.h>
#include <buffers.h>

namespace _5GS
{
namespace IE
{

_5GMM_cause::_5GMM_cause(_5GMM_cause::Value value)
{
    set(value);
}

std::string _5GMM_cause::getName() const
{
    return "5GMM cause";
}

void _5GMM_cause::set(_5GMM_cause::Value id)
{
    m_present = true;
    m_value = id;
}

_5GMM_cause::Value _5GMM_cause::get() const
{
    raise_exception_if_not_present(className(this));
    return m_value;
}

int _5GMM_cause::code_V(std::vector<uint8_t> &data) const
{
    raise_exception_if_not_present(className(this));
    data.push_back(static_cast<uint8_t>(m_value));
    return 1;
}

int _5GMM_cause::decode_V(const std::vector<uint8_t> &data)
{
    if(data.size() < 2)
    {
        std::cout<<"No %GMM cause data present"<<std::endl;
        return -1; // error 
    }

    return decode_V(data[1]);
}

int _5GMM_cause::decode_V(const uint8_t &data)
{
    m_value = _5GMM_cause::uint8_t_to_Value(data);
    m_present = true;
 
    return static_cast<uint8_t>(m_value);
}

_5GMM_cause::Value _5GMM_cause::uint8_t_to_Value(const uint8_t &byte)
{
    return static_cast<_5GMM_cause::Value>(byte);
}

std::string _5GMM_cause::valueToString() const
{
    if (not isSet())
    {
        return "-";
    }
    return _5GMM_cause::value_to_string(m_value);
}

std::string _5GMM_cause::value_to_string(_5GMM_cause::Value value)
{
    switch (value)
    {

        case _5GMM_cause::Value::Illegal_UE_:															
        return "Illegal UE";											
        case _5GMM_cause::Value::PEI_not_accepted_:                                                     
        return "PEI not accepted";
        case _5GMM_cause::Value::Illegal_ME_:                                                           
        return "Illegal ME";
        case _5GMM_cause::Value::_5GS_services_not_allowed_:                                            
        return "5GS services not allowed";
        case _5GMM_cause::Value::UE_identity_cannot_be_derived_by_the_network_:                         
        return "UE identity cannot be derived by the network";
        case _5GMM_cause::Value::Implicitly_deregistered_:                                              
        return "Implicitly de-registered";
        case _5GMM_cause::Value::PLMN_not_allowed_:                                                     
        return "PLMN not allowed";
        case _5GMM_cause::Value::Tracking_area_not_allowed_:                                            
        return "Tracking area not allowed ";
        case _5GMM_cause::Value::Roaming_not_allowed_in_this_tracking_area_:                            
        return "Roaming not allowed in this tracking area ";
        case _5GMM_cause::Value::No_suitable_cells_in_tracking_area_:                                   
        return "No suitable cells in tracking area ";
        case _5GMM_cause::Value::MAC_failure_:                                                          
        return "MAC failure ";
        case _5GMM_cause::Value::Synch_failure_:                                                        
        return "Synch failure ";
        case _5GMM_cause::Value::Congestion_:                                                           
        return "Congestion ";
        case _5GMM_cause::Value::UE_security_capabilities_mismatch_:                                    
        return "UE security capabilities mismatch ";
        case _5GMM_cause::Value::Security_mode_rejected_unspecified_:                                   
        return "Security mode rejected, unspecified ";
        case _5GMM_cause::Value::Non_5G_authentication_unacceptable_:                                   
        return "Non-5G authentication unacceptable ";
        case _5GMM_cause::Value::N1_mode_not_allowed_:                                                  
        return "N1 mode not allowed ";
        case _5GMM_cause::Value::Restricted_service_area_:                                              
        return "Restricted service area"; 
        case _5GMM_cause::Value::Redirection_to_EPC_required_:                                          
        return "Redirection to EPC required"; 
        case _5GMM_cause::Value::LADN_not_available_:                                                   
        return "LADN not available ";
        case _5GMM_cause::Value::No_network_slices_available_:                                          
        return "No network slices available ";
        case _5GMM_cause::Value::Maximum_number_of_PDU_sessions_reached_:                               
        return "Maximum number of PDU sessions reached ";
        case _5GMM_cause::Value::Insufficient_resources_for_specific_slice_and_DNN_:                    
        return "Insufficient resources for specific slice and DNN ";
        case _5GMM_cause::Value::Insufficient_resources_for_specific_slice_:                            
        return "Insufficient resources for specific slice ";
        case _5GMM_cause::Value::ngKSI_already_in_use_:                                                 
        return "ngKSI already in use ";
        case _5GMM_cause::Value::Non_3GPP_access_to_5GCN_not_allowed_:                                 
        return "Non-3GPP access to 5GCN not allowed ";
        case _5GMM_cause::Value::Serving_network_not_authorized_:                                       
        return "Serving network not authorized ";
        case _5GMM_cause::Value::Temporarily_not_authorized_for_this_SNPN_:                             
        return "Temporarily not authorized for this SNPN ";
        case _5GMM_cause::Value::Permanently_not_authorized_for_this_SNPN_:                             
        return "Permanently not authorized for this SNPN ";
        case _5GMM_cause::Value::Not_authorized_for_this_CAG_or_authorized_for__CAG_cells_only_:        
        return "Not authorized for this CAG or authorized for CAG cells only";
        case _5GMM_cause::Value::Wireline_access_area_not_allowed_:                                     
        return "Wireline access area not allowed ";
        case _5GMM_cause::Value::Payload_was_not_forwarded_:                                            
        return "Payload was not forwarded ";
        case _5GMM_cause::Value::DNN_not_supported_or_not_subscribed_in_the_slice_:                     
        return "DNN not supported or not subscribed in the slice ";
        case _5GMM_cause::Value::Insufficient_user_plane_resources_for_the_PDU_session_:                
        return "Insufficient user-plane resources for the PDU session";
        case _5GMM_cause::Value::Semantically_incorrect_message_:                                       
        return "Semantically incorrect message"; 
        case _5GMM_cause::Value::Invalid_mandatory_information_:                                        
        return "Invalid mandatory information ";
        case _5GMM_cause::Value::Message_type_non_existent_or_not_implemented_:                         
        return "Message type non-existent or not implemented ";
        case _5GMM_cause::Value::Message_type_not_compatible_with_the_protocol_state_:                  
        return "Message type not compatible with the protocol state";
        case _5GMM_cause::Value::Information_element_non_existent_or_not_implemented_:                  
        return "Information element non-existent or not implemented";
        case _5GMM_cause::Value::Conditional_IE_error_:                                                 
        return "Conditional IE error ";
        case _5GMM_cause::Value::Message_not_compatible_with_the_protocol_state_:                       
        return "Message not compatible with the protocol state ";
        case _5GMM_cause::Value::Protocol_error_unspecified_:                                           
        return "Protocol error, unspecified ";

    // All other value are "Reserved"
    default:
        return "Reserved";
    }
    throw std::invalid_argument("Not a value");
}

} // namespace IE
} // namespace _5GS
