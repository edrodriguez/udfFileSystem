#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <vector>
#include <cstdlib>
#include <string>
#include "files.h"

/*
*	Eduardo Rodriguez Macrillante
*	EECS 4520
*/

using namespace std;

int roundToBlockSize(int size){
	int remainder = size % 2048;

	if (remainder == 0){
		return size;
	}
	
	return size + 2048 - remainder;
}

const int LINESIZE = 256;

int main (int argc, char *argv[]){
	ifstream inputFile;
	char currentLocation[LINESIZE];
	char destination[LINESIZE];
	struct stat currentFileInfo;
	vector<UDFFile> files;
	string fileArgument;
	int numBlocks = 0;
	
	//check if the number of arguments in the necessary
	if (argc < 2){
		perror("No input file");
		exit(0);
	}
	else{
		fileArgument = argv[1];
	}

	//check the -f argument
	if (fileArgument != "-f"){
		perror("No input file");
		exit(0);
	}
	else {
		inputFile.open(argv[2]);
	}
	
	//check that file was properly open
	if (!inputFile) { 
        	perror("Error reading input file");
        	exit(0);
    }
	
	//read contents of the file
	while (inputFile.getline(currentLocation, LINESIZE)) {
        inputFile.getline(destination, LINESIZE);
		
		UDFFile currentFile;
		currentFile.currentLocation = currentLocation;
		currentFile.destination = destination;
		
		//check accessibility
		if (access(currentLocation, F_OK) < 0){
			fprintf(stderr, "Access error for %s\n", currentLocation);
			continue;
		}
		else{
			currentFile.accessible = true;
		}
		
		//check readability
		if (access(currentLocation, R_OK) < 0){
			fprintf(stderr, "File %s cannot be read\n", currentLocation);
			continue;
		}
		else{
			currentFile.readable = true;
		}
		
		//get file info
		if (stat(currentLocation, &currentFileInfo) < 0){
			fprintf(stderr, "File %s stat error\n", currentLocation);
		}
		else {
			//check if file is a regular file
			if (S_ISREG(currentFileInfo.st_mode)){
				//get size and times
				currentFile.roundedSize = roundToBlockSize(currentFileInfo.st_size);
				currentFile.lastAccess = currentFileInfo.st_atim;
				currentFile.modificationDate = currentFileInfo.st_mtim;
			}
			else{
				fprintf(stderr, "File %s is not a regular file\n", currentLocation);
			}
		}
		
		//calculate block size
		numBlocks += currentFile.roundedSize / 2048;
		
		//store info
		files.push_back(currentFile);
	}
	
	printf("Number of 2KB blocks: %d\n", numBlocks);
	
	inputFile.close();
	exit(0);
}