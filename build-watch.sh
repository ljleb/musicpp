#!/bin/bash

cd "$(dirname "${0}")" || exit

while inotifywait --event modify --recursive ./src; do
    ./build.sh && \
    ffplay -vn -autoexit -loglevel quiet -f f32le -ar 44100 -ac 1 ./out/a.raw
done
