LIBS_ = sfml-graphics sfml-audio sfml-window sfml-system
LIBS = $(patsubst %,-l%,$(LIBS_))

main: main.cpp staff.cpp audioProcessor.cpp types.cpp
	g++ -g -o $@ $^ $(LIBS) -std=c++20

.PHONY: clean
clean:
	rm -f main