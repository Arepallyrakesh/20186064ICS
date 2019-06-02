// Vo Tran Thanh Luong - 1551020
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <strings.h>
#include <math.h>
#include "cachelab.h"

typedef unsigned long long int memoryAddress;

//Struct to get the cache parameter, this is like the front page of cache.
struct cacheParameter {      // store all the  basic info of cache
	int s; 
	int b; 
	int E; 
	int S; 
	int B; 
	int hits;
	int misses;
	int evicts;
};

//Struct to store the sets present in the cache.
struct setLine {          // store necessary info of a Line
	int latestUsed; //stores the address of latest
	int valid;
	memoryAddress tag;
	char *block;
};

//struct to store the array of lines in a set
struct cacheSet{    // multi lines build up a set
	struct setLine *lines;
}; 

//Struct to store the array for all the sets in the cache.
struct cache{      // a cache has many sets
	 struct cacheSet *sets;
};

//initialise the cache memory and return the same.
struct cache initCache(long long numberOfSets, int numberOfLines, long long sizeOfBlock) 
{    // this function constructs the cache by the given information

	struct cache newCache;	 //cache memory
	struct cacheSet set;	//array with all sets
	struct setLine line;	//Array with all lines in a set
	int indexOfSet;
	int indexOfLine;

	//alllocate the memory for the cache, with the help S*size of set
	newCache.sets = (struct cacheSet*) malloc(sizeof(struct cacheSet) * numberOfSets);      // do memory allocation in C

	//Repeat the entire cache and place the zero.
	for (indexOfSet = 0; indexOfSet < numberOfSets; indexOfSet ++) 
	{        // this loop loops through every line in every set and put the default value 0 inside every slot. (because we contain nothing in the cache)
		
		set.lines =  (struct setLine *) malloc(sizeof(struct setLine) * numberOfLines);
		newCache.sets[indexOfSet] = set;

		for (indexOfLine = 0; indexOfLine < numberOfLines; indexOfLine ++) 
		{
			line.latestUsed = 0;
			line.valid = 0;
			line.tag = 0; 
			set.lines[indexOfLine] = line;	
		}
		
	} 

	return newCache;
	
}


int detectEmptyLine(struct cacheSet exampleSet, struct cacheParameter exampleParameter) {
	// check is whether the line that is matched is empty or not
	
	int numberOfLines = exampleParameter.E;
	int index;
	struct setLine line;

	//loop over the cache and check wether it is empty. If empty return the index on that line. Else return -1
	for (index = 0; index < numberOfLines; index ++) {
		line = exampleSet.lines[index];
		if (line.valid == 0) {    // one line is available
			return index;
		}
	}
	return -1;     // no line is available
}

int detectEvictLine(struct cacheSet exampleSet, struct cacheParameter exampleParameter, int *usedLines) {
	// this function detects the line that can be evicted ( the least recently used line)

	int numberOfLines = exampleParameter.E;
	int maxFreqUsage = exampleSet.lines[0].latestUsed;     //store usage frequency
	int minFreqUsage = exampleSet.lines[0].latestUsed;	 // store usage frequency
	int minFreqUsage_index = 0;
	int indexOfLine;

	//Initiallly , take min and max range as 0. Then compare then with the number the number each line of a set's latest used variable
	//If the min is the line used then store the min and max in  array and then retun the minimum used array.
	//very basic loop, compare each line with max & min to decide
	for (indexOfLine = 1; indexOfLine < numberOfLines; indexOfLine ++) {
		if (minFreqUsage > exampleSet.lines[indexOfLine].latestUsed) {
			minFreqUsage_index = indexOfLine;	
			minFreqUsage = exampleSet.lines[indexOfLine].latestUsed;
		}

		if (maxFreqUsage < exampleSet.lines[indexOfLine].latestUsed) {
			maxFreqUsage = exampleSet.lines[indexOfLine].latestUsed;
		}
	}

	usedLines[0] = minFreqUsage;
	usedLines[1] = maxFreqUsage;
	return minFreqUsage_index;
}

//Here we performa the cache operations and calculate the no of hits, misses and evictions.
struct cacheParameter accessTheCacheData(struct cache myCache, struct cacheParameter exampleParameter, memoryAddress address) {
		int indexOfLine;
		int checkFullCache = 1;     // assume that cache is full

		int numberOfLines = exampleParameter.E; //no of lines
		int previousHit = exampleParameter.hits;	//Used to check for misses during the cahce operation.

		//Here we get the address from the arguments as address. which contains, Tag, set, block.
		//Set and block are of 4 bits each. 
		int tagSize = (64 - (exampleParameter.s + exampleParameter.b));    // take the valid tag out t = m-s-b will get 56
		//To get the address of the tag. We fist do the right shift by 56 bits right.
		//This will give the address of tag.
		memoryAddress inputTag = address >> (exampleParameter.s + exampleParameter.b);
		//Now in order to get the address of the set we first eliminate the tag by left shift by 56 
		unsigned long long temp = address << (tagSize);
		//And then eliminate the block by right shift by 60 bits.
		unsigned long long indexOfSet = temp >> (tagSize + exampleParameter.b);
		
		//We store the sets
  		struct cacheSet exampleSet = myCache.sets[indexOfSet];

  		//iterate over the lines in a set
		for (indexOfLine = 0; indexOfLine < numberOfLines; indexOfLine ++) 	{
			
			//If the valid bit in a line is not zero, then it means it is being occupaied. Then check wether the occupied is what we required.?
			if (exampleSet.lines[indexOfLine].valid) {   // check the valid tag != 0
				//Check weather the occupied is what we need . This can be done by comparing the input tag and the tag of the line
				if (exampleSet.lines[indexOfLine].tag == inputTag) {
						//check for matching tag
					//If both match then update last used by incrementing.
					exampleSet.lines[indexOfLine].latestUsed ++;  // update for later use of eviction
					exampleParameter.hits ++;    // tag match -> raise hit
				}
			//If it is being occupied and then 
			} else if (!(exampleSet.lines[indexOfLine].valid) && (checkFullCache)) {
				// valid tag = 0, fullcache != 0
				checkFullCache = 0;	    // reset this to 0	because there is empty space left.
			}
			// 
		}	

		if (previousHit == exampleParameter.hits) {   // if after the above loop nothing hit -> we miss
			exampleParameter.misses++;    // raise miss
		} else {
			return exampleParameter;
		}
		int *usedLines = (int*) malloc(sizeof(int) * 2);
		//get the index of the min used line
		int minFreqUsage_index = detectEvictLine(exampleSet, exampleParameter, usedLines);	

		
		if (checkFullCache)     // if cache is full (checkFullCache!=0) do eviction
		{
			exampleParameter.evicts++;
			exampleSet.lines[minFreqUsage_index].tag = inputTag;
			exampleSet.lines[minFreqUsage_index].latestUsed = usedLines[1] + 1;
		
		}

		else        // else write to tge empty line
	        {
			int empty_index = detectEmptyLine(exampleSet, exampleParameter);
			exampleSet.lines[empty_index].tag = inputTag;
			exampleSet.lines[empty_index].valid = 1;
			exampleSet.lines[empty_index].latestUsed = usedLines[1] + 1;
		}						

		free(usedLines);
		return exampleParameter;
}

int main(int argc, char **argv)
{
	
	struct cache myCache;	//Cache memory
	struct cacheParameter exampleParameter;	//cache memory parameters
	long long numberOfSets;	// S
	long long sizeOfBlock;	// B
	FILE *openTrace; //To open the file
	char instructionInTraceFile;	//file input
	memoryAddress address;	//Given address
	int size;	//size
	char *trace_file;
	char c;
	/* this part takes in argument. More on how do I do this-> read report file */
	//read the input
    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1)
	{
        switch(c)
		{
		//set ofSet bits
        case 's':
            exampleParameter.s = atoi(optarg);
            break;
        //No of set bits
        case 'E':
            exampleParameter.E = atoi(optarg);
            break;
        //blocks
        case 'b':
            exampleParameter.b = atoi(optarg);
            break;
        // t test
        case 't':
            trace_file = optarg;
            break;
        //Test may be 
        case 'h':
            exit(0);
        default:
            exit(1);
        }
    }
   /* end of take in arguments from command line */ 

    //Declare the required variables
 	numberOfSets = pow(2.0, exampleParameter.s);   // get Number of set by 2^s
	sizeOfBlock = pow(2.0, exampleParameter.b);  //  get sizeOfBlock by 2^b
	exampleParameter.hits = 0;
	exampleParameter.misses = 0;
	exampleParameter.evicts = 0;

	//initialise the cahce memory.
	myCache = initCache (numberOfSets, exampleParameter.E, sizeOfBlock);

	//reading the input from the trace file. Here we have a switch case for hanlding the inputs 
	//such as I-initialise,L-Load,S-Store,M-Modify
	openTrace  = fopen(trace_file, "r");
	if (openTrace != NULL) {
		while (fscanf(openTrace, " %c %llx,%d", &instructionInTraceFile, &address, &size) == 3) {
			switch(instructionInTraceFile) {
				case 'I':
					break;
				case 'L':
					exampleParameter = accessTheCacheData(myCache, exampleParameter, address);
					break;
				case 'S':
					exampleParameter = accessTheCacheData(myCache, exampleParameter, address);
					break;
				case 'M':
					exampleParameter = accessTheCacheData(myCache, exampleParameter, address);
					exampleParameter = accessTheCacheData(myCache, exampleParameter, address);	
					break;
				default:
					break;
			}
		}
	}
	/* end of read file */
	//Printing the summary of
    printSummary(exampleParameter.hits, exampleParameter.misses, exampleParameter.evicts);
	fclose(openTrace);
    return 0;
}