#include <sstream>

#include "SampleApp/PosixQueueManager.h"
#include "SampleApp/ConsolePrinter.h"
#include <errno.h>

namespace alexaClientSDK {
namespace sampleApp {

std::unique_ptr<PosixQueueManager> PosixQueueManager::create(std::shared_ptr<InteractionManager> interactionManager) {
    if (!interactionManager) {
        ConsolePrinter::simplePrint("Invalid InteractionManager passed to PosixQueueManager");
        return nullptr;
    }
    return std::unique_ptr<PosixQueueManager>(new PosixQueueManager(interactionManager));
}

PosixQueueManager::PosixQueueManager(std::shared_ptr<InteractionManager> interactionManager) : m_interactionManager{interactionManager} {

	m_AssistantControlQueue = mq_open("/AssistantsControlQueue",O_RDWR | O_CREAT , S_IRUSR | S_IWUSR ,NULL);
    
    if(m_AssistantControlQueue == -1){
        ConsolePrinter::simplePrint("MessageQueue open error.");
    }
	m_AlexaQueue = mq_open("/AlexaQueue",O_RDWR| O_CREAT ,S_IRUSR | S_IWUSR , NULL);
    if(m_AlexaQueue == -1){
        ConsolePrinter::simplePrint("MessageQueue open error.");
    }

    m_previousState = DialogUXState::IDLE;

}

PosixQueueManager::~PosixQueueManager(){

    mq_close(m_AssistantControlQueue);
    mq_close(m_AlexaQueue);
    mq_unlink("/AlexaQueue");
    
}

ssize_t PosixQueueManager::receive(char *buff){

	struct mq_attr attr;
    ssize_t n;

    mq_getattr( m_AlexaQueue ,&attr );
    
    n = mq_receive( m_AlexaQueue, buff, attr.mq_msgsize,NULL);
    if(n == -1){
         ConsolePrinter::simplePrint("MessageQueue receive error.");
    }
    return n;

}
void PosixQueueManager::send(const char *buff,ssize_t len){
    
    mq_send(m_AssistantControlQueue,buff,len,0);
	return;
}

void PosixQueueManager::onDialogUXStateChanged(DialogUXState newState){

    switch (newState) {
        case DialogUXState::IDLE:
            if(m_previousState != DialogUXState::IDLE){
                send(finish,strlen(finish));
            }
            m_previousState = newState;
            break;
        case DialogUXState::LISTENING:
            m_previousState = newState;
            break;
        case DialogUXState::THINKING:
        	send(think,strlen(think));
            m_previousState = newState;
            break;
        case DialogUXState::SPEAKING:
         	send(speak,strlen(speak));
            m_previousState = newState;
            break;
        case DialogUXState::FINISHED:
            break;
    }
    
}

void PosixQueueManager::run() {

	char buff[256] = {0};
	while (true) {

	    receive(buff);
	    if(!strcmp(buff,"wakeup")){
        	m_interactionManager->tap();
        }
    }
}

}  // namespace sampleApp
}  // namespace alexaClientSDK
