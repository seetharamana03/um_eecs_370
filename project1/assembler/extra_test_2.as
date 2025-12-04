		lw		0	1 sever
		lw		0	2 haley
		lw 		0	3 neg1
start	add		3	2 2
        nor     2   1   4
		beq	    1	2	start
		halt
sever	.fill	0
haley		.fill	10
neg1	.fill 	-1