SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

objects = ml_workngTrials

DEBUG = -g

CPPFLAGS := $(CPPFLAGS)
CXXFLAGS := $(CXXFLAGS) $(DEBUG)

all: $(objects) ;

ml_%.o::ml_%.cpp ;\
    $(COMPILE.cc) -c $@ $(^F)

ml_%.o::ml_%.o ;\
    $(LINK.cc) -o $@ $(^F) $(SFML_LIBS)

ml_%::ml_%.cpp ;\
    $(LINK.cc) -c $@ $(^F)

clean :
	rm -f *.o $(objects)