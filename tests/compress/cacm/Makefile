
# Note: we use the compiler in lib and src, not bin

CYCDIR := ../../..
include $(CYCDIR)/Makefile.inc

CYCC := $(CYCDIR)/bin/$(CYCCOMP)

override CYC_FLAGS += -I$(CYCDIR)/lib -B$(CYCDIR)/lib

all: encode$(EXE) decode$(EXE)

encode$(EXE): bit_output.o arithmetic_encode.o adaptive_model.o encode.o
	$(CYCC) $(CYC_FLAGS) -g -o $@ $^

decode$(EXE): bit_input.o arithmetic_decode.o adaptive_model.o decode.o
	$(CYCC) $(CYC_FLAGS) -g -o $@ $^

%.o : %.cyc
	$(CYCC) $(CYC_FLAGS) -c $< 

clean:
	rm -f *.o *.c *.exe encode decode
