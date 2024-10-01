#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

class Topic {
    public:
    vector<string> messages;
    
    void addMessage(const string &message) {
        messages.push_back(message);
    }
};

class QueueSystem {
    private:
    unordered_map<string, Topic> topics;
    unordered_map<string, vector<string> > subscribers;

    public:
    void createTopic(const string &topicName) {
        if (topics.find(topicName) == topics.end()) {
            topics[topicName] = Topic();
            cout << "Topic '" << topicName << "' created." << endl;
        }
    }

    void subscribe(const string &topicName, const string &consumerID) {
        if (topics.find(topicName) != topics.end()) {
            subscribers[topicName].push_back(consumerID);
            cout << consumerID << " subscribed to " << topicName << endl;
        }
    }

    void publish(const string &topicName, const string &message) {
        if (topics.find(topicName) != topics.end()) {
            topics[topicName].addMessage(message);
            cout << "Message published to " << topicName << ": message" << endl;
            notifySubscribers(topicName, message);
        }
    }

    void notifySubscribers(const string &topicName, const string &message) {
        if (subscribers.find(topicName) != subscribers.end()) {
            for (const string &consumer: subscribers[topicName]) {
                cout << consumer << " received " << message << endl;
            }
        }
    }
};


class Producer {
    private:
    string producerId;
    QueueSystem queueSystem;

    public:
    Producer(const string &id, QueueSystem &qs) {
        producerId = id;
        queueSystem = qs;
    }

    void publishMessage(const string &topicName, const string &message) {
        queueSystem.publish(topicName, message);
    }
};

class Consumer {
    private:
    string consumerID;
    QueueSystem queueSystem;

    public:
    Consumer(const string &id, QueueSystem &qs) {
        queueSystem = qs;
        consumerID = id;
    }

    void subcribeToTopic(const string &topicName) {
        queueSystem.subscribe(topicName, consumerID);
    }
};

int main() {
    // Initialize the queue system
    QueueSystem queueSystem;

    // Create topics
    queueSystem.createTopic("topic1");
    queueSystem.createTopic("topic2");

    // Create producers
    Producer producer1("producer1", queueSystem);
    Producer producer2("producer2", queueSystem);

    // Create consumers
    Consumer consumer1("consumer1", queueSystem);
    Consumer consumer2("consumer2", queueSystem);
    Consumer consumer3("consumer3", queueSystem);
    Consumer consumer4("consumer4", queueSystem);
    Consumer consumer5("consumer5", queueSystem);

    // Subscribe consumers to topics
    consumer1.subcribeToTopic("topic1");
    consumer2.subcribeToTopic("topic1");
    consumer3.subcribeToTopic("topic1");
    consumer4.subcribeToTopic("topic1");
    consumer5.subcribeToTopic("topic1");

    consumer1.subcribeToTopic("topic2");
    consumer3.subcribeToTopic("topic2");
    consumer4.subcribeToTopic("topic2");

    // Publish messages to topics
    producer1.publishMessage("topic1", "Message 1");
    producer1.publishMessage("topic1", "Message 2");
    producer2.publishMessage("topic1", "Message 3");
    producer1.publishMessage("topic2", "Message 4");
    producer2.publishMessage("topic2", "Message 5");

    return 0;
}

