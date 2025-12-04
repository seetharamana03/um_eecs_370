        lw    0    1    data
        lw    0    2    hello
        lw    0    3    type
        lw    0    4    gang
        nor    3    4    5
        noop
start	beq	1	2	end
        add    1    1    1	
	noop
	noop
        noop
        beq    0    0    start
end	halt
data	.fill	5
hello	.fill	10
type    .fill    2
gang    .fill    4
