#include <stdio.h>
#include <tchar.h>
#include <numeric>
#include <algorithm>
#include <iostream>

#include "NetworkInterface.h"

int main(int argc, char* argv[])
{
	bool fullInfo = false;
	bool ethernetOnly = false;

	for (auto i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "/e") {
			ethernetOnly = true;
		}
		else if (std::string(argv[i]) == "/f") {
			fullInfo = true;
		}
	}

	auto adapters = NetworkInterface::loadAll();

	std::for_each(adapters.begin(), adapters.end(), [fullInfo, ethernetOnly](auto element) {
		if (!ethernetOnly || (ethernetOnly && element->getType() == NetAdapterType::Ethernet)) {
			std::cout << element->print(fullInfo) << std::endl;
		}
	});

	char c;
	std::cin >> c;

	return 0;
}

