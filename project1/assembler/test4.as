        lw    0    1    data
        lw    0    2    data1
        lw    0    3    data2
        lw    0    4    data3
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
data1	.fill	10
data2    .fill    2
data3    .fill    4
