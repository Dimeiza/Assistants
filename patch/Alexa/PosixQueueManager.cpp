#include <sstream>

#include "SampleApp/PosixQueueManager.h"
#include "SampleApp/ConsolePrinter.h"


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

	m_AssistantControlQueue = mq_open("/AssistantsControlQueue", O_RDWR| O_CREAT );
	m_AlexaQueue = mq_open("/alexa_queue", O_RDWR| O_CREAT );
    sem_init(&m_waitQueueSemaphore,0,1);

}

PosixQueueManager::~PosixQueueManager(){

    mq_close(m_AssistantControlQueue);
    mq_close(m_AlexaQueue);
    mq_unlink("/alexa_queue");
    
    sem_close(&m_waitQueueSemaphore);
}

ssize_t PosixQueueManager::receive(char *buff){

	struct mq_attr attr;
    ssize_t n;

    mq_getattr( m_AlexaQueue ,&attr );
    n = mq_receive( m_AlexaQueue, buff, attr.mq_msgsize,NULL);
	return n;

}
void PosixQueueManager::send(const char *buff,ssize_t len){
    mq_send(m_AssistantControlQueue,buff,len,0);
	return;
}

void PosixQueueManager::onDialogUXStateChanged(DialogUXState newState){

    switch (newState) {
        case DialogUXState::IDLE:
        	send(finish,strlen(finish));
        	sem_post(&m_waitQueueSemaphore);
            break;
        case DialogUXState::LISTENING:
            break;
        case DialogUXState::THINKING:
        	send(think,strlen(think));
            break;
        case DialogUXState::SPEAKING:
        	send(speak,strlen(speak));
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
           	sem_wait(&m_waitQueueSemaphore);
        }
    }
}

}  // namespace sampleApp
}  // namespace alexaClientSDK
