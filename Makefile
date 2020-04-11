CXX=${CROSS_COMPILE}g++
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

ttyio: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) ttyio


