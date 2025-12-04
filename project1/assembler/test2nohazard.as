	    beq	0	0	offset
	    noop
	    noop
	    add	3	4	5
offset	nor	5	4	7
	    halt
data1	.fill	12345