# `asl`

`asl` is a collection of libraries built as an alternative to the C++ standard library
suited to our needs. It has not been thoroughly tested in production and thus we
discourage anyone from using it, although feel free to read the code and have fun
with it.

`asl` in parts reproduces the standard C++ library, and in other parts diverges
from it and even adds higher level features such as logging and testing.

## Overview

- **Built with C++ 23.** We do not care about backwards compatibility. We intend on
  progressively adopting newer, carefully selected C++ features as they become
  available, and modernizing older implementations where it makes sense.
- **Written with compile-time and runtime performance in mind.** Although we don't go
  as hardcore as other codebases, we strive to keep implementations simple and performant,
  mainly by avoiding performance pessimization.
- **Designed for Clang only.** Supporting multiple compilers would be nice, but
  the uneven rate of adoption of C++ features makes this annoying. Anyway we don't
  need it, so we don't do it.
- **Leveraging Bazel.** Although fairly complex, Bazel has proven to be a reliable
  tool for building cross-platform and multi-language codebases. These libraries can
  be easily integrated in other codebases through Bzlmod and our [private registry](https://git.stevenlr.com/460nm/bazel-registry).
- Currently this codebase is developed and tested on Windows x64, Linux x64,
  and Linux arm64.

## Code structure & features

Soon...

## License

`asl` is licensed under the 3-clause BSD License. See [LICENSE.txt](LICENSE.txt) for more information.
