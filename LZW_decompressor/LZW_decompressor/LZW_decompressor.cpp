// LZW Decompressor - Jonathan Page
// Decompresses files compressed by an LZW algorithm

#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main()
{
	// Declare and initialize required variables
	ifstream toDecomp;				// Input fstream object for reading the compressed file
	ofstream outfile;				// Output file to write decompressed text
	string decomp = "";				// String to contain decompressed text
	map<int, string> codeDiction;	// Dictionary for code-string combinations
	int newEntry = 256;				// Index counter for new dictionary entries
	char three_bytes[3];			// Character array to store bytes from file, three at a time
	unsigned int twelveBit[2];		// Array to store 12-bit sequences, two at a time
	int readpos = 0;				// Read position input file
	string conject;					// Conjecture for new dictionary entries
	string conject_temp1;			// Conjecture element 1
	string conject_temp2;			// Conjecture element 2

									// Read in file in binary format
	toDecomp.open("C:/Users/jonny/Documents/Starleaf_exercise/LzwInputData/compressedfile4.z", ios::binary);


	// Initialize dictionary with 256 decimal-ASCII character pairs
	for (int i = 0; i <= 255; i++)
	{
		codeDiction[i] = static_cast<char>(i);
	}

	// Find the size of the file
	toDecomp.seekg(0, toDecomp.end);
	int length = toDecomp.tellg();
	toDecomp.seekg(0, toDecomp.beg);

	// Do first iteration of decompression algorithm
	toDecomp.read(three_bytes, 3); // Read three bytes of data from the input file and store in three_bytes array

	twelveBit[0] = (three_bytes[0] << 4) + ((three_bytes[1] & 0xF0) >> 4); // Take only the first 4 characters from the second byte - the Hex values extract only the the first 4 elements of the second byte
	twelveBit[1] = ((three_bytes[1] & 0X0F) << 8) + three_bytes[2]; // Take only the final 4 characters from the second byte - the Hex values extract only the the final 4 elements of the second byte

																	// Add new entries to the final decompressed string
	decomp = decomp + codeDiction[twelveBit[0]] + codeDiction[twelveBit[1]];

	// Allocate conjecture components
	conject_temp1 = codeDiction[twelveBit[0]];
	conject_temp2 = codeDiction[twelveBit[1]];

	conject = conject_temp1 + conject_temp2[0]; // Construct conjecture

	codeDiction[newEntry] = conject; // Add the conjecture to the dictionary

	newEntry++; // Increment newEntry counter

	conject_temp1 = conject_temp2; // change conjecture array such that the second element is now the first

								   // Push read position further along by three bytes
	readpos = readpos + 3;

	// Continue algorithm
	// Continue loop for as long as there is still more data to read
	while (readpos < length)
	{
		toDecomp.seekg(readpos); // Set read position in file to extract bytes
		toDecomp.read(three_bytes, 3); // Read three bytes of data from the input file and store in three_bytes array

									   // Extract two 12-bit codes from three 8-bit bytes, shifting the binary codes as necessary
		twelveBit[0] = (three_bytes[0] << 4) + ((three_bytes[1] & 0xF0) >> 4); // Take only the first 4 characters from the second byte
		twelveBit[1] = ((three_bytes[1] & 0X0F) << 8) + three_bytes[2]; // Take only the final 4 characters from the second byte

																		// Find the dictionary entry for the code and add it to the decompressed string;
		decomp = decomp + codeDiction[twelveBit[0]];

		// Construct the conjecture and add it to the dictionary
		conject_temp2 = codeDiction[twelveBit[0]];
		conject = conject_temp1 + conject_temp2[0];
		codeDiction[newEntry] = conject;
		newEntry++;  // Increment entry counter
		conject_temp1 = conject_temp2; // change the conjecture array such that the second element is now the first

									   // If all the possible entries become used, clear all entries past the initial 256 (i.e. indices 0-255) and change the entry counter back to 256
		if (newEntry == 4096)
		{
			for (int i = 256; i <= 4096; i++)
			{
				codeDiction.erase(i);
			}
			newEntry = 256;
		}

		// Now repeat for the second code obtained from this byte read
		decomp = decomp + codeDiction[twelveBit[1]];
		conject_temp2 = codeDiction[twelveBit[1]];
		conject = conject_temp1 + conject_temp2[0];
		codeDiction[newEntry] = conject;
		newEntry++;
		conject_temp1 = conject_temp2;

		if (newEntry == 4096)
		{
			for (int i = 256; i < 4097; i++)
			{
				codeDiction.erase(i);
			}
			newEntry = 256;
		}

		// Push read position along by three
		readpos = readpos + 3;

	}

	// Close file
	toDecomp.close();

	// Save to a text file
	outfile.open("C:/Users/jonny/Documents/Starleaf_exercise/LzwOutputData/decompressedfile_4.txt");
	outfile << decomp;
	outfile.close();

	return 0;
}