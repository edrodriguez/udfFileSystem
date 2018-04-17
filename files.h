#include <sys/stat.h>
#include <string>

using namespace std;

class UDFFile{
	public:
	
	bool accessible;
	bool readable;
	int roundedSize;
	struct timespec lastAccess;
	struct timespec modificationDate;
	string currentLocation;
	string destination;
	
	UDFFile();
};