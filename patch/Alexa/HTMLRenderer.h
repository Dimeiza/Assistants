
#ifndef ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_HTMLRENDERER_H_
#define ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_HTMLRENDERER_H_

#include <AVSCommon/Utils/JSON/JSONUtils.h>

namespace alexaClientSDK {
namespace sampleApp {

class HTMLRenderer {

public:

    static std::string getHTML(const std::string &templateType,rapidjson::Document &payload);

private:

    static std::string RenderBodyTemplate1HTML(rapidjson::Document &payload);
    static std::string RenderBodyTemplate2HTML(rapidjson::Document &payload);
    static std::string RenderWeatherTemplateHTML(rapidjson::Document &payload);

};
}  // namespace sampleApp
}  // namespace alexaClientSDK

#endif