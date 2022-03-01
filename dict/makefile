FLAGS=
LIBS=-lrt -lpthread

ifeq ($(MODE), debug)
FLAGS:= $(FLAGS) -g
endif

all:
	g++ $(FLAGS) -o dict *.cpp $(LIBS)

clean:
	rm -f *.o dict
