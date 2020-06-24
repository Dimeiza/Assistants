
#ifndef ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_HTMLRENDERER_H_
#define ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_HTMLRENDERER_H_

#include <AVSCommon/Utils/JSON/JSONUtils.h>

namespace alexaClientSDK {
namespace sampleApp {

class HTMLRenderer {

public:

    static void outputHTMLToFile(const std::string &templateType,rapidjson::Document &payload);
    static std::string renderHTML(const std::string &templateType,rapidjson::Document &payload);

private:

    static std::string renderBodyTemplate1HTML(rapidjson::Document &payload);
    static std::string renderBodyTemplate2HTML(rapidjson::Document &payload);
    static std::string renderWeatherTemplateHTML(rapidjson::Document &payload);

};
}  // namespace sampleApp
}  // namespace alexaClientSDK

#endif