#include "ipc/IPCManager.hpp"


IPCManager::IPCManager(std::shared_ptr<Config> runningConsoleConfig, std::shared_ptr<Log> logger) {
    this->runningConsoleConfig = runningConsoleConfig;
    this->logger = logger;

    printf("[IPCManager] Initiating IPC...\n");
    
    std::string defaultPort = runningConsoleConfig->getProperty<std::string>(IPC_PORT);
};

void IPCManager::start() {
    

    printf("[IPCManager] Creating context...\n");
    this->context = std::make_shared<zmqpp::context>();

    printf("[IPCManager] Setting socket-type...\n");
    this->type = zmqpp::socket_type::router;
    printf("[IPCManager] Creating socket...\n");
    this->socket = std::make_shared<zmqpp::socket>(*(this->context.get()),this->type);

    printf("[IPCManager] Binding...\n");
    this->socket->bind("tcp://127.0.0.1:5555");

    printf("[IPCManager] Done\n");
    //TODO: Start Thread to listen
    this->shouldThreadStop = false;
    this->zmqppThread = std::thread(&IPCManager::receiveLoop, this);

};

void IPCManager::receiveLoop() {
    while(!this->shouldThreadStop) {
        
        //Receive ZMQ messages here
        zmqpp::message message;
        
        this->socket->receive(message, false);
        std::string s1 = message.get(0);
        std::string s2 = message.get(1);
        
        std::cout << s1 << " " << s2 << std::endl;
        
        //ATTENTION: S2 is a String Object and IS INDEED VALUE-COMPARED HERE!
        if(s2 == "LOGIN") {
            zmqpp::message response;
            response.add(s1);
            response.add("");
            response.add("OK");
            this->socket->send(response);
        }

    }
}

IPCManager::~IPCManager() {
    printf("[IPCManager] Closing IPC...\n");
    this->shouldThreadStop = true;

    printf("Closing Socket: \n");
    std::string endpoint = this->socket->get<std::string>(zmqpp::socket_option::last_endpoint);

    std::cout << "Unbinding: " << endpoint << std::endl;

    this->socket->unbind(endpoint);
    printf("ERRNO: %s\n", zmq_strerror(zmq_errno()));

    zmq_close(*this->socket.get());
    printf("ERRNO: %s\n", zmq_strerror(zmq_errno()));    

    printf("Destroy Context: \n");
    
    zmq_ctx_term(*(this->context.get()));
    printf("ERRNO: %s\n", zmq_strerror(zmq_errno()));

    //this->zmqppThread.join();
};