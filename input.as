lea #5, r3
bne L3
K : .data 22
S: .extern abc
inc K
LENGTH: .data 6,-9,15
MAIN: mov S5.1, W
macro m1
.entry LENGTH
.extern L3
prn #-5
.extern W
endmacro
m1
S5: .struct 8, "ab"
P: .data 22
.extern LENGTH


