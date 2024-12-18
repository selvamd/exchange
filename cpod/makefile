
.PHONY: all google_test exchange_code clean clean_exchange

all: google_test exchange_code
external: google_test 
#cover_all: google_test quick_fix urisyntax poco_libs openssl_lib pcap coverage_code
#cover: coverage_code

google_test:
	cd external_libs/googletest/gtest-1.7.0; ./configure; cd ./make; make -s

exchange_code:
	cd ./src; make -s

clean_exchange:
	cd ./src; make clean -s

clean:
	cd external_libs/googletest/gtest-1.7.0/make; make clean -s
	cd ./src; make clean -s
