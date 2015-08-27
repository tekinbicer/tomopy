# Compiler
CC = icc

# Directories
ROOTDIR = ..
# DISP Framework src dir
DISPDIR = ${ROOTDIR}/DISP
# Bin directory
BINDIR = ${ROOTDIR}/bin

# Flags
CFLAGS = -std=c++11 -fPIC -DAFFINITY_OFF -g -O3 -Wall -Wextra 

# Common data structures and utilities classes
COMMONDIR = ${ROOTDIR}/common

LIBS = -lm
LIBDIRS =
INCLUDES = -I$(DISPDIR) -I${COMMONDIR}

# Executable/reconstruction objects
PML_OBJS = pml.o pml_main.o pml_wrapper.o
COMMON_OBJS = trace_utils.o
PML_WRAPPER_OBJS = pml_wrapper.o

# Executables
SHSO = pml.so

.PHONY: default clean

all: $(SHSO)

# PML
pml.so: $(COMMON_OBJS) $(PML_OBJS)
	$(CC) -shared $(PML_OBJS) $(COMMON_OBJS) $(LIBDIRS) $(LIBS) -o pml.so

pml_wrapper.o: pml_wrapper.cc pml_wrapper.h
	$(CC) $(CFLAGS)  -c pml_wrapper.cc

pml.o: pml.cc pml.h
	$(CC) $(CFLAGS) -c pml.cc $(INCLUDES)

pml_main.o: pml_main.cc
	$(CC) $(CFLAGS) -c pml_main.cc $(INCLUDES)

trace_utils.o: $(COMMONDIR)/trace_utils.cc $(COMMONDIR)/trace_utils.h
	$(CC) $(CFLAGS) -c $(COMMONDIR)/trace_utils.cc $(INCLUDES)

clean:
	rm -f $(SHSO) *.o *.a *~ *.lst *.tmp .pure *.bak *.log
