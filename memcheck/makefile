#
# generic makefile 
#
# Usage make all | make clean
#



#---------------------------------------------------------------------------
# Declare vars

EXE = memcheck


CC		= gcc
CCFLAGS 	= -g
CCDEFINES 	= 
CCPATHS 	= -I../../../include -I../../../include/espresso

LD 		= gcc
LDPATHS		= -L.  -L../../../lib/espresso 
LDLIBS		= -lstdc++ -lespresso
LDFLAGS		= ${LDPATHS} ${LDLIBS} 

# AIX Linker settings - maxdata & maxstack affect the max memory a process can use
#
ifeq ($(OS_TYPE), AIX) 
LDFLAGS += -Wl,-bmaxdata:0x80000000 -Wl,-bmaxstack:0x20000000
endif




#---------------------------------------------------------------------------
# Build

OBJ = Main.o


all: ${OBJ}
	@echo Linking...
	${LD} -o ${EXE} ${LDPATHS} ${OBJ} ${LDLIBS} 

# enable high memory usage on HP-UX
ifeq ($(OS_TYPE), HPUX) 
	/usr/bin/chatr +q3p enable ${EXE}
endif





#---------------------------------------------------------------------------
# Clean

clean: 
	@echo Cleaning... 
	rm -f *.o *.a core
	rm -f ${EXE}
	@echo Done! 





#---------------------------------------------------------------------------
# Build Instructions

.SUFFIXES: .C 

.C.o:
	@echo Compiling '$<'...
	$(CC) -c $< $(CCFLAGS) $(CCDEFINES) $(CCPATHS)

.SUFFIXES: .cxx
.cxx.o:
	@echo Compiling '$<'...
	$(CC) -c $< $(CCFLAGS) $(CCDEFINES) $(CCPATHS)

