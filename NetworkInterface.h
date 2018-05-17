#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <string>
#include <vector>
#include <memory>

class NetworkInterface;
using AdaptersList = std::vector<std::shared_ptr<NetworkInterface>>;

enum class NetAdapterType { Ethernet, TokenRing, FDDI, PPP, LoopBack, Slip, Other };

class NetworkInterface
{
private:
	DWORD _id;
	std::string _name;
	std::string _description;
	NetAdapterType _type = NetAdapterType::Other;
	static NetAdapterType getType(const UINT winType);
	std::string printType() const;

public:
	NetworkInterface() = delete;
	NetworkInterface(const DWORD id, const std::string& name, const std::string& description, const NetAdapterType type);
	~NetworkInterface();

	NetAdapterType getType() const { return _type; }

	std::string print(bool printAll = false) const;
	static AdaptersList loadAll();
};
