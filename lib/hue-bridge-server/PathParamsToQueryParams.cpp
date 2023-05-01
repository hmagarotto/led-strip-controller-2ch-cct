#include "PathParamsToQueryParams.h"

#include <vector>

PathParamsToQueryParams::PathParamsToQueryParams() :
     AsyncWebRewrite(NULL, NULL)
{
}

PathParamsToQueryParams::~PathParamsToQueryParams()
{
}

void PathParamsToQueryParams::addRule(const char* from, const char* to, WebRequestMethodComposite method) {
     RewriteRule* rule = new RewriteRule();
     rule->method = method;
     rule->from = from;
     rule->to = to;
     auto fromSize = strlen(from)+1;
     rule->buf = (char*)malloc(fromSize);
     memcpy(rule->buf, from, fromSize);
     rule->parts.reserve(6);
     char* tok = strtok(rule->buf, "/");
     while(tok) {
          rule->parts.push_back(tok);
          tok = strtok(NULL, "/");
     }
     rule->parts.shrink_to_fit();
     this->_rules.push_back(rule);
}

bool PathParamsToQueryParams::match(AsyncWebServerRequest *request) {
     String url = request->url();
     String reqParams = "";
     int index = url.indexOf('?');
     if (index > 0) {
        reqParams = _toUrl.substring(index +1);
        url = _toUrl.substring(0, index);
      }
      
      std::unique_ptr<char[]> buf(new char[url.length() + 1]);
      memcpy(buf.get(), url.c_str(), url.length() + 1);
      std::vector<const char *> parts;
      parts.reserve(6);

     char* tok = strtok(buf.get(), "/");
     while(tok) {
          parts.push_back(tok);
          tok = strtok(NULL, "/");
     }


     const RewriteRule* matchedRule = NULL;
     for (auto rule : this->_rules) {
          if (matchRule(rule, request->method(), parts)) {
               matchedRule = rule;
               break;
          }
     }

     if (!matchedRule) {
          return false;
     }
     this->_from = matchedRule->from;
     this->_toUrl = matchedRule->to;
     if (!reqParams.isEmpty()) {
          if (!this->_params.isEmpty()) this->_params += "&";
          _params += reqParams;
     }
     return true;
}

bool PathParamsToQueryParams::matchRule(const RewriteRule* rule, WebRequestMethodComposite method, std::vector<const char *>& parts) {
     // test method
     if ((rule->method & method) == 0) {
          return false;
     }
     // request shoud have same parts size
     if (rule->parts.size() != parts.size()) {
          return false;
     }
     // evaluate each part
     this->_params = "";
     size_t idx = 0;
     for (auto rulePart : rule->parts) {
          const char* part = parts.at(idx++);
          if (rulePart[0] == '{') {
               if (!this->_params.isEmpty()) this->_params += "&";
               String paramName(rulePart);
               this->_params += paramName.substring(1, paramName.length()-1);
               this->_params += "=";
               this->_params += part;
          } else if(strcmp(part, rulePart) != 0) {
               return false;
          }
     }
     return true;
}
