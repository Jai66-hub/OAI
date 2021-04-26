#include <cassert>

#include <buffers.h>
#include <_5GS/registration_accecpt.h>

using namespace _5GS;

int main()
{
    std::vector<uint8_t> data;
    IE::partial_tracking_area_identity TAIvalue;

    TAIvalue.m_PTAIvalue.m_typeoflist =  1;
    TAIvalue.m_PTAIvalue.m_noofelements = 1;
    TAIvalue.m_PTAIvalue.m_MCC1_MCC2[0] = 2;
    TAIvalue.m_PTAIvalue.m_MCC3_MNC3[0] = 3;
    TAIvalue.m_PTAIvalue.m_MNC1_MNC2[0] = 4;
    TAIvalue.m_PTAIvalue.TAC[0][0] = 6;
    TAIvalue.m_PTAIvalue.TAC[0][1] = 7;
    TAIvalue.m_PTAIvalue.TAC[0][2] = 8;

    Registration_accept Racp(
        IE::Security_header_type(IE::Security_header_type::Value::Integrty_protected_and_ciphered_new_5gNas_security),
        IE::Spare_half_Octet(IE::Spare_half_Octet::Value::spare),
        IE::_5GS_registration_result(IE::_5GS_registration_result::_5g_reg_value::non_3gpp_access,IE::_5GS_registration_result::sms_allowed::sma_over_nas_allowed,IE::_5GS_registration_result::NSSA_performed::Network_slice_authentication_authorization_is_performed,IE::_5GS_registration_result::Emergency_registered::Not_registered_for_emergency_services),
        IE::TAI_list_5gs_tracking_identity(TAIvalue.m_PTAIvalue));

    int size = Racp.code(data);

    // print info to terminal to stderr
    std::cerr << dump_wireshark(data);
    std::cerr << "size = " << size << " | size of buffer = " << data.size() << "\n";
    std::cerr << Racp.to_string() << "\n";

    // dump to stdout in order to redirect to a file and inspect with wireshark
    std::cout << dump_wireshark_with_ngap_encapsulation(data);

    const std::vector<uint8_t> result = {
        0x7e, 0x04, 0x00, 0x42, 0x01, 0x1a , 0x54, 0x07,
         0x21 , 0x2, 0x3, 0x4, 0x6, 0x7, 0x8};
    assert(data == result);

    std::cout<<"I am working in this"<<std::endl;

    return 0;
}
