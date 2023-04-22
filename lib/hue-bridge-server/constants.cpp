#include "constants.h"


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
