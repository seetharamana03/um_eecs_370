	lw      0       1       gang
	noop
	noop
	noop
	noop
start   beq     1       2       end
	add	1	2	2
	beq	0	0	start
end    halt
gang    .fill 	1
six	.fill	6
