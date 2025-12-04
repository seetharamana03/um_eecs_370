            lw      0   1   mcand       ; R1 = mcand
            lw      0   2   mplier      ; R2 = mplier
            lw      0   3   zero        ; R3 (result)
            lw      0   4   zero        ; Loop counter
            lw      0   5   one         ; mask
            lw      0   6   zero        ; And value
            lw      0   7   max         ; temp register
loop        beq     4   7   end         ; while loop is less than 16
            nor     2   0   7           
            nor     5   0   6
            nor     7   6   6           ; (R2 & mask)
            beq     6   0   skip        ; if lsb is 0
            add     1   3   3           ; R3 = R1 + R3
skip        add     1   1   1           ; shift mcand left
            add     5   5   5           ; shift mask left
            lw      0   6   one
            lw      0   7   max
            add     4   6   4           ; increment loop counter
            beq     0   0   loop        ; restart loop
end         halt
zero        .fill   0
one         .fill   1
max         .fill   16
mcand       .fill   6203
mplier      .fill   1429