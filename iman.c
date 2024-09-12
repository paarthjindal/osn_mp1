#include "iman.h"
#define PORT 80
#define BUFFER_SIZE 4096

void fetchManPage(const char *command) {
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
    if (getaddrinfo("man.he.net", NULL, &hints, &res) != 0) {
        perror("getaddrinfo error");
        return;
    }
    servaddr = *((struct sockaddr_in *) res->ai_addr);
    freeaddrinfo(res);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    // Connect to the server
    servaddr.sin_port = htons(PORT);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection Failed");
        close(sockfd);
        return;
    }

    // Prepare HTTP GET request
    snprintf(request, sizeof(request),
        "GET /cgi-bin/man.cgi?query=%s&manpath=Linux+Man+Pages HTTP/1.1\r\n"
        "Host: man.he.net\r\n"
        "Connection: close\r\n"
        "\r\n", command);

    // Send HTTP GET request
    send(sockfd, request, strlen(request), 0);

    // Read and print the response
    int header_done = 0;
    while ((n = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';

        if (!header_done) {
            // Find end of header
            char *header_end = strstr(buffer, "\r\n\r\n");
            if (header_end) {
                // Skip header
                header_done = 1;
                printf("%s", header_end + 4); // Print body after header
            } else {
                // If header not found, print all buffer content
                continue;
            }
        } else {
            // Print remaining content
            printf("%s", buffer);
        }
    }

    close(sockfd);
}
