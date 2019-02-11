EASYLOCAL = ../easylocal-3
INCLUDE_DIR = ../include
FLAGS = -std=c++14 -Wall -march=native -O3 # -g -Wextra -Werror -pedantic
COMPOPTS = -I$(EASYLOCAL)/include -I$(INCLUDE_DIR) $(FLAGS)
LINKOPTS = -lboost_program_options -pthread

OBJECT_FILES = FP_Data.o FP_Basics.o FP_Helpers.o FP_Main.o

FP: $(OBJECT_FILES)
	$(CXX) $^ $(LINKOPTS) -o $@

FP_Data.o: FP_Data.cc $(INCLUDE_DIR)/FP_Data.hh
	$(CXX) -c $(COMPOPTS) $<

FP_Basics.o: FP_Basics.cc $(INCLUDE_DIR)/FP_Basics.hh $(INCLUDE_DIR)/FP_Data.hh
	$(CXX) -c $(COMPOPTS) $<

FP_Helpers.o: FP_Helpers.cc $(INCLUDE_DIR)/FP_Helpers.hh $(INCLUDE_DIR)/FP_Basics.hh $(INCLUDE_DIR)/FP_Data.hh
	$(CXX) -c $(COMPOPTS) $<

FP_Main.o: FP_Main.cc $(INCLUDE_DIR)/FP_Helpers.hh $(INCLUDE_DIR)/FP_Basics.hh $(INCLUDE_DIR)/FP_Data.hh
	$(CXX) -c $(COMPOPTS) $<

clean:
	$(RM) -f $(OBJECT_FILES) FP