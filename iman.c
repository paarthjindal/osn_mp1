#include "iman.h"
#define PORT 80
#define BUFFER_SIZE 4096

void fetchManPage(const char *command)
{
    int sockfd;
    struct sockaddr_in servaddr;
    struct addrinfo hints, *res;
    char buffer[BUFFER_SIZE];
    char request[1024];
    int n;

    // Resolve hostname
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo("man.he.net", NULL, &hints, &res) != 0)
    {
        perror("getaddrinfo error");
        return;
    }
    servaddr = *((struct sockaddr_in *)res->ai_addr);
    servaddr.sin_port = htons(PORT); // Set the port
    freeaddrinfo(res);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return;
    }

    // Connect to the server
    servaddr.sin_port = htons(PORT);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Connection Failed");
        close(sockfd);
        return;
    }

    // Prepare the correct HTTP GET request
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n"
             "\r\n",
             command);

    // Send HTTP GET request
    send(sockfd, request, strlen(request), 0);

    // Read and print the response
    int header_done = 0;
    while ((n = read(sockfd, buffer, sizeof(buffer) - 1)) > 0)
    {
        // printf("reached");
        buffer[n] = '\0';
        // i am assuming that i am not ignoring headers and also including it 
        printf("%s", buffer);
    }

    close(sockfd);
}