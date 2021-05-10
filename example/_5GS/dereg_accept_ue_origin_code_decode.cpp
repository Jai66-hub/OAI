#include <cassert>
#include <gtest/gtest.h>
#include <_5GS/decode.h>
#include <buffers.h>
#include <_5GS/deregistration_accept_UE_origin.h>

using namespace _5GS;


class DecodeExample : public Decode
{
    

};

class Dereg_acpt_ue_org_test : public ::testing::Test{

public:
       IE::Security_header_type SHTobject;    
       IE::Spare_half_Octet SHOobject;    

       const std::vector<uint8_t> temp1 ={0,1,1};
       const std::vector<uint8_t> temp2;
       std::vector<uint8_t> data;

       Dereg_acpt_ue_org_test (){
         std::cout<<"IN constructor"<<std::endl;         
       }

       int retsize(IE::Auth_response_param in_Authparam)
       {

        Deregistration_acpt_UE_origin ARacp(
        IE::Security_header_type(IE::Security_header_type::Value::Integrty_protected_and_ciphered_new_5gNas_security),
        IE::Spare_half_Octet(IE::Spare_half_Octet::Value::spare));
        int size = ARacp.code(data);

       // std:: cout<<"size of code"<<size<<std::endl;
        return size;

       }

      virtual void SetUp() {
  //  std::cout<<"This is setup for debug"<<std::endl;    


  }

  virtual void TearDown() {
    //std::cout<<"This is teardown for debug"<<std::endl;
  }       

};

/* For security type decode */
TEST_F(Dereg_acpt_ue_org_test, testdecodesht_valid)
{
    ASSERT_NO_THROW(SHTobject.decode_V(temp2));
 // ASSERT_EXIT((SHTobject.decode_V(temp),exit(0)),::testing::KilledBySignal(SIGSEGV),".*");
}
/*  end *****/

/* For spare half octet */

TEST_F(Dereg_acpt_ue_org_test, testdecodesho_crct)
{     
    // for decode
    EXPECT_EQ(1,SHOobject.decode_V(temp1));        
}


TEST_F(Dereg_acpt_ue_org_test, testdecodesho_wrong)
{
  //for decode   
   EXPECT_NE(2,SHTobject.decode_V(temp2));
}

/*  end ******/


TEST_F(Dereg_acpt_ue_org_test, testdecodesho_crct)
{     
    // for decode
    EXPECT_EQ(1,SHOobject.decode_V(temp1));        
}


TEST_F(Dereg_acpt_ue_org_test, testdecodesho_wrong)
{
  //for decode   
   EXPECT_NE(2,SHTobject.decode_V(temp2));
}


int main(int argc, char **argv)
{
    std::vector<uint8_t> data;

    Deregistration_acpt_UE_origin ARacp(
        IE::Security_header_type(IE::Security_header_type::Value::Integrty_protected_and_ciphered_new_5gNas_security),
        IE::Spare_half_Octet(IE::Spare_half_Octet::Value::spare));

    int size = ARacp.code(data);

    // print info to terminal to stderr
    std::cerr << dump_wireshark(data);
    std::cerr << "size = " << size << " | size of buffer = " << data.size() << "\n";
    std::cerr << ARacp.to_string() << "\n";

    std::cout<<"End of encode"<<std::endl;

    std::cout<<"start of decode"<<std::endl;

    const std::vector<uint8_t> tmpdata = { 0x7e /* protocol disc-  Mobility mgmnt*/, 0x04 /*security header type*/ , 
    0x00 /* spare half octet */, 0x57  /* message type*/};
    std::vector<uint8_t> outdata;
    size = 0;

    DecodeExample decoder;
    size = decoder.decode(tmpdata,outdata);

    for(int i =0; i< outdata.size() ; i++)
    {
        std::cout<<" "<<unsigned(outdata[i]);
    }

    std::cout<<std::endl;


    /************************ run gtest ***************/


    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();   

    return 0;
}
