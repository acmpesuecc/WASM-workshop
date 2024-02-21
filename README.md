# WASM-workshop

This is the repo for ACM-PESU-ECC's webassembly workshop.

## Running hello.c

```
emcc hello.c -Lraylib/lib -lraylib -lglfw3 -o hello.html -s USE_GLFW=3
```

## Running graphics.c

```
emcc graphics.c -Lraylib/lib -lraylib --preload-file resources -o graphics.html -s USE_GLFW=3
```
