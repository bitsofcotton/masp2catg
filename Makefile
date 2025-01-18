CXX=	clang++
#CXX=	eg++

# compiler flags.
#CXXFLAGS+=	-O0 -mtune=generic -gfull
#CXXFLAGS+=	-Ofast -mtune=native -gfull
#CXXFLAGS+=	-O3 -mtune=native -g3
#CXXFLAGS+=	-Oz -mtune=native -gfull
CXXFLAGS+=	-O2 -mtune=native -gfull
#CXXFLAGS+=	-O0 -mtune=native -gfull
#CXXFLAGS+=	-pg
MPFLAGS=	-I/usr/local/include -L/usr/local/lib -lomp -fopenmp
#MPFLAGS=	-I/usr/local/include -L/usr/local/lib -lgomp -fopenmp
CXXFLAGS+=	-std=c++11
LDFLAGS+=	-lc++ -L/usr/local/lib
#LDFLAGS+=	-lestdc++ -L/usr/local/lib

CLEANFILES= *.o masp2catg masp2catg32 masp2catgmp masp2catg32mp

clean:
	@rm -rf ${CLEANFILES}

all:	masp2catg masp2catg32 masp2catgmp masp2catg32mp

masp2catg:
	${CXX} ${CXXFLAGS} -static -o masp2catg masp2catg.cc
masp2catg32:
	${CXX} ${CXXFLAGS} -static -D_FLOAT_BITS_=32 -o masp2catg32 masp2catg.cc
masp2catgmp:
	${CXX} ${CXXFLAGS} ${MPFLAGS} -o masp2catgmp masp2catg.cc
masp2catg32mp:
	${CXX} ${CXXFLAGS} ${MPFLAGS} -D_FLOAT_BITS_=32 -o masp2catg32mp masp2catg.cc

