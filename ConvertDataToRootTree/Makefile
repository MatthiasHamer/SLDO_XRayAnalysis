# APEffUncert

LIBS = `root-config --glibs` -L/usr/X11R6/lib -lm -ldl -lstdc++

CXXFLAGS =  -O3 -Wall --exceptions -I$(ROOTSYS)/include -I./

%.o : %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
convert: convert.cpp
	$(CXX) $(CXXFLAGS) -c $<
	$(CXX) $(LDFLAGS) $(LIBS) convert.o -o convert
clean:
	rm -f convert
	rm -f *.o
	rm -f *~
