#ifndef ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_POSIXQUEUE_H_
#define ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_POSIXQUEUE_H_

#include <memory>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#include <AVSCommon/SDKInterfaces/DialogUXStateObserverInterface.h>

#include "InteractionManager.h"

namespace alexaClientSDK {
namespace sampleApp {

/// Observes user input from the console and notifies the interaction manager of the user's intentions.
class PosixQueueManager        : public avsCommon::sdkInterfaces::DialogUXStateObserverInterface {

public:
    static std::unique_ptr<PosixQueueManager> create(std::shared_ptr<InteractionManager> interactionManager);

	PosixQueueManager(std::shared_ptr<InteractionManager> interactionManager);
	~PosixQueueManager();

	void run() ;
    /**
     * UXDialogObserverInterface methods
     */
    void onDialogUXStateChanged(DialogUXState newState) override;

private:

    mqd_t m_AlexaQueue;
    mqd_t m_AssistantControlQueue;

	const char* finish="finish";
	const char* speak="speak";
	const char* think="think";

	ssize_t receive(char *buff);
	void send(const char *buff,ssize_t len);

	std::shared_ptr<InteractionManager> m_interactionManager;
	sem_t m_waitQueueSemaphore;

};

}  // namespace sampleApp
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_POSIXQUEUE_H_
