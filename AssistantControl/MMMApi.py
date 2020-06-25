import requests
from time import  sleep
assistantData = [['Alexa','https://images-na.ssl-images-amazon.com/images/G/01/mobile-apps/dex/avs/docs/ux/branding/mark1._TTH_.png'],
['Google%20Assistant','https://developers.google.com/assistant/images/badges/XPM_BADGING_GoogleAssistant_VER.png']]

class MMMApi:

    def showAlert(self,title,message,imgUrl):
        postRequestURL = 'http://localhost:8080/api/v1/modules/alert/SHOW_ALERT?title=' + title + '&message=' + message + '&imageUrl=' + imgUrl
        try:
            response = requests.post(postRequestURL)
        except requests.exceptions.RequestException:
            # print("Magic Mirror is not connected.")
            return False
        print(response.text)
        return True

    def hideAlert(self):
        postRequestURL = 'http://localhost:8080/api/v1/modules/alert/HIDE_ALERT'
        try:
            response = requests.post(postRequestURL)
        except requests.exceptions.RequestException:
            # print("Magic Mirror is not connected.")
            return False

        print(response.text)
        return True

    def wakeup(self,assistantNo):
        message = "Please%20speak%20your%20request."
        return self.showAlert(assistantData[assistantNo][0] ,message,assistantData[assistantNo][1])


    def think(self,assistantNo):
        message = "thinking..."
        return self.showAlert(assistantData[assistantNo][0] ,message,assistantData[assistantNo][1])
        
    def speak(self,assistantNo):
        sleep(2)
        getRequestURL = 'http://localhost:8080/remote?action=NOTIFICATION&notification=SWD_URL&payload={"update":"0"}'
        
        try:
            response = requests.get(getRequestURL)
        except requests.exceptions.RequestException:
            # print("Magic Mirror is not connected.")
            return False
        return True

    def finish(self,assistantNo):
        pass

    def off(self):
        self.hideAlert()