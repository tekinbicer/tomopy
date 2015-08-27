CC = icc
# Top level Makefile
ROOTDIR = ../..
LIBDIR = ${ROOTDIR}/lib

# Object files
MLEMDIR = ./mlem
MLEM_OBJS = ${MLEMDIR}/mlem.o ${MLEMDIR}/mlem_main.o ${MLEMDIR}/mlem_wrapper.o
SIRTDIR = ./sirt
SIRT_OBJS = ${SIRTDIR}/sirt.o ${SIRTDIR}/sirt_main.o ${SIRTDIR}/sirt_wrapper.o
PMLDIR = ./pml
PML_OBJS = ${PMLDIR}/pml.o ${PMLDIR}/pml_main.o ${PMLDIR}/pml_wrapper.o

# Get trace util object from one of the compilation dirs
TRACE_UTILS_OBJ = ${SIRTDIR}/trace_utils.o

# Executables
TRACETOMO = tracetomo

.PHONY: default clean

all: $(TRACETOMO)

# MLEM
tracetomo:
	cd ${PMLDIR}; make
	cd ${SIRTDIR}; make
	cd ${MLEMDIR}; make
	${CC} -shared $(MLEM_OBJS) ${SIRT_OBJS} ${PML_OBJS} ${TRACE_UTILS_OBJ} -o libtracetomo.so

install:
	cp libtracetomo.so $(LIBDIR)

clean:
	cd ${PMLDIR}; make clean
	cd ${SIRTDIR}; make clean
	cd ${MLEMDIR}; make clean
	rm -f *.so *.o *.a *~ *.lst *.tmp .pure *.bak *.log
