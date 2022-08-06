mov S1.3, S1.1
mov r1, r9
mov 5, r4
mov r1, #5
lea #-1, r1
lea r1, r2
jsr #-10
hlt r1, r2
LABEL: .struct 8,8
MAIN: .struct "abc"
DATA: .struct "abc", 8
.data r1,r2
.data -5,3,7,+6, -1a
.data a1
.data #5
.string abc"
.string "abc
.entry r1
.extern mov
