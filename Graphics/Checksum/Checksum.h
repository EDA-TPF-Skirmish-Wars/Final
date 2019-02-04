#ifndef CHECKSUM_H
#define CHECKSUM_H
#define CHKSM_ERR	-1
#include <iostream>
#include <fstream>
#include <iostream>

class Checksum
{
public:
	Checksum(std::ifstream * f);
	int getChecksum();
protected:
	int tabla[1000];
	std::ifstream * file;
	int length;
	int index(int n);
	unsigned int archivo(int n);
};

#endif
