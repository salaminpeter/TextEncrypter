#!/bin/bash

emcc ../TextEncrypter.cpp ../TextEncrypterWrapper.cpp -O3 \
    -s WASM=1 \
    -s EXPORTED_FUNCTIONS='["_LoadImage", "_AddText", "_ReadText", "_malloc", "_free"]' \
    -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
    -o textencrypter.js