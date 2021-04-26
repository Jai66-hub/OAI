#include <vector>

#include <_5GS/decode.h>
#include <_5GS/registration_accecpt.h>

using namespace _5GS;

class DecodeExample : public Decode
{
   /* void onPduSessionEstablishmentRequest(const PDU_session_establishment_request &pdu)
    {
        std::cout << pdu.to_string();
    }*/

};

int main()
{
   // const std::vector<uint8_t> data = { 0x7e, 0x03, 0x42, 0xc1, 0x02, 0xff, 0x93 };

    const std::vector<uint8_t> data = { 0x7e /* protocol disc-  Mobility mgmnt*/, 0x04 /*security header type*/ , 
    0x00 /* spare half octet */, 0x42 /* message type*/,0x1, 0x1a, 0x5, 0x7, 0x21, 0x8, 0x9, 0x10 , 0x11, 0x12, 0x13};
    std::vector<uint8_t> outdata;
    int size;

    DecodeExample decoder;
    size = decoder.decode(data,outdata);

    for(int i =0; i< outdata.size() ; i++)
    {
        std::cout<<" "<<unsigned(outdata[i]);
    }

    std::cout<<std::endl;


    std::cout << "Decoded " << size << " bytes." << std::endl;
    return 0;
}