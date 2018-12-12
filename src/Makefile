COMPILE_OPTIONS = -I"../include" -pedantic -Wall -Wextra -Werror -O3 -std=c++11 -march=native
INCLUDE_DIR = ../include

FP_Test: FP_Test.o FP_Data.o
	g++ -o FP_Test FP_Test.o FP_Data.o

FP_Test.o: FP_Test.cc $(INCLUDE_DIR)/FP_Data.hh
	g++ -c $(COMPILE_OPTIONS) FP_Test.cc

FP_Data.o: FP_Data.cc $(INCLUDE_DIR)/FP_Data.hh
	g++ -c $(COMPILE_OPTIONS) FP_Data.cc

clean:
	rm -f FP_Test FP_Test.o FP_Data.o
