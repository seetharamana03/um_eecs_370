warning: blockSize 3 is not a power of 2
warning: numSets 3 is not a power of 2
Simulating a cache with 3 total lines; each line has 3 words
Each set in the cache contains 1 lines; there are 3 sets
$$$ transferring word [0-2] from the memory to the cache
$$$ transferring word [0-0] from the cache to the processor
$$$ transferring word [0-2] from the cache to nowhere
$$$ transferring word [9-11] from the memory to the cache
$$$ transferring word [11-11] from the cache to the processor
$$$ transferring word [9-11] from the cache to nowhere
$$$ transferring word [0-2] from the memory to the cache
$$$ transferring word [1-1] from the cache to the processor
$$$ transferring word [15-17] from the memory to the cache
$$$ transferring word [17-17] from the processor to the cache
$$$ transferring word [2-2] from the cache to the processor
$$$ transferring word [16-16] from the cache to the processor
$$$ transferring word [3-5] from the memory to the cache
$$$ transferring word [3-3] from the cache to the processor
$$$ transferring word [15-15] from the processor to the cache
$$$ transferring word [4-4] from the cache to the processor
$$$ transferring word [17-17] from the cache to the processor
$$$ transferring word [5-5] from the cache to the processor
$$$ transferring word [0-2] from the cache to nowhere
$$$ transferring word [9-11] from the memory to the cache
$$$ transferring word [10-10] from the processor to the cache
$$$ transferring word [15-17] from the cache to the memory
$$$ transferring word [6-8] from the memory to the cache
$$$ transferring word [6-6] from the cache to the processor
$$$ transferring word [7-7] from the cache to the processor
$$$ transferring word [11-11] from the processor to the cache
$$$ transferring word [8-8] from the cache to the processor
machine halted
total of 9 instructions executed
final state of machine:

@@@
state:
	pc 9
	memory:
		mem[ 0 ] 0x0081000B
		mem[ 1 ] 0x00C9000E
		mem[ 2 ] 0x00850010
		mem[ 3 ] 0x00D9000F
		mem[ 4 ] 0x00810011
		mem[ 5 ] 0x00C5000A
		mem[ 6 ] 0x000D0003
		mem[ 7 ] 0x00C3000B
		mem[ 8 ] 0x01800000
		mem[ 9 ] 0x00000001
		mem[ 10 ] 0x00000002
		mem[ 11 ] 0x00000003
		mem[ 12 ] 0x00000004
		mem[ 13 ] 0x00000005
		mem[ 14 ] 0x00000006
		mem[ 15 ] 0x00000003
		mem[ 16 ] 0x00000022
		mem[ 17 ] 0x00000003
	registers:
		reg[ 0 ] 0
		reg[ 1 ] 3
		reg[ 2 ] 0
		reg[ 3 ] 37
		reg[ 4 ] 0
		reg[ 5 ] 34
		reg[ 6 ] 0
		reg[ 7 ] 0
end state
$$$ Main memory words accessed: 24
End of run statistics:
hits 9, misses 7, writebacks 4
3 dirty cache blocks left
