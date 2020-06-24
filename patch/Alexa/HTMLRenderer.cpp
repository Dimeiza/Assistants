#include <sstream>

#include "SampleApp/HTMLRenderer.h"
#include "SampleApp/ConsolePrinter.h"
#include <errno.h>
#include <vector>
#include <set>
#include <fstream>

namespace alexaClientSDK {
namespace sampleApp {
using namespace avsCommon::utils::json;

static const std::string HTML_HEADER = 
    "<html>\n"
    "<head><meta charset=\"utf-8\"></head>\n"
    "<body bgcolor=\"white\">";

static const std::string HTML_FOOTER = 
    "</body>\n"
    "</html>\n";

static const std::string ALEXA_LOGO_URL = 
"https://images-na.ssl-images-amazon.com/images/G/01/mobile-apps/dex/avs/docs/ux/branding/mark3._TTH_.png";

/// Tag for find the title node in the payload of the RenderTemplate directive
static const std::string TITLE_NODE("title");


/// Tag for find the maintitle in the title node of the RenderTemplate directive
static const std::string MAIN_TITLE_TAG("mainTitle");


void HTMLRenderer::outputHTMLToFile(const std::string &templateType,rapidjson::Document &payload){

    std::string htmlContents =HTMLRenderer::renderHTML(templateType,payload);

    std::ofstream HTMLFile("/var/tmp/Assistants.html");
    HTMLFile << htmlContents;
    HTMLFile.close();

}

std::string HTMLRenderer::renderHTML(const std::string &templateType,rapidjson::Document &payload){

    std::string htmlContents= "";
    std::string body;

    if(templateType == "BodyTemplate1"){
        body = renderBodyTemplate1HTML(payload);
    }
    else if(templateType == "BodyTemplate2"){
        body = renderBodyTemplate2HTML(payload);
    }
    else if(templateType == "WeatherTemplate"){
        body = renderWeatherTemplateHTML(payload);
    }
    else{
        body = "Sorry, this template is not supported.";
    }

    htmlContents += HTML_HEADER;
    htmlContents += body;
    htmlContents += HTML_FOOTER;
 
    return htmlContents;

}

struct weatherStr{
    std::string imageURL;
    std::string day;
    std::string date;
    std::string highTemperature;
    std::string lowTemperature;
};


std::string HTMLRenderer::renderWeatherTemplateHTML(rapidjson::Document &payload){

    std::vector<weatherStr> forecastStrs;

    rapidjson::Value::ConstMemberIterator titleNode;
    if (!jsonUtils::findNode(payload, TITLE_NODE, &titleNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no title field");
    }

    std::string mainTitle = "";
    if (!jsonUtils::retrieveValue(titleNode->value, MAIN_TITLE_TAG, &mainTitle)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no main title field");
    }

    std::string subTitle= "";
    if (!jsonUtils::retrieveValue(titleNode->value, "subTitle", &subTitle)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no main title field");
    }

    std::string currentWeather= "";
    if (!jsonUtils::retrieveValue(payload, std::string("currentWeather"), &currentWeather)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no text field");
    }

    std::string description= "";
    if (!jsonUtils::retrieveValue(payload, std::string("description"), &description)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no text field");
    }

    rapidjson::Value::ConstMemberIterator currentWeatherIcon;
    if (!jsonUtils::findNode(payload, std::string("currentWeatherIcon"), &currentWeatherIcon)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }

    rapidjson::Value::ConstMemberIterator lowTemperature;
    if (!jsonUtils::findNode(payload, "lowTemperature", &lowTemperature)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no title field");
    }

    std::string lowTemperatureValue;
    if (!jsonUtils::retrieveValue(lowTemperature->value, std::string("value"), &lowTemperatureValue)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no text field");
    }

    rapidjson::Value::ConstMemberIterator lowTemperatureImage;
    if (!jsonUtils::findNode(lowTemperature->value, std::string("arrow"), &lowTemperatureImage)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }

    rapidjson::Value::ConstMemberIterator lowTemperatureImageSourceNode;
    if (!jsonUtils::findNode(lowTemperatureImage->value, std::string("sources"), &lowTemperatureImageSourceNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }


    std::string lowTemperatureArrow= "";
    if (!jsonUtils::retrieveValue(lowTemperatureImageSourceNode->value[0], std::string("url"), &lowTemperatureArrow)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no url field");
    }


    rapidjson::Value::ConstMemberIterator highTemperature;
    if (!jsonUtils::findNode(payload, "highTemperature", &highTemperature)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no title field");
    }

    std::string highTemperatureValue= "";
    if (!jsonUtils::retrieveValue(highTemperature->value, std::string("value"), &highTemperatureValue)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no text field");
    }

    rapidjson::Value::ConstMemberIterator highTemperatureImage;
    if (!jsonUtils::findNode(highTemperature->value, std::string("arrow"), &highTemperatureImage)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }

    rapidjson::Value::ConstMemberIterator highTemperatureImageSourceNode;
    if (!jsonUtils::findNode(highTemperatureImage->value, std::string("sources"), &highTemperatureImageSourceNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }

    std::string highTemperatureArrow= "";
    if (!jsonUtils::retrieveValue(highTemperatureImageSourceNode->value[0], std::string("url"), &highTemperatureArrow)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no url field");
    }


    rapidjson::Value::ConstMemberIterator imageSourceNode;
    if (!jsonUtils::findNode(currentWeatherIcon->value, std::string("sources"), &imageSourceNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }

    std::string currentWeatherIconUrl= "";
    if (!jsonUtils::retrieveValue(imageSourceNode->value[0], std::string("url"), &currentWeatherIconUrl)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no url field");
    }

    rapidjson::Value::ConstMemberIterator weatherForecast;
    if (!jsonUtils::findNode(payload, "weatherForecast", &weatherForecast)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no weatherForecast field");
    }

    weatherStr weather;
    for (rapidjson::Value::ConstValueIterator weatherit = weatherForecast->value.Begin();
        weatherit != weatherForecast->value.End();weatherit++){
        
        rapidjson::Value::ConstMemberIterator forecastImageNode;
        if (!jsonUtils::findNode(*weatherit, "image", &forecastImageNode)) {
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no forecastImageNode field");
            break;
        }

        rapidjson::Value::ConstMemberIterator forecastImageSourceNode;
        if (!jsonUtils::findNode(forecastImageNode->value, "sources", &forecastImageSourceNode)) {
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no forecastImageSourceNode field");
            break;
        }

        if (!jsonUtils::retrieveValue(forecastImageSourceNode->value[0], std::string("url"), &weather.imageURL) ){
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageURL field");
            break;
        }
        
        if (!jsonUtils::retrieveValue(*weatherit, std::string("day"), &(weather.day))) {
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no day field");
            break;
        }
        if (!jsonUtils::retrieveValue(*weatherit, std::string("date"), &(weather.date))) {
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no date field");
            break;
        }
        if (!jsonUtils::retrieveValue(*weatherit, std::string("highTemperature"), &(weather.highTemperature))) {
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no highTemperature field");
            break;
        }

        if (!jsonUtils::retrieveValue(*weatherit, std::string("lowTemperature"), &(weather.lowTemperature))) {
            ConsolePrinter::simplePrint("ERROR: Template JSON payload has no lowTemperature field");
            break;
        }
        forecastStrs.push_back(weather);
    }

    std::string WeatherTemplateHTML;

    WeatherTemplateHTML += "<h1>" + mainTitle + "</h1></br>";

    WeatherTemplateHTML += "<table>";
    WeatherTemplateHTML += "<tr><td>";
    WeatherTemplateHTML += subTitle;
    WeatherTemplateHTML += "</td></tr>";
    WeatherTemplateHTML += "<tr><td>";
    WeatherTemplateHTML += "<img src=\"" + currentWeatherIconUrl + "\"/>";
    WeatherTemplateHTML += "</td><td>";
    WeatherTemplateHTML += description + "<br>";
    WeatherTemplateHTML += currentWeather + "<br>";
    WeatherTemplateHTML += "</td><td>";
    WeatherTemplateHTML += "<img src=\"" + highTemperatureArrow + "\"/>";
    WeatherTemplateHTML += highTemperatureValue;
    WeatherTemplateHTML += "<img src=\"" + lowTemperatureArrow + "\"/>";
    WeatherTemplateHTML += lowTemperatureValue + "<br>";
    WeatherTemplateHTML += "</td></tr>";
    WeatherTemplateHTML += "</table>";

    WeatherTemplateHTML += "<table>";
    WeatherTemplateHTML += "<tr>";
    for(unsigned int i = 0;i < forecastStrs.size();i++){
        
        WeatherTemplateHTML += "<td>";
        WeatherTemplateHTML += forecastStrs[i].date;
        WeatherTemplateHTML += "<br>";
        WeatherTemplateHTML += forecastStrs[i].day;
        WeatherTemplateHTML += "</td>";
    }
    WeatherTemplateHTML += "</tr>";
    WeatherTemplateHTML += "<tr>";
    for(unsigned int i = 0;i < forecastStrs.size();i++){
        
        WeatherTemplateHTML += "<td>";
        WeatherTemplateHTML += "<img src=\"" + forecastStrs[i].imageURL + "\"/>";
        WeatherTemplateHTML += forecastStrs[i].highTemperature;
        WeatherTemplateHTML += "/";
        WeatherTemplateHTML += forecastStrs[i].lowTemperature;
        WeatherTemplateHTML += "</td>";
    }

    WeatherTemplateHTML += "</tr>";
    WeatherTemplateHTML += "</table>";

    WeatherTemplateHTML += "<div align=\"right\">";
    WeatherTemplateHTML += "<img src=";
    WeatherTemplateHTML += "\"";
    WeatherTemplateHTML += ALEXA_LOGO_URL;
    WeatherTemplateHTML += "\"";
    WeatherTemplateHTML += " />";
    WeatherTemplateHTML += "</div>";

    return WeatherTemplateHTML;

}

std::string HTMLRenderer::renderBodyTemplate1HTML(rapidjson::Document &payload){


    rapidjson::Value::ConstMemberIterator titleNode;
    if (!jsonUtils::findNode(payload, TITLE_NODE, &titleNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no title field");
    }

    std::string mainTitle;
    if (!jsonUtils::retrieveValue(titleNode->value, MAIN_TITLE_TAG, &mainTitle)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no main title field");
    }

    std::string textField = "";
    if (!jsonUtils::retrieveValue(payload, std::string("textField"), &textField)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no text field");
    }
    std::string BodyTemplate1HTML;

    BodyTemplate1HTML += "<h1>" + mainTitle + "</h1></br>";
    BodyTemplate1HTML += "<table>";
    BodyTemplate1HTML += "<tr><td>";
    BodyTemplate1HTML += textField + "<br>";
    BodyTemplate1HTML += "</td></tr>";
    BodyTemplate1HTML += "</table>";
    BodyTemplate1HTML += "<div align=\"right\">";
    BodyTemplate1HTML += "<img src=";
    BodyTemplate1HTML += "\"";
    BodyTemplate1HTML += ALEXA_LOGO_URL;
    BodyTemplate1HTML += "\"";
    BodyTemplate1HTML += " />";
    BodyTemplate1HTML += "</div>";

    return BodyTemplate1HTML;
}



std::string HTMLRenderer::renderBodyTemplate2HTML(rapidjson::Document &payload){

    std::string BodyTemplate2HTML = "";


    rapidjson::Value::ConstMemberIterator titleNode;
    if (!jsonUtils::findNode(payload, TITLE_NODE, &titleNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no title field");
    }

    std::string mainTitle= "";
    if (!jsonUtils::retrieveValue(titleNode->value, MAIN_TITLE_TAG, &mainTitle)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no main title field");
    }

    std::string textField= "";
    if (!jsonUtils::retrieveValue(payload, std::string("textField"), &textField)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no text field");
    }

    rapidjson::Value::ConstMemberIterator imageNode;
    if (!jsonUtils::findNode(payload, std::string("image"), &imageNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no image field");
    }

    rapidjson::Value::ConstMemberIterator imageSourceNode;
    if (!jsonUtils::findNode(imageNode->value, std::string("sources"), &imageSourceNode)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no imageSource field");
    }

    std::string url= "";
    if (!jsonUtils::retrieveValue(imageSourceNode->value[0], std::string("url"), &url)) {
        ConsolePrinter::simplePrint("ERROR: Template JSON payload has no url field");
    }


    BodyTemplate2HTML += "<h1>" + mainTitle + "</h1></br>";
    BodyTemplate2HTML += "<table>";
    BodyTemplate2HTML += "<tr><td>";
    BodyTemplate2HTML += textField + "<br>";
    BodyTemplate2HTML += "</td></tr>";
    BodyTemplate2HTML += "<tr><td>";
    BodyTemplate2HTML += "<img src=\"" + url + "\"/>";
    BodyTemplate2HTML += "</td>";
    BodyTemplate2HTML += "</tr>";
    BodyTemplate2HTML += "</table>";
    BodyTemplate2HTML += "<div align=\"right\">";
    BodyTemplate2HTML += "<img src=";
    BodyTemplate2HTML += "\"";
    BodyTemplate2HTML += ALEXA_LOGO_URL;
    BodyTemplate2HTML += "\"";
    BodyTemplate2HTML += " />";
    BodyTemplate2HTML += "</div>";

    return BodyTemplate2HTML;
}


}}
