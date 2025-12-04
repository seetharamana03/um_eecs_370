        lw      0       1       lumen
        noop
        noop
        noop
        lw      0       2       lumen
        noop
        noop
        noop
        beq     1       2       tuff
        sw      0       2       mark
tuff    add     1       2       3
done    halt
five    .fill 5
work    .fill 3
sever   .fill 2
lumen   .fill 1
mark    .fill 6