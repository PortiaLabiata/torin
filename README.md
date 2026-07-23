# Torin: a simple data structure collection
## Motivation
This library is mostly an educational project. It is a collection of simple yet powerful data structures and algorithms, adapted to work in resource-constrained (i. e. embedded) applications. For example, all data structures are intended to be used with static memory pools and performance and memory efficiency is prioritised.

## Implemented data structures
Right now the following data structures are supported:
 - Ring buffer
 - Hash table
 - Stack
 - Queue
 - Bloom filter
 - Fenwick tree

There are also following algorithms:
 - Arena allocator
 - Pool allocator
 - Slab allocator

## Installation and usage
This library is intended to be used as a static library. To build it, simply run

```sh
make all
```

in repositary's root. Run `make help` to see all available commands.

Usage examples can be seen in tests, but overall code is pretty self-explanatory.

## Testing
All data structures and algorithms are covered by unit tests. They can be ran by running `make test` in repo's root directory. [utest][https://github.com/sheredom/utest.h] is used as a testing framework.

## Licensing
This is free and unencumbered software released into the public domain.

 Anyone is free to copy, modify, publish, use, compile, sell, or
 distribute this software, either in source code form or as a compiled
 binary, for any purpose, commercial or non-commercial, and by any
 means.

 In jurisdictions that recognize copyright laws, the author or authors
 of this software dedicate any and all copyright interest in the
 software to the public domain. We make this dedication for the benefit
 of the public at large and to the detriment of our heirs and
 successors. We intend this dedication to be an overt act of
 relinquishment in perpetuity of all present and future rights to this
 software under copyright law.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

 For more information, please refer to <http://unlicense.org/>
