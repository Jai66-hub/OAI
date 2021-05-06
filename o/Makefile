SDIR	= ../src

all :
	$(CXX) -I ../include -fPIC -c $(SDIR)/*.cpp
	ar rcsD libnascodec.a *.o
#	cp libnascodec.a ../../../gtest/OAI/lib
	$(CXX) -I ../include -fPIC -c ../example/_5GS/Auth_response_code_decode.cpp
	cp Auth_response_code_decode.o ../example/_5GS/
	$(CXX) -o check ../example/_5GS/Auth_response_code_decode.o libnascodec.a

clean :
	rm *.o 
	rm libnascodec.a
	rm check
	rm ../example/_5GS/Auth_response_code_decode.o
