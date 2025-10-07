# RecRel_RecElim

This is the official implementation for the paper **"Automatic Recursion Elimination using Recurrence Relations for Synthesis of Stack-free Hardware"** for the 2026 Asia and South Pacific Design Automation Conference (ASP-DAC 2026).

This repository contains:
- An implementation of the method described in the paper.
- The benchmark programs used for evaluation.

## Building the Tool
Requires installing the ROSE framework: https://github.com/rose-compiler/rose/wiki. Make sure the Makefile points to the correct location of ROSE.

After installing ROSE, the analysis tool is built by running:

    make

## Running the Benchmarks

To run the selected set of benchmarks:

    make test


The results are found in the benchmark folder under the "RecRel_RecElim" subfolder.
Each test has its own folder. The header.hpp and test.cpp don't change and are only used for the HLS process.
The prog.cpp contains all the resulting code.

## General Usage

The tool can be run manually as follows:

```
    ./analysis_tool [optional: -I include_path] input_file output_file
```

## Notes

For some files, the following message may appear:

```
    undefined reference to `main'
    collect2: error: ld returned 1 exit status
```

This is a **ROSE-specific warning** and has **no impact** on the correctness or output of the tool. It can safely be ignored.


## Acknowledgemnts
Code from the HeteroRefactor and HLSRecurse repositories was used to help write some of the benchmarks.

HeteroRefactor: https://github.com/heterorefactor/heterorefactor

HLSRecurse: https://github.com/m8pple/hls_recurse
