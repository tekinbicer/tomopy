# Compiler
CC = icc

# Directories
ROOTDIR = ..
# DISP Framework src dir
DISPDIR = ${ROOTDIR}/DISP/src/core/
# Bin directory
BINDIR = ${ROOTDIR}/bin/

# Flags
CFLAGS = -std=c++11 -fPIC -DAFFINITY_OFF -g -O3 -Wall -Wextra 

# Common data structures and utilities classes
COMMONDIR = ${ROOTDIR}/common

LIBS = -lm 
LIBDIRS = 
INCLUDES = -I$(DISPDIR) -I${COMMONDIR}

# Executable/reconstruction objects
MLEM_OBJS = mlem.o mlem_main.o mlem_wrapper.o
COMMON_OBJS = trace_utils.o

# Executables
SHSO = mlem.so

.PHONY: default clean

all: $(SHSO)

# MLEM
mlem.so: $(COMMON_OBJS) $(MLEM_OBJS)
	$(CC) -shared $(MLEM_OBJS) $(COMMON_OBJS) $(LIBDIRS) $(LIBS) -o mlem.so

mlem_wrapper.o: mlem_wrapper.cc mlem_wrapper.h
	$(CC) $(CFLAGS) -c mlem_wrapper.cc

mlem.o: mlem.cc mlem.h
	$(CC) $(CFLAGS) -c mlem.cc $(INCLUDES)

mlem_main.o: mlem_main.cc mlem_main.h
	$(CC) $(CFLAGS) -c mlem_main.cc $(INCLUDES)

trace_utils.o: $(COMMONDIR)/trace_utils.cc $(COMMONDIR)/trace_utils.h
	$(CC) $(CFLAGS) -c $(COMMONDIR)/trace_utils.cc $(INCLUDES)

clean:
	rm -f $(SHSO) *.o *.a *~ *.lst *.tmp .pure *.bak *.log
