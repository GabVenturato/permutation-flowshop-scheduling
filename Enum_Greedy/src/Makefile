CXXFLAGS = -I$(INCLUDE_DIR) -pedantic -Wall -Wextra -Werror -O3 -std=c++11 -march=native
INCLUDE_DIR = ../include

FP_Test: FP_Test.o FP_Data.o
	$(CXX) -o $@ $^

FP_Test.o: FP_Test.cc $(INCLUDE_DIR)/FP_Data.hh
	$(CXX) -c $(CXXFLAGS) $<

FP_Data.o: FP_Data.cc $(INCLUDE_DIR)/FP_Data.hh
	$(CXX) -c $(CXXFLAGS) $<

clean:
	$(RM) FP_Test FP_Test.o FP_Data.o