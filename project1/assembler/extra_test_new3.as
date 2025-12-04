        lw   0   2   one
        lw   0   3   neg
        lw   0   4   total
        nor  2   2   1
check   beq  0   4   end
        nor  4   4   5
        nor  1   5   5
        beq  0   5   isEven
        add  6   2   6
        beq  0   0   next
isEven  add  7   2   7
next    add  4   3   4
        beq  0   0   check
end     halt
total   .fill   4
one     .fill   1
neg     .fill  -1
