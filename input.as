mov S1.2, r3 
LABEL: prn #-5
inc K
K: .data 22
S: .extern abc
LENGTH: .data 6,-9,15
MAIN: mov S1.1, W
S1: .struct 8, "ab"	
macro m1
.entry LENGTH
.extern L3
bne L3
.extern W
endmacro
m1
