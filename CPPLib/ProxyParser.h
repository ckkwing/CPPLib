#pragma once

#include <string>
#include <map>
#include <vector>

struct ProxySettings {
    std::map<std::string, std::string> ProtocolProxies;
    std::vector<std::string> Exceptions;
    std::string DefaultProxy;
};

class CProxyParser
{
public:
    CProxyParser(void);
    ~CProxyParser(void);

    bool Skipped(const std::string& url);
    bool ValidateProxyUrl(const std::string& url);
    ProxySettings ParseWindowsProxyString(const std::string& proxyStr);
    std::string GetProxyForProtocol(const ProxySettings& settings, const std::string& protocol);
    bool ShouldBypassProxy(const ProxySettings& settings, const std::string& host);
};

//Here is an example

//std::string proxyStr = "http=http://proxy.example.com:8080;https=https://secure-proxy.example.com:8443;ftp=ftp://ftp-proxy.example.com:21;<local>;*.internal.example.com;test.domain";
//
//ProxySettings settings = ParseWindowsProxyString(proxyStr);
//
//std::cout << "Default Proxy: " << settings.defaultProxy << "\n\n";
//
//std::cout << "Protocol Proxies:\n";
//for (const auto& pair : settings.protocolProxies) {
//    std::cout << "  " << pair.first << " -> " << pair.second << "\n";
//}
//
//std::cout << "\nExceptions:\n";
//for (const auto& exception : settings.exceptions) {
//    std::cout << "  " << exception << "\n";
//}
//
//// Test for gettting proxy
//std::cout << "\nProxy for HTTPS: " << GetProxyForProtocol(settings, "https") << "\n";
//std::cout << "Proxy for FTP: " << GetProxyForProtocol(settings, "ftp") << "\n";
//std::cout << "Proxy for WS: " << GetProxyForProtocol(settings, "ws") << "\n"; // use default
//
//// Test for exceptions
//std::cout << "\nBypass check:\n";
//std::cout << "  localhost: " << (ShouldBypassProxy(settings, "localhost") ? "Bypass" : "Use proxy") << "\n";
//std::cout << "  server.internal.example.com: " << (ShouldBypassProxy(settings, "server.internal.example.com") ? "Bypass" : "Use proxy") << "\n";
//std::cout << "  test.domain: " << (ShouldBypassProxy(settings, "test.domain") ? "Bypass" : "Use proxy") << "\n";
//std::cout << "  external.com: " << (ShouldBypassProxy(settings, "external.com") ? "Bypass" : "Use proxy") << "\n";

