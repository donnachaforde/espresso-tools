#
# generic makefile 
#
# Usage make all | make clean
#



#---------------------------------------------------------------------------
# Declare vars

EXE = secs2time


CC		= gcc
CCFLAGS 	= -g
CCDEFINES 	= 
CCPATHS 	= -I../../../include -I../../../include/espresso

LD 		= gcc
LDPATHS		= -L.  -L../../../lib/espresso
LDLIBS		= -lstdc++ -lespresso
LDFLAGS		= ${LDPATHS} ${LDLIBS}



#---------------------------------------------------------------------------
# Build

OBJ = Main.o


all: ${OBJ}
	@echo Linking...
	${LD} -o ${EXE} ${LDPATHS} ${OBJ} ${LDLIBS} 




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

