# PostgreSQL Visualization Experiments

A set of experiments to visualize PostgreSQL internals.

## NOT PRODUCTION READY

By "experiments" it means "test". It is not ready to be used in production.

## DEPENDENCIES

- Raylib 5.0
- libpq-dev
- libpq5

## Experiments

### `pgviz_buffercache`

Visualize the PostgreSQL shared buffer state at 10 FPS.

| - | 0  | 1  | 2  | 3  | 4  |
| 0 | 1  | 2  | 3  | 4  | 5  |
| 1 | 6  | 7  | 8  | 9  | 10 |
| 2 | 11 | 12 | 13 | 14 | 15 |
| 3 | 16 | 17 | 18 | 19 | 20 |
| 4 | 21 |    |    |    |    |

## LICENSE

The MIT License (MIT)

Copyright (c) 2024 Dickson S. Guedes

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

