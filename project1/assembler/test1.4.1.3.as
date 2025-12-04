        lw      0       1       one
        lw      0       2       five
        lw      3       5       four
loop    add     1       1       1
        sw      0       1       small
        beq     1       2       loop
        halt     
one	.fill	1
two     .fill   2
three	.fill   3
four	.fill	4
five	.fill   5
six	.fill	6
large	.fill	77
medium	.fill	34
small	.fill	14