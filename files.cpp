#include "files.h"

UDFFile::UDFFile(){
	accessible = false;
	readable = false;
	roundedSize = 0;
	currentLocation = "";
	destination = "";
}