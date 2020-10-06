#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	const char* hostName = "localhost";
	const uint16_t portNum = 3060;
	int clientSocket, serverSocket;
	char buffer[256]; // bytes to communicate

	// Create our socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		error("Error creating socket...");
	}

	// Create our server and client objects
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	bzero((char*) &serverAddress, sizeof(serverAddress));
	bzero((char*) &clientAddress, sizeof(clientAddress));

	// Populate our server object
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNum);

	// Bind the server to its address
	if (bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
		error("Error while binding...");
	}

	while (true) {
		// Listen for connections to the server socket, up to 5 on waiting list
		listen(serverSocket, 5);
		socklen_t clientLength = sizeof(clientAddress);
		// accept blocks until a client connects
		clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientLength);
		if (clientSocket < 0) {
			error("Error accepting client...");
		}

		// Read the message from the client
		bzero(buffer, 256);
		int n = read(clientSocket, buffer, 255);
		if (n < 0) {
			error("Error reading from client...");
		}

		printf("Received message from %d: %s\n", serverAddress.sin_addr.s_addr, buffer);

		n = write(clientSocket, "I got your message", 18);
		if (n < 0) {
			error("Error writing to client...");
		}
	}

	close(clientSocket);
	close(serverSocket);

	return 0; 
}