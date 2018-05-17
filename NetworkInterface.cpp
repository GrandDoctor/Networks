#include <sstream>
#include <iostream>

#include "NetworkInterface.h"
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

NetworkInterface::NetworkInterface(const DWORD id, const std::string& name, const std::string& description, const NetAdapterType type)
{
	_id = id;
	_name = name;
	_description = description;
	_type = type;
}

NetworkInterface::~NetworkInterface()
{
}

std::string NetworkInterface::print(bool printAll) const
{
	std::stringstream out;

	out << "ID: " << std::to_string(_id) << std::endl;
	out << "Name: " << _name << std::endl;

	if (printAll) {
		out << "Description: " << _description << std::endl;
		out << "Type: " << printType() << std::endl;
	}

	return out.str();
}

NetAdapterType NetworkInterface::getType(const UINT winType)
{
	switch (winType) {
	case MIB_IF_TYPE_OTHER:
		return NetAdapterType::Other;
		break;
	case MIB_IF_TYPE_ETHERNET:
		return NetAdapterType::Ethernet;
		break;
	case MIB_IF_TYPE_TOKENRING:
		return NetAdapterType::TokenRing;
		break;
	case MIB_IF_TYPE_FDDI:
		return NetAdapterType::FDDI;
		break;
	case MIB_IF_TYPE_PPP:
		return NetAdapterType::PPP;
		break;
	case MIB_IF_TYPE_LOOPBACK:
		return NetAdapterType::LoopBack;
		break;
	case MIB_IF_TYPE_SLIP:
		return NetAdapterType::Slip;
		break;
	default:
		return NetAdapterType::Other;
		break;
	}
}

std::string NetworkInterface::printType() const
{
	switch (_type) {
	case NetAdapterType::Ethernet:
		return "Ethernet";
		break;
	case NetAdapterType::TokenRing:
		return "Token ring";
		break;
	case NetAdapterType::FDDI:
		return "FDDI";
		break;
	case NetAdapterType::PPP:
		return "PPP";
		break;
	case NetAdapterType::LoopBack:
		return "Loopback";
		break;
	case NetAdapterType::Slip:
		return "Slip";
		break;
	default:
		return "Other";
		break;
	}
}

AdaptersList NetworkInterface::loadAll()
{
	AdaptersList list;

	PIP_ADAPTER_INFO adaptersInfo;
	ULONG outBufLen = sizeof(IP_ADAPTER_INFO);
	adaptersInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (adaptersInfo == NULL) {
		std::cout << "Memory allocation error" << std::endl;
		return list;
	}

	if (GetAdaptersInfo(adaptersInfo, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(adaptersInfo);
		adaptersInfo = (IP_ADAPTER_INFO *)MALLOC(outBufLen);
		if (adaptersInfo == NULL) {
			std::cout << "Memory allocation error" << std::endl;
			return list;
		}
	}

	PIP_ADAPTER_INFO currentAdapter = NULL;
	if (GetAdaptersInfo(adaptersInfo, &outBufLen) == NO_ERROR) {
		currentAdapter = adaptersInfo;
		while (currentAdapter) {
			list.emplace_back(std::make_shared<NetworkInterface>(
				currentAdapter->ComboIndex, 
				currentAdapter->AdapterName, 
				currentAdapter->Description, 
				getType(currentAdapter->Type))
			);
			currentAdapter = currentAdapter->Next;
		}
	}
	else {
		std::cout << "GetAdaptersInfo failed" << std::endl;
	}

	if (adaptersInfo) {
		FREE(adaptersInfo);
	}

	return list;
}
