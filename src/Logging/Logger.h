//
// Created by kgaft on 5/16/24.
//

#ifndef LOGGER_H
#define LOGGER_H
#include "Threads/Thread.h"

#include <chrono>
#include <fstream>

enum MessageSeverity {
    MESSAGE_WARNING,
    MESSAGE_INFO,
    MESSAGE_VERBOSE,
    MESSAGE_ERROR,

};

struct Message {
    std::string text;
    std::string source;
    std::time_t messageTime;
    MessageSeverity severity;
};

class Logger {
private:
    static inline Thread loggerThread = Thread();
    static inline std::vector<Message*> messages = std::vector<Message*>();
    static inline uint32_t releasedMessageCount = 0;
    static inline char errbuf[2048];
public:
    static void initializeLogger(const char* logFileName) {
        std::ofstream* outputFile = nullptr;
        if(logFileName) {
            outputFile = new std::ofstream(logFileName);
        }
        loggerThread.run(loggerMain, outputFile);
    }
    static void logMessage(Message* message) {

        messages.push_back(message);
    }
    static std::string severityToString(MessageSeverity severity) {
        switch (severity) {
            case MESSAGE_WARNING:
                return "WARNING";
               
            case MESSAGE_INFO:
                return "INFO";
               
            case MESSAGE_VERBOSE:
                return "VERBOSE";
               
            case MESSAGE_ERROR:
                return "ERROR";
               
        }
        throw std::runtime_error("Unknown severity");
    }
    static std::time_t currentTime() {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }
    static void logAPRError(apr_status_t status) {
        char* error = apr_strerror(status, errbuf, sizeof(errbuf));
        if(error) {
            logMessage(new Message{std::string(error), "ApachePortableRuntime", Logger::currentTime(), MESSAGE_ERROR});
        }
    }
private:
    static void loggerMain(void* data) {
        auto* outputFile = (std::ofstream*)data;
        while(1) {
            for (uint32_t i = 0; i<messages.size(); i++) {
                if(messages[i]) {
                    printMessage(messages[i], outputFile);
                    delete messages[i];
                    messages[i] = nullptr;
                    releasedMessageCount++;
                }
            }
            if(releasedMessageCount>20) {
                releasedMessageCount = 0;
                messages.clear();
            }
        }
    }
    static void printMessage(Message* message, std::ofstream* outputFileStream) {
        if(message) {
            std::string outputTime = std::string(ctime(&message->messageTime));
            outputTime[outputTime.size() - 1] = ' ';
            std::string outputMessage = outputTime + message->source + " [" + severityToString(message->severity) + "] "  + message->text;
            if(message->severity == MESSAGE_ERROR) {
                std::cerr<<outputMessage<<std::endl;
            } else {
                std::cout<<outputMessage<<std::endl;
            }
            if(outputFileStream) {
                (*outputFileStream)<<outputMessage<<std::endl;
                outputFileStream->flush();
            }
        }

    }
};



#endif //LOGGER_H
