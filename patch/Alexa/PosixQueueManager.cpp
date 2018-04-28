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

    m_isAlreadyFinished = false;
    m_state = DialogUXState::IDLE;

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


//	switch(m_state){
//		case DialogUXState::IDLE:
//			ConsolePrinter::simplePrint("m_state:DialogUXState::IDLE");
//			break;
//		case DialogUXState::LISTENING:
//			ConsolePrinter::simplePrint("m_state:DialogUXState::LISTENING");
//			break;
//		case DialogUXState::THINKING:
//			ConsolePrinter::simplePrint("m_state:DialogUXState::THINKING");
//			break;
//		case DialogUXState::SPEAKING:
//			ConsolePrinter::simplePrint("m_state:DialogUXState::SPEAKING");
//			break;
//		case DialogUXState::FINISHED:
//			ConsolePrinter::simplePrint("m_state:DialogUXState::FINISHED");
//			break;
//	}
//
//    switch (newState) {
//    	case DialogUXState::IDLE:
//            ConsolePrinter::simplePrint("newState:DialogUXState::IDLE");
//            break;
//        case DialogUXState::LISTENING:
//            ConsolePrinter::simplePrint("newState:DialogUXState::LISTENING");
//            break;
//        case DialogUXState::THINKING:
//            ConsolePrinter::simplePrint("newState:DialogUXState::THINKING");
//            break;
//        case DialogUXState::SPEAKING:
//            ConsolePrinter::simplePrint("newState:DialogUXState::SPEAKING");
//            break;
//        case DialogUXState::FINISHED:
//            ConsolePrinter::simplePrint("newState:DialogUXState::FINISHED");
//            break;
//    }


	const char * nextMessage = getNextMesssageForAssistantController(newState);

	if(nextMessage != NULL){
        send(nextMessage,strlen(nextMessage));
	}

	m_state = newState;

}
const char* PosixQueueManager::getNextMesssageForAssistantController(DialogUXState newState){

	switch(m_state){
	case DialogUXState::IDLE:
		if(newState == DialogUXState::SPEAKING){
			m_isAlreadyFinished = true;
		}
		break;
	case DialogUXState::LISTENING:
		if(newState == DialogUXState::THINKING){
			return think;
		}
		break;
	case DialogUXState::THINKING:
		if(newState == DialogUXState::SPEAKING){
			return speak;
		}
		if(newState == DialogUXState::IDLE){
			return finish;
		}
		break;
	case DialogUXState::SPEAKING:
        if(newState == DialogUXState::IDLE && m_isAlreadyFinished != true){
            	return finish;
       	}
   		m_isAlreadyFinished = false;
		break;
    case DialogUXState::FINISHED:
    	break;
	}

	return NULL;

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
