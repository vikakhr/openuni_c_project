MAIN: mov S5.1, W
macro m1
.entry LENGTH
.extern L3
prn #+5
.extern W
endmacro

LABEL: .string "a6"

m1

macro m2
1
2
3
endmacro

m2
