This project exhibits matrix multiplication using increasingly aggressive register reuse to speed up the process completion time.
As shown in the results, register use can increase the spead of computationally intensive processes over 4x.
- dgemm 0 (does not utilize registers)
- dgemm 1 (makes non-architecture dependant and small use of registers)
- dgemm 2 (makes significant, aggressive use of registers informed by the processor architecture)
