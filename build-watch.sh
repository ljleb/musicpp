#!/bin/bash

cd "$(dirname "${0}")" || exit

while inotifywait --quiet --event modify --recursive ./src; do
    time make && \
    play out/a.wav
done
