        add     1       2       3
label   nor     4       5       6
        lw      0       1       label
        sw      1       2       3
        beq     1       2       label
        jalr    2       3                           
        halt
        noop
        .fill   42
        .fill   label
