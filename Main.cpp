#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool input()
{
	char mode;
	std::cout << "Mode (F/T): ";
	std::cin >> mode;
	system("cls");

	if (mode == 'F' || mode == 'f')
		return false;
	if (mode == 'T' || mode == 't')
		return true;
	return input();
}

const char* hex = "0123456789ABCDEF";

struct HexSplit
{
	unsigned char l : 4;
	unsigned char f : 4;
};

union HexCNV
{
	char in;
	HexSplit hex;
};

int main()
{
	std::vector<std::string> resNames, fileNames;
	std::string resName, fileName, outStr;
	std::ifstream in;

	if (input()) // text
	{
		while (std::cin >> resName >> fileName)
		{
			if (resName == "exit" && fileName == "exit")
				break;
			resNames.push_back(resName);
			fileNames.push_back(fileName);
		}
	}
	else // file
	{
		in.open("res.txt");
		if (in.is_open())
		{
			while (in >> resName >> fileName)
			{
				resNames.push_back(resName);
				fileNames.push_back(fileName);
			}
			in.close();
		}
		else
			return 69420;
	}

	if (resNames.size())
	{
		HexCNV hexCNV;
		std::ofstream out("res.h");
		
		for (int i = 0; i < resNames.size(); i++)
		{
			in.open(fileNames[i], std::ios::binary | std::ios::ate);
			if (in.is_open())
			{
				long long size = in.tellg();
				in.seekg(0, std::ios::beg);

				char* data = new char[size];
				in.read(data, size);

				out << "unsigned long long " << resNames[i] << "_SIZE = " << size << "; unsigned char " << resNames[i] << "[] = {";
				for (long long l = 0; l < size; l++)
				{
					hexCNV.in = data[l];
					if (l)
						out << ',';
					out << "0x" << hex[hexCNV.hex.f] << hex[hexCNV.hex.l];
				}
				out << "};\n";

				delete[] data;
			}
			else
				std::cout << "Unable to open \"" << fileNames[i] << "\"\n";
		}
		out.close();
	}
}