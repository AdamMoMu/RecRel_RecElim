# Benchmarks
The original folder contains the original kernels for each benchmark.
The other folders contain the refactored, HLS-safe code for each approach.
The results for this paper are found under the RecRel_RecElim folder.

Each test has its own folder. 
The header.hpp and test.cpp don't change and are only used for the HLS process.
The prog.cpp contains all the produced code.

The results for HLSRecurse are manual.
The results for HeteroRefactor can be found by following the steps in their repository.