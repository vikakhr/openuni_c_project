.entry LOOP
.entry LENGTH
.extern L3
.extern W
;.extern S1
MAIN: mov S1.1, W
mov r3, S1.2
prn #+5
S1: .struct 8, "a"
