.PHONY: all watch build-debug


# c++ compiler
CXX := /usr/bin/g++

# c++ include files
CXX_INCLUDES := -I./src

# c++ options
CXX_OPTS := -std=c++2a
# c++ release options
CXX_RELEASE_OPTS := -O3

# c++ debug options
CXX_DEBUG_OPTS := -g -O0


# raw header
RXX := /usr/bin/sox

# raw options
RXX_OPTS := --rate $(SAMPLE_RATE) --encoding floating-point --bits 32 --channels 1


# wav player
WXX := play

# wav options
WXX_OPTS :=


all: out/main.wav

watch:
	while inotifywait --quiet --event modify --recursive ./src; do \
		make play; \
	done

play: out/main.wav
	$(WXX) $(WXX_OPTS) $<

build-debug: out/main-debug

out/main-debug: src/main.cpp
	$(CXX) $(CXX_OPTS) $(CXX_DEBUG_OPTS) $(CXX_INCLUDES) -o $@ $<

out/main.wav: out/main.raw
	$(RXX) $(RXX_OPTS) $< $@

out/main.raw: out/main
	$< $@ $(SAMPLE_RATE)

out/main: src/main.cpp
	$(CXX) $(CXX_OPTS) $(CXX_RELEASE_OPTS) $(CXX_INCLUDES) -o $@ $<
