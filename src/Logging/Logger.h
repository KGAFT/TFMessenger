//
// Created by kgaft on 5/16/24.
//

#ifndef LOGGER_H
#define LOGGER_H
#include "Threads/Thread.h"

#include <chrono>

enum MessageSeverity {
    MESSAGE_WARNING,
    MESSAGE_INFO,
    MESSAGE_VERBOSE,
    MESSAGE_ERROR
};

struct Message {
    std::string text;
    std::string source;
    std::time_t messageTime;
    MessageSeverity severity;
    bool processed = false;
};

class Logger {
private:
    static Thread loggerThread;
    static std::vector<Message*> messages;
public:
    static void initializeLogger(const char* logFileName) {

    }
};



#endif //LOGGER_H
