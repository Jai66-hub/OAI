#include <cassert>
#include <gtest/gtest.h>
#include <_5GS/decode.h>
#include <buffers.h>
#include <_5GS/Authentication_response.h>

using namespace _5GS;


class DecodeExample : public Decode
{
    

};

class Auth_resp_gtest : public ::testing::Test{

public:
       IE::Security_header_type SHTobject;    
       IE::Spare_half_Octet SHOobject;    
       IE::Auth_response_param AuthparamObj;       

       const std::vector<uint8_t> temp1 ={0,1,1};
       const std::vector<uint8_t> temp2;
       std::vector<uint8_t> data;

       Auth_resp_gtest (){
         std::cout<<"IN constructor"<<std::endl;         
       }

       int retsize(IE::Auth_response_param in_Authparam)
       {

        Authentication_response ARacp(
        IE::Security_header_type(IE::Security_header_type::Value::Integrty_protected_and_ciphered_new_5gNas_security),
        IE::Spare_half_Octet(IE::Spare_half_Octet::Value::spare),
        IE::Auth_response_param(in_Authparam.m_lvalue));

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



TEST_F(Auth_resp_gtest, test_regaccpt1)
{

    AuthparamObj.m_lvalue.m_u8length = 4;
    AuthparamObj.m_lvalue.m_u8Response[0] = 5;
    AuthparamObj.m_lvalue.m_u8Response[1] = 6;
    AuthparamObj.m_lvalue.m_u8Response[2] = 7;
    AuthparamObj.m_lvalue.m_u8Response[3] = 8;

 // for code
    EXPECT_EQ(15,retsize(AuthparamObj));     

}

TEST_F(Auth_resp_gtest, test_regaccpt2)
{

  AuthparamObj.m_lvalue.m_u8length = 4;
    AuthparamObj.m_lvalue.m_u8Response[0] = 5;
    AuthparamObj.m_lvalue.m_u8Response[1] = 6;
    AuthparamObj.m_lvalue.m_u8Response[2] = 7;
    AuthparamObj.m_lvalue.m_u8Response[3] = 8;
 

 // for code
    EXPECT_NE(15,retsize(AuthparamObj));     

}

/* For security type decode */


TEST_F(Auth_resp_gtest, testdecodesht_valid)
{
    ASSERT_NO_THROW(SHTobject.decode_V(temp2));
 // ASSERT_EXIT((SHTobject.decode_V(temp),exit(0)),::testing::KilledBySignal(SIGSEGV),".*");
}
/*  end *****/

/* For spare half octet */

TEST_F(Auth_resp_gtest, testdecodesho_crct)
{     
    // for decode
    EXPECT_EQ(1,SHOobject.decode_V(temp1));        
}


TEST_F(Auth_resp_gtest, testdecodesho_wrong)
{
  //for decode   
   EXPECT_NE(2,SHTobject.decode_V(temp2));
}

/*  end ******/


int main(int argc, char **argv)
{
    std::vector<uint8_t> data;
    IE::Auth_response_param Authresp;

    Authresp.m_lvalue.m_u8length = 4;
    Authresp.m_lvalue.m_u8Response[0] = 5;
    Authresp.m_lvalue.m_u8Response[1] = 6;
    Authresp.m_lvalue.m_u8Response[2] = 7;
    Authresp.m_lvalue.m_u8Response[3] = 8;


    Authentication_response ARacp(
        IE::Security_header_type(IE::Security_header_type::Value::Integrty_protected_and_ciphered_new_5gNas_security),
        IE::Spare_half_Octet(IE::Spare_half_Octet::Value::spare),
        IE::Auth_response_param(Authresp.m_lvalue));

    int size = ARacp.code(data);

    // print info to terminal to stderr
    std::cerr << dump_wireshark(data);
    std::cerr << "size = " << size << " | size of buffer = " << data.size() << "\n";
    std::cerr << ARacp.to_string() << "\n";

    std::cout<<"End of encode"<<std::endl;

    std::cout<<"start of decode"<<std::endl;

    const std::vector<uint8_t> tmpdata = { 0x7e /* protocol disc-  Mobility mgmnt*/, 0x04 /*security header type*/ , 
    0x00 /* spare half octet */, 0x57 , /* message type*/0x2d,0x4, 0x8, 9, 10 , 11};
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
