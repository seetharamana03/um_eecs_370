/*
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256
#define MAX_ACCESSES 1000000
// **Note** this is a preprocessor macro. This is not the same as a function.
// Powers of 2 have exactly one 1 and the rest 0's, and 0 isn't a power of 2.
#define is_power_of_2(val) (val && !(val & (val - 1)))
/*
 * Accesses 1 word of memory.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads and 1 for writes.
 * write_data is a word, and is only valid if write_flag is 1.
 * If write flag is 1, mem_access does: state.mem[addr] = write_data.
 * The return of mem_access is state.mem[addr].
 */
extern int mem_access(int addr, int write_flag, int write_data);
/*
 * Returns the number of times mem_access has been called.
 */
extern int get_num_mem_accesses(void);
// Use this when calling printAction. Do not modify the enumerated type below.
enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};
/* You may add or remove variables from these structs */
typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int lruLabel;
    int tag;
    int valid;
} blockStruct;
typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int numSets;
    int blocksPerSet;
    int cacheType; // 0 is set-associative, 1 is fully-associative, 2 is direct-mapped
    // add any variables for end-of-run stats
} cacheStruct;
/* Global Cache variable */
cacheStruct cache;
void printAction(int, int, enum actionType);
void printCache(void);
void updateLRU(cacheStruct *, int, int);
int findLRU(cacheStruct *, int);
int num_hits = 0;
int num_misses = 0;
int num_writebacks = 0;
int total_accessed = 0;
int num_dirty = 0;
int accesses[MAX_ACCESSES];
int num_accesses = 0;
/*
 * Set up the cache with given command line parameters. This is
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet)
{
    if (blockSize <= 0 || numSets <= 0 || blocksPerSet <= 0)
    {
        printf("error: input parameters must be positive numbers\n");
        exit(1);
    }
    if (blocksPerSet * numSets > MAX_CACHE_SIZE)
    {
        printf("error: cache must be no larger than %d blocks\n", MAX_CACHE_SIZE);
        exit(1);
    }
    if (blockSize > MAX_BLOCK_SIZE)
    {
        printf("error: blocks must be no larger than %d words\n", MAX_BLOCK_SIZE);
        exit(1);
    }
    if (!is_power_of_2(blockSize))
    {
        printf("warning: blockSize %d is not a power of 2\n", blockSize);
    }
    if (!is_power_of_2(numSets))
    {
        printf("warning: numSets %d is not a power of 2\n", numSets);
    }
    printf("Simulating a cache with %d total lines; each line has %d words\n",
           numSets * blocksPerSet, blockSize);
    printf("Each set in the cache contains %d lines; there are %d sets\n",
           blocksPerSet, numSets);
    /********************* Initialize Cache *********************/
    cache.blockSize = blockSize;
    cache.blocksPerSet = blocksPerSet;
    cache.numSets = numSets;
    if (numSets == 1)
    {
        cache.cacheType = 1;
    }
    else if (blocksPerSet == 1)
    {
        cache.cacheType = 2;
    }
    else
    {
        cache.cacheType = 0;
    }
    for (int i = 0; i < (blocksPerSet * numSets); i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            cache.blocks[i].data[j] = 0;
        }
        cache.blocks[i].dirty = 0;
        cache.blocks[i].lruLabel = 0;
        cache.blocks[i].tag = 0;
        cache.blocks[i].valid = 0;
    }
    return;
}
/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data)
{
    /* The next line is a placeholder to connect the simulator to
    memory with no cache. You will remove this line and implement
    a cache which interfaces between the simulator and memory. */
    int blockNumber = addr / cache.blockSize;
    int setIndex = blockNumber % cache.numSets;
    int tag = blockNumber / cache.numSets;
    int blockOffset = addr % cache.blockSize;
    int startIndex = setIndex * cache.blocksPerSet; // index used to iterate through cache
    int endIndex = startIndex + cache.blocksPerSet;
    int hit = 0;
    int hitBlockIndex = -1;
    int targetIndex = -1;
    for (int i = startIndex; i < endIndex; i++)
    {
        if (cache.blocks[i].valid == 1 && cache.blocks[i].tag == tag)
        {
            hit = 1;
            hitBlockIndex = i;
            break;
        }
    }
    if (hit == 1)
    {
        num_hits++;
        targetIndex = hitBlockIndex;
    }
    else
    {
        num_misses++;
        int blockStartAddr = blockNumber * cache.blockSize; // index used to get first index of block element from memory
        int spaceAvailable = 0;
        int replacementIndex = -1;
        for (int i = startIndex; i < endIndex; i++) // indexing data within correct tag
        {
            if (cache.blocks[i].valid == 0 && spaceAvailable == 0)
            {
                for (int j = 0; j < cache.blockSize; j++)
                {
                    cache.blocks[i].data[j] = mem_access(blockStartAddr + j, 0, 0);
                }
                cache.blocks[i].valid = 1;
                cache.blocks[i].tag = tag;
                if (cache.blocks[i].dirty == 1)
                {
                    cache.blocks[i].dirty = 0;
                    num_dirty--;
                }
                printAction(blockStartAddr, cache.blockSize, memoryToCache); // there is an empty block in set so directly put memory to cache
                total_accessed += cache.blockSize;
                updateLRU(&cache, setIndex, i);
                spaceAvailable = 1;
                replacementIndex = i;
                targetIndex = replacementIndex;
            }
        }
        if (spaceAvailable == 0)
        {
            replacementIndex = findLRU(&cache, setIndex);
            int lruBlockNumber = cache.blocks[replacementIndex].tag * cache.numSets + setIndex;
            int lruBlockStartAddr = lruBlockNumber * cache.blockSize;
            if (cache.blocks[replacementIndex].dirty == 1)
            {
                for (int j = 0; j < cache.blockSize; j++)
                {
                    mem_access(lruBlockStartAddr + j, 1, cache.blocks[replacementIndex].data[j]);
                }
                printAction(lruBlockStartAddr, cache.blockSize, cacheToMemory); // write clean cache data to memory
                total_accessed += cache.blockSize;
            }
            else
            {
                printAction(lruBlockStartAddr, cache.blockSize, cacheToNowhere); // disregard clean cache data
            }
            for (int i = 0; i < cache.blockSize; i++)
            {
                cache.blocks[replacementIndex].data[i] = mem_access(blockStartAddr + i, 0, 0);
            }
            if (cache.blocks[replacementIndex].dirty == 1)
            {
                cache.blocks[replacementIndex].dirty = 0;
                num_dirty--;
            }
            cache.blocks[replacementIndex].tag = tag;
            cache.blocks[replacementIndex].valid = 1;
            printAction(blockStartAddr, cache.blockSize, memoryToCache); // put relevant memory data into cache
            total_accessed += cache.blockSize;
            updateLRU(&cache, setIndex, replacementIndex);
            targetIndex = replacementIndex;
        }
    }
    if (write_flag == 0)
    {
        int newData = cache.blocks[targetIndex].data[blockOffset];
        printAction(addr, 1, cacheToProcessor);
        updateLRU(&cache, setIndex, targetIndex);
        return newData;
    }
    else
    {
        num_writebacks++;
        cache.blocks[targetIndex].data[blockOffset] = write_data;
        cache.blocks[targetIndex].dirty = 1;
        num_dirty++;
        printAction(addr, 1, processorToCache);
        updateLRU(&cache, setIndex, targetIndex);
        return write_data;
    }
}
/*
 * print end of run statistics like in the spec. **This is not required**,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 * DO NOT print $$$ in this function
 */
void printStats(void)
{
    printf("$$$ Main memory words accessed: %d\n", total_accessed);
    printf("End of run statistics:\n");
    printf("hits %d, misses %d, writebacks %d\n", num_hits, num_misses, num_writebacks);
    printf("%d dirty cache blocks left\n", num_dirty);
    return;
}
/*
 * Log the specifics of each cache action.
 *
 *DO NOT modify the content below.
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);
    if (type == cacheToProcessor)
    {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache)
    {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache)
    {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory)
    {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere)
    {
        printf("from the cache to nowhere\n");
    }
    else
    {
        printf("Error: unrecognized action\n");
        exit(1);
    }
}
/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache(void)
{
    int blockIdx;
    int decimalDigitsForWaysInSet = (cache.blocksPerSet == 1) ? 1 : (int)ceil(log10((double)cache.blocksPerSet));
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set)
    {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block)
        {
            blockIdx = set * cache.blocksPerSet + block;
            if (cache.blocks[set * cache.blocksPerSet + block].valid)
            {
                printf("\t\t[ %0*i ] : ( V:T | D:%c | LRU:%-*i | T:%i )\n\t\t%*s{",
                       decimalDigitsForWaysInSet, block,
                       (cache.blocks[blockIdx].dirty) ? 'T' : 'F',
                       decimalDigitsForWaysInSet, cache.blocks[blockIdx].lruLabel,
                       cache.blocks[blockIdx].tag,
                       7 + decimalDigitsForWaysInSet, "");
                for (int index = 0; index < cache.blockSize; ++index)
                {
                    printf(" 0x%08X", cache.blocks[blockIdx].data[index]);
                }
                printf(" }\n");
            }
            else
            {
                printf("\t\t[ %0*i ] : ( V:F )\n\t\t%*s{     }\n", decimalDigitsForWaysInSet, block, 7 + decimalDigitsForWaysInSet, "");
            }
        }
    }
    printf("end cache\n");
}
void updateLRU(cacheStruct *cache, int setIndex, int accessedBlockIndex)
{
    accesses[num_accesses] = accessedBlockIndex;
    num_accesses++;
    int diffNumbers[cache->blocksPerSet];
    int numUnique = 0;

    int startIndex = setIndex * cache->blocksPerSet;
    int endIndex = startIndex + cache->blocksPerSet;

    for (int i = 0; i < cache->blocksPerSet; i++)
    {
        diffNumbers[i] = -1;
    }

    for (int i = num_accesses - 1; i >= 0; i--)
    {
        int alreadySeen = 0;
        int curr_access = accesses[i];
        for (int j = 0; j < numUnique; j++)
        {
            if (diffNumbers[j] == curr_access)
            {
                alreadySeen = 1;
                break;
            }
        }

        if (alreadySeen == 0)
        {
            for (int k = startIndex; k < endIndex; k++)
            {
                if (k == curr_access)
                {
                    diffNumbers[numUnique] = curr_access;
                    numUnique++;
                }
            }
        }

        if (numUnique == cache->blocksPerSet)
        {
            break;
        }
    }

    for (int i = startIndex; i < endIndex; i++)
    {
        if (i == diffNumbers[numUnique - 1])
        {
            cache->blocks[i].lruLabel = 1;
        }
        else
        {
            cache->blocks[i].lruLabel = 0;
        }
    }
}

int findLRU(cacheStruct *cache, int setIndex)
{
    int startIndex = setIndex * cache->blocksPerSet;
    int endIndex = startIndex + cache->blocksPerSet;
    int currLRU = -1;
    for (int i = startIndex; i < endIndex; i++)
    {
        if (cache->blocks[i].lruLabel == 1)
        {
            currLRU = i;
            break;
        }
    }
    return currLRU;
}