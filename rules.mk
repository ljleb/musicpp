.PHONY: all watch build-debug

MPP_DIR := ./musicpp

CXX := /usr/bin/g++
CXX_INCLUDES := -I./src -I$(MPP_DIR)/include
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

out/main-debug: src/main.cpp | out
	$(CXX) $(CXX_OPTIONS) $(CXX_DEBUG_OPTIONS) $(CXX_INCLUDES) -o $@ $<

out/main.wav: out/main.raw | out
	$(RXX) $(RXX_OPTIONS) $< $@

out/main.raw: out/main | out
	$< $@ $(SAMPLE_RATE)

out/main: src/main.cpp | out
	$(CXX) $(CXX_OPTIONS) $(CXX_RELEASE_OPTIONS) $(CXX_INCLUDES) -o $@ $<

out:
	mkdir -p out

clean:
	rm -rf out
