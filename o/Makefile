SDIR	= ../src

all :
	$(CXX) -I ../include -fPIC -c $(SDIR)/*.cpp
	ar rcsD libnascodec.a *.o
	$(CXX) -I ../include -fPIC -c ../example/_5GS/Registration_accept_decode.cpp
	cp Registration_accept_decode.o ../example/_5GS/
	$(CXX) -o check ../example/_5GS/Registration_accept_decode.o libnascodec.a

clean :
	rm *.o 
	rm libnascodec.a
	rm check
	rm ../example/_5GS/Registration_accept_decode.o
