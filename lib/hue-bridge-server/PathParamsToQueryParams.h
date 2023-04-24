#ifndef PATHPARAMSTOQUERYPARAMS_H
#define PATHPARAMSTOQUERYPARAMS_H

#pragma once

#include <array>
#include <ESPAsyncWebServer.h>

class PathParamsToQueryParams : public AsyncWebRewrite
{
public:
    PathParamsToQueryParams();
    ~PathParamsToQueryParams();
    virtual bool match(AsyncWebServerRequest *request);
    void addRule(const char* from, const char* to, WebRequestMethodComposite method);

private:
    struct RewriteRule {
        WebRequestMethodComposite method;
        const char* from;
        const char* to;
        char* buf;
        std::vector<const char*> parts;    
    };

    bool matchRule(const RewriteRule* rule, WebRequestMethodComposite method, std::vector<const char *>& parts);
    std::vector<const RewriteRule*> _rules;
};

#endif