/*
My solution to the puzzles that can be found here: https://adventofcode.com/2021/day/1
Written in C++ and compiled using Visual Studio 2017. View Readme for more info.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string CompareDepths(std::string prev, std::string curr) {
	std::string result;
	if (prev.empty()) {
		result = "N/A - no previous measurement";
	}
	else if (prev.length() != curr.length()) {
		if (prev.length() < curr.length()) { result = "increased"; }
		else { curr = "decreased"; }
	}
	else {
		for (unsigned i = 0; i < prev.length(); ++i) {
			if ((prev[i] - '0') < (curr[i] - '0')) {
				result = "increased";
				break;
			}
			else if ((prev[i] - '0') > (curr[i] - '0')) {
				result = "decreased";
				break;
			}
			else { /*do nothing*/ }
		}
	}
	return result;
}


int main(int argc, char *argv[])
{
	int depthIncrementCount = 0;
	int functionSelect;
	char *filePath;

    std::cout << "Sonar Sweep. Checking for sweep report.\n";

	if (argc == 2) {
		std::cout << "Report found. Now parsing.\n";
		filePath = argv[1];
		std::ifstream ifs (filePath, std::ifstream::in);
		if (!ifs.is_open()) { 
			std::cout << "Report file could not be opened. Please check argument.\n";
			return 0;
		}
		std::string previousDepth, depthResult;

		/*TODO: Sanitize input somehow so we only look at numbers. Use regex maybe?*/
		do
		{
			std::cout << "Please select a mode to parse measurements. Type the value corresponding to the option.\n"
				<< "1: Go through all measurements and determine elevation changes from one to the next.\n"
				<< "2: Create sums of three-measurement windows and determine elevation changes from one to the next.\n";
			std::cin >> functionSelect;
		} while (functionSelect != 1 && functionSelect != 2);

		switch (functionSelect)
		{
		case(1):
			for (std::string currentDepth; std::getline(ifs, currentDepth);) {
				depthResult = CompareDepths(previousDepth, currentDepth);
				if (depthResult == "increased") { depthIncrementCount++; }
				std::cout << currentDepth << " (" << depthResult << ")\n";

				previousDepth = currentDepth;
			}
			std::cout << "There are " << depthIncrementCount
				<< " measurements that are larger than the previous measurement.\n";
			break;

		case(2):
			//TODO: Clean this up because oh god it's ugly
			while (!ifs.eof()) {
				std::string ifsStr1, ifsStr2, ifsStr3;
				std::vector<int> sums;
				std::vector<int>::iterator sums_it1, sums_it2; //I need better names
				
				for (int firstDepth, secondDepth, thirdDepth; std::getline(ifs, ifsStr1);) {
					/*if all 3 strings have value, add 3 strings value to vector
					copy second to third, then first to second and repeat loop until eof*/
					if (!ifsStr1.empty() && !ifsStr2.empty() && !ifsStr3.empty()) {
						firstDepth = std::stoi(ifsStr1);
						secondDepth = std::stoi(ifsStr2);
						thirdDepth = std::stoi(ifsStr3);

						sums.push_back(firstDepth);
						sums.back() += secondDepth;
						sums.back() += thirdDepth;
					}
					ifsStr3 = ifsStr2;
					ifsStr2 = ifsStr1;
				}

				if (!sums.empty()) {
					sums_it1 = sums.begin();
					sums_it2 = sums_it1 + 1;
					for (unsigned i = 0; i < sums.size(); i++) {
						if (i == 0) { depthResult = "N/A - no previous sum"; }
						else if (sums[i-1] < sums[i]) { 
							depthResult = "increased";
							depthIncrementCount++;
						}
						else if (sums[i-1] > sums[i]) { depthResult = "decreased"; }
						else { depthResult = "no change"; }
						
						std::cout << sums[i] << " (" << depthResult << ")\n";
					}
					std::cout << "There are " << depthIncrementCount
						<< " sums that are larger than the previous sum.\n";
				}
				else {
					std::cout << "Not enough values to consider sums of three-measurement windows.\n"
						<< "Verify input contains at least three measurements.\n";
				}
			}
			break;

		default:
			break;
		}

		
	}

	else if (argc > 2) { std::cout << "Multiple arguments detected.\n"; }
	else { std::cout << "No report found.\n"; }

	return 0;
}