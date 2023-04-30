#include "constants.h"

const char UDP_SEARCH_RESPONSE[] PROGMEM = 
"HTTP/1.1 200 OK\r\n"
"EXT:\r\n"
"CACHE-CONTROL: max-age=100\r\n" // SSDP_INTERVAL
"LOCATION: http://%d.%d.%d.%d:%d/description.xml\r\n"
"SERVER: FreeRTOS/6.0.5, UPnP/1.0, IpBridge/1.17.0\r\n" // _modelName, _modelNumber
"hue-bridgeid: %s\r\n"
"ST: urn:schemas-upnp-org:device:Basic:1\r\n"  // _deviceType
"USN: uuid:2f402f80-da50-11e1-9b23-%s::upnp:rootdevice\r\n" // _uuid::_deviceType
"\r\n";
const size_t UDP_SEARCH_RESPONSE_SZ = sizeof(UDP_SEARCH_RESPONSE);


const char DESCRIPTION_XML[] PROGMEM = R"(<?xml version="1.0" encoding="UTF-8" ?>
<root xmlns="urn:schemas-upnp-org:device-1-0">
<specVersion>
<major>1</major>
<minor>0</minor>
</specVersion>
<URLBase>http://%ip%:%port%/</URLBase>
<device>
<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>
<friendlyName>%name% (%ip%)</friendlyName>
<manufacturer>Signify</manufacturer>
<manufacturerURL>http://www.meethue.com</manufacturerURL>
<modelDescription>Philips hue Personal Wireless Lighting</modelDescription>
<modelName>Philips hue bridge 2015</modelName>
<modelNumber>BSB002</modelNumber>
<modelURL>http://www.meethue.com</modelURL>
<serialNumber>%serial%</serialNumber>
<UDN>uuid:2f402f80-da50-11e1-9b23-%serial%</UDN>
<presentationURL>index.html</presentationURL>
<iconList>
<icon>
<mimetype>image/png</mimetype>
<height>48</height>
<width>48</width>
<depth>24</depth>
<url>hue_logo_0.png</url>
</icon>
</iconList>
</device>
</root>)";

const char USER_RESPONSE[] PROGMEM = "[{\"success\": {\"username\": \"7fad985ede4f11edafbfb42e9910b8b3\"}}]";
