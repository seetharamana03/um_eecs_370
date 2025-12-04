warning: blockSize 3 is not a power of 2
Simulating a cache with 8 total lines; each line has 3 words
Each set in the cache contains 4 lines; there are 2 sets
$$$ transferring word [0-2] from the memory to the cache
$$$ transferring word [0-0] from the cache to the processor
$$$ transferring word [6-8] from the memory to the cache
$$$ transferring word [7-7] from the cache to the processor
$$$ transferring word [1-1] from the cache to the processor
$$$ transferring word [9-11] from the memory to the cache
$$$ transferring word [11-11] from the cache to the processor
$$$ transferring word [2-2] from the cache to the processor
$$$ transferring word [10-10] from the cache to the processor
$$$ transferring word [3-5] from the memory to the cache
$$$ transferring word [3-3] from the cache to the processor
$$$ transferring word [4-4] from the cache to the processor
$$$ transferring word [15-17] from the memory to the cache
$$$ transferring word [15-15] from the processor to the cache
$$$ transferring word [5-5] from the cache to the processor
$$$ transferring word [6-6] from the cache to the processor
machine halted
total of 7 instructions executed
final state of machine:

@@@
state:
	pc 7
	memory:
		mem[ 0 ] 0x00810007
		mem[ 1 ] 0x0082000B
		mem[ 2 ] 0x009D000A
		mem[ 3 ] 0x00090001
		mem[ 4 ] 0x00C1000F
		mem[ 5 ] 0x010AFFFD
		mem[ 6 ] 0x01800000
		mem[ 7 ] 0x00000001
		mem[ 8 ] 0x00000002
		mem[ 9 ] 0x00000003
		mem[ 10 ] 0x00000004
		mem[ 11 ] 0x00000005
		mem[ 12 ] 0x00000006
		mem[ 13 ] 0x0000004D
		mem[ 14 ] 0x00000022
		mem[ 15 ] 0x0000000E
	registers:
		reg[ 0 ] 0
		reg[ 1 ] 2
		reg[ 2 ] 5
		reg[ 3 ] 0
		reg[ 4 ] 0
		reg[ 5 ] 4
		reg[ 6 ] 0
		reg[ 7 ] 0
end state
$$$ Main memory words accessed: 15
End of run statistics:
hits 6, misses 5, writebacks 1
1 dirty cache blocks left
