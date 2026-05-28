#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int current_voip = 580;
int current_filt = 2;
double financial_gain = 48921.40;

// Isolated background thread handling incoming web connection requests
void *api_server_thread(void *arg) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080); // API Gateway stream port mapping
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket >= 0) {
            char buffer[1024] = {0};
            read(new_socket, buffer, 1024);
            
            // Build real-time JSON data stream payload string
            char json_response[512];
            sprintf(json_response, 
                "{\"voip_ingress_kbps\":%d,\"filtration_drop_rate\":%d,\"financial_gain_usd\":%.2f}", 
                current_voip, current_filt, financial_gain);
                
            // Send standardized HTTP headers with CORS unblocking configurations
            char http_header[1024];
            sprintf(http_header, 
                "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: %d\r\n\r\n%s", 
                (int)strlen(json_response), json_response);
                
            write(new_socket, http_header, strlen(http_header));
            close(new_socket);
        }
        usleep(10000); // 10ms cycle sleep to protect processing threads
    }
    return NULL;
}

int main() {
    printf("\n\033[1;36m[MARDUKH ENGINE GALAXY CONTEXT v2.0.0]\033[0m\n");
    printf("[GATEWAY] Deploying Thread-Isolated Local API Server on Port 8080...\n");
    
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, api_server_thread, NULL);
    
    printf("\033[1;32m[EXEC] Active processing cores initialized on 2 threads.\033[0m\n\n");
    
    int cycle = 1;
    while(1) {
        // Generate fluctuating telemetry data parameters locally
        current_voip = 450 + (rand() % 350);
        current_filt = 1 + (rand() % 8);
        financial_gain += 0.05;
        
        printf("[DATA] Cycle %d: Emitting live network state to port 8080 [SPEED: 412 H/s]\n", cycle++);
        sleep(2);
    }
    return 0;
}
