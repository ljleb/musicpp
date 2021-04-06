.PHONY: all

all: out/a.wav

out/a.wav: out/a.raw
	sox --rate 44100 --encoding floating-point --bits 32 --channels 1 $< $@

out/a.raw: out/main
	$< $@

out/main: src/main.cpp
	g++ -std=c++2a -O3 -I./src -o $@ $<
