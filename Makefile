.PHONY: all watch

all: out/a.wav

watch:
	while inotifywait --quiet --event modify --recursive ./src; do \
		make && \
		play out/a.wav; \
	done

out/a.wav: out/a.raw
	sox --rate 44100 --encoding floating-point --bits 32 --channels 1 $< $@

out/a.raw: out/main
	$< $@

out/main: src/main.cpp
	g++ -std=c++2a -O3 -I./src -o $@ $<
