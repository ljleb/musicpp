.PHONY: all watch build-debug

CXX := /usr/bin/g++
CXX_INCLUDES := -I./src
CXX_OPTIONS := -std=c++2a
CXX_RELEASE_OPTIONS := -O3
CXX_DEBUG_OPTIONS := -g -O0

RXX := /usr/bin/sox
RXX_OPTIONS := --rate $(SAMPLE_RATE) --encoding floating-point --bits 32 --channels 1

WXX := play
WXX_OPTIONS :=

all: out/main.wav

watch:
	while inotifywait --quiet --event modify --recursive ./src; do make play; done

play: out/main.wav
	$(WXX) $(WXX_OPTIONS) $<

build-debug: out/main-debug

out/main-debug: src/main.cpp
	$(CXX) $(CXX_OPTIONS) $(CXX_DEBUG_OPTIONS) $(CXX_INCLUDES) -o $@ $<

out/main.wav: out/main.raw
	$(RXX) $(RXX_OPTIONS) $< $@

out/main.raw: out/main
	$< $@ $(SAMPLE_RATE)

out/main: src/main.cpp
	$(CXX) $(CXX_OPTIONS) $(CXX_RELEASE_OPTIONS) $(CXX_INCLUDES) -o $@ $<
