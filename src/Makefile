CC		= g++

# Use this one to get Windows multi-threading
CC_FLAGS	= -O3 -flto -mtune=generic -fno-use-linker-plugin
# Use this one to get OpenMP multi-threading
# CC_FLAGS	= -O3 -flto -fopenmp -mtune=generic -fno-use-linker-plugin

LD_FLAGS	= 		\
	-Wl,--subsystem,windows \
	-Wl,--output-def,$(DLLBASE).def	\
	-Wl,--dynamicbase 	\
	-Wl,--nxcompat 		\
	-Wl,--no-seh 		\
	-Wl,--enable-stdcall-fixup

# These flags are not turned on be default, but DDS should pass them.
# Turn them on below.
WARN_FLAGS	= 		\
	-Wshadow 		\
	-Wsign-conversion 	\
	-pedantic -Wall -Wextra  \
	-Wcast-align -Wcast-qual \
	-Wctor-dtor-privacy 	\
	-Wdisabled-optimization \
	-Wformat=2 		\
	-Winit-self 		\
	-Wlogical-op 		\
	-Wmissing-declarations 	\
	-Wmissing-include-dirs 	\
	-Wnoexcept 		\
	-Wold-style-cast 	\
	-Woverloaded-virtual 	\
	-Wredundant-decls 	\
	-Wsign-promo 		\
	-Wstrict-null-sentinel 	\
	-Wstrict-overflow=1 	\
	-Wswitch-default -Wundef \
	-Werror 		\
	-Wno-unused 		\
	-Wno-unknown-pragmas 	\
	-Wno-long-long

# Here you can turn on warnings.
# CC_FULL_FLAGS	= $(CC_FLAGS)
CC_FULL_FLAGS	= $(CC_FLAGS) $(WARN_FLAGS)

LIB_FLAGS	= -shared

WINDRES_FLAG	= 

SOURCE_FILES 	=		\
        stats.cpp               \
        DefList.cpp             \
        MoveList.cpp            \
        SideMoveList.cpp        \
        Trick.cpp               \
        Segment.cpp             \
        TrickList.cpp           \
        AltList.cpp             \
        AltMatrix1D.cpp         \
        AltMatrix2D.cpp         \
        Holding.cpp             \
        LoopHold.cpp            \
        SimpleMoves.cpp         \
        ComplexMoves.cpp        \
        Header.cpp              \
        Hash.cpp                \
        Twiddle.cpp             \
        Dist.cpp                \
        sort.cpp                \
        files.cpp               \
        check.cpp               \
        hist.cpp                \
        options.cpp             \
        summary.cpp             \
        single.cpp

O_FILES 	= $(subst .cpp,.o,$(SOURCE_FILES))

ifeq ($(DDS_THREADS),none)
DDS_THR		= -DDDS_THREADS_SINGLE
else
DDS_THR		= 
endif


cygwin:	$(O_FILES)
	$(CC) $(CC_FULL_FLAGS) $(O_FILES) $(LD_FLAGS) -o single.exe

%.o:	%.cpp
	$(CC) $(CC_FULL_FLAGS) $(DDS_THR) -c $<

depend:
	makedepend -Y -- $(CC_FLAGS) -- $(SOURCE_FILES)

clean:
	rm -f $(O_FILES) $(DLL) single.{lib,def,exp,res}

# DO NOT DELETE

stats.o: portab.h stats.h
DefList.o: Trick.h compare.h const.h Header.h DefList.h AltList.h TrickList.h
DefList.o: Segment.h AltMatrix2D.h files.h options.h
MoveList.o: Header.h Trick.h compare.h const.h MoveList.h SideMoveList.h
MoveList.o: Holding.h DefList.h AltList.h TrickList.h Segment.h Hash.h
MoveList.o: summary.h sort.h files.h
SideMoveList.o: Header.h Trick.h compare.h const.h SideMoveList.h Holding.h
SideMoveList.o: DefList.h AltList.h TrickList.h Segment.h Hash.h summary.h
SideMoveList.o: sort.h portab.h
Trick.o: Trick.h compare.h const.h Holding.h
Segment.o: Trick.h compare.h const.h Segment.h Holding.h
TrickList.o: Holding.h Trick.h compare.h const.h TrickList.h Segment.h
TrickList.o: Header.h
AltList.o: Trick.h compare.h const.h AltList.h TrickList.h Segment.h
AltList.o: AltMatrix1D.h AltMatrix2D.h Holding.h Header.h files.h options.h
AltMatrix1D.o: AltMatrix1D.h compare.h const.h
AltMatrix2D.o: AltMatrix2D.h compare.h const.h
Holding.o: Holding.h Trick.h compare.h const.h
LoopHold.o: Trick.h compare.h const.h LoopHold.h Holding.h DefList.h
LoopHold.o: AltList.h TrickList.h Segment.h Header.h
SimpleMoves.o: Trick.h compare.h const.h Header.h DefList.h AltList.h
SimpleMoves.o: TrickList.h Segment.h MoveList.h SideMoveList.h Holding.h
SimpleMoves.o: Hash.h summary.h sort.h SimpleMoves.h ComplexMoves.h Twiddle.h
SimpleMoves.o: hist.h Dist.h LoopHold.h single.h
ComplexMoves.o: Header.h Trick.h compare.h const.h DefList.h AltList.h
ComplexMoves.o: TrickList.h Segment.h MoveList.h SideMoveList.h Holding.h
ComplexMoves.o: Hash.h summary.h sort.h ComplexMoves.h stats.h portab.h
ComplexMoves.o: files.h single.h options.h hist.h
Header.o: Header.h Trick.h compare.h const.h
Hash.o: Hash.h Header.h Trick.h compare.h const.h
Twiddle.o: Twiddle.h hist.h check.h options.h single.h const.h
Dist.o: Dist.h hist.h Holding.h Trick.h compare.h const.h single.h check.h
Dist.o: options.h
sort.o: sort.h
files.o: summary.h files.h
check.o: MoveList.h SideMoveList.h Header.h Trick.h compare.h const.h
check.o: Holding.h DefList.h AltList.h TrickList.h Segment.h Hash.h summary.h
check.o: sort.h ComplexMoves.h hist.h check.h
hist.o: hist.h const.h
options.o: single.h options.h const.h
summary.o: summary.h
single.o: portab.h Header.h Trick.h compare.h const.h MoveList.h
single.o: SideMoveList.h Holding.h DefList.h AltList.h TrickList.h Segment.h
single.o: Hash.h summary.h sort.h SimpleMoves.h ComplexMoves.h stats.h
single.o: Twiddle.h hist.h Dist.h LoopHold.h files.h options.h single.h
