		lw	    0	    1	    three
		sw	    1	    1	    six
		lw		0		5		medium
		sw	    3	    1	    large
		lw		0		1		small
		sw		0		5		two
		add		1		5		3
		sw		0		3		three		
		halt
one	    .fill	1
two		.fill   2
three	.fill   3
four	.fill	4
five	.fill   5
six		.fill	6
large	.fill	77
medium	.fill	34
small	.fill	14
