#include "pch.h"
#include "ProxyParser.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include "StringUtility.h"

using namespace std;

CProxyParser::CProxyParser(void)
{
}

CProxyParser::~CProxyParser(void)
{
}

bool CProxyParser::Skipped(const std::string& url)
{
	if (url.find("localhost") != std::string::npos)
		return true;
	if (url.find("127.0.0.1") != std::string::npos)
		return true;
	return false;
}

bool CProxyParser::ValidateProxyUrl(const std::string& url)
{
	//example: http://example.com:8888 ; https://example.com:8888 ; ftp://example.com:8888 ; socks://example.com:8888
	//std::regex urlRegex(R"(^([a-zA-Z]+)://([^/:]+)(:\d+)?(/.*)?$)");

	//example: http://example.com:8888 ; https://example.com:8888 ; ftp://example.com:8888 ; socks://example.com:8888 ; example.com:8888 ; 192.168.82.333:8888
	std::regex urlRegex(R"(^(([a-zA-Z]+)://)?([^/:]+)(:\d+)?(/.*)?$)");

	return std::regex_match(url, urlRegex);
}

ProxySettings CProxyParser::ParseWindowsProxyString(const std::string& proxyStr)
{
	ProxySettings settings;
	vector<string> proxies = CStringUtility::SplitString(proxyStr, ';');
	for (const auto& token : proxies)
	{
		if (token == "<local>") {
			settings.Exceptions.insert(settings.Exceptions.end(), { "localhost", "127.*", "[::1]" });
		}
		else if (token.find('=') != std::string::npos) {
			size_t eqPos = token.find('=');
			std::string protocol = token.substr(0, eqPos);
			std::string proxy = token.substr(eqPos + 1);

			//if (Skipped(proxy))
			//	continue;

			std::string protocolLower = protocol;
			std::transform(protocolLower.begin(), protocolLower.end(), protocolLower.begin(), ::tolower);

			if (ValidateProxyUrl(proxy)) {
				if (protocolLower == "http" && settings.DefaultProxy.empty()) {
					settings.DefaultProxy = proxy;
				}
				settings.ProtocolProxies[protocolLower] = proxy;
			}
		}
		else if (!token.empty()) {
			//settings.Exceptions.push_back(token);
			// Check whether it is a simple proxy address（如 "192.168.80.222:3128"）
			if (ValidateProxyUrl(token)) {
				// Set as default proxy
				if (settings.DefaultProxy.empty()) {
					settings.DefaultProxy = token;
				}
				// You can also set this proxy for common protocols
				if (settings.ProtocolProxies.find("http") == settings.ProtocolProxies.end()) {
					settings.ProtocolProxies["http"] = token;
				}
				if (settings.ProtocolProxies.find("https") == settings.ProtocolProxies.end()) {
					settings.ProtocolProxies["https"] = token;
				}
			}
			else {
				// If it is not a valid proxy URL, treat it as an exception
				settings.Exceptions.push_back(token);
			}
		}
	}

	return settings;
}

std::string CProxyParser::GetProxyForProtocol(const ProxySettings& settings, const std::string& protocol) {
	std::string protocolLower = protocol;
	std::transform(protocolLower.begin(), protocolLower.end(), protocolLower.begin(), ::tolower);

	auto it = settings.ProtocolProxies.find(protocolLower);
	if (it != settings.ProtocolProxies.end()) {
		return it->second;
	}

	return settings.DefaultProxy;
}

bool CProxyParser::ShouldBypassProxy(const ProxySettings& settings, const std::string& host)
{
	// Check exception list
	for (const auto& pattern : settings.Exceptions) {
		// Simple implementation should actually support more complex pattern matching
		if (pattern.find('*') != std::string::npos) {
			// Simple wildcard matching
			std::string regexPattern = std::regex_replace(pattern, std::regex("\\*"), ".*");
			std::regex re(regexPattern, std::regex_constants::icase);
			if (std::regex_match(host, re)) {
				return true;
			}
		}
		else if (host == pattern) {
			return true;
		}
	}
	return false;
}