        lw      0   5   base
        lw      0   6   count
        lw      0   7   minus
loop    add     7   6   6
        nor     6   5   3
        beq     5   6   loop
        halt
base        .fill   0
count       .fill   10
minus       .fill   -1
