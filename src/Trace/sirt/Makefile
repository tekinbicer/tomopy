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
SIRT_OBJS = sirt.o sirt_main.o sirt_wrapper.o
COMMON_OBJS = trace_utils.o
SIRT_WRAPPER_OBJS = sirt_wrapper.o

# Executables
SHSO = sirt.so

.PHONY: default clean

all: $(SHSO) 

copy:
	cp sirt.so ../../../lib/libtracesirt.so

# SIRT
sirt.so: $(COMMON_OBJS) $(SIRT_OBJS)
	$(CC) -shared $(SIRT_OBJS) $(COMMON_OBJS) $(LIBDIRS) $(LIBS) -o sirt.so

sirt_wrapper.o: sirt_wrapper.cc sirt_wrapper.h
	$(CC) $(CFLAGS)  -c sirt_wrapper.cc

sirt.o: sirt.cc sirt.h
	$(CC) $(CFLAGS) -c sirt.cc $(INCLUDES)

sirt_main.o: sirt_main.cc
	$(CC) $(CFLAGS) -c sirt_main.cc $(INCLUDES)

trace_utils.o: $(COMMONDIR)/trace_utils.cc $(COMMONDIR)/trace_utils.h
	$(CC) $(CFLAGS) -c $(COMMONDIR)/trace_utils.cc $(INCLUDES)

clean:
	rm -f $(SHSO) *.o *.a *.so *.py* *~ *.lst *.tmp .pure *.bak *.log
