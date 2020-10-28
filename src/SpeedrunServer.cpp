#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

void error(const char *msg) {
	perror(msg);
	exit(1);
}

static const int max_players = 4;

int findSocketIndex(int socket, int socketArray[max_players])
{
	
	int index = -1;
	
	for(int i = 0; i < max_players; i++)
	{
		
		if(socket == socketArray[i])
		{
			index = i;
			break;
		}
		
	}
	
	return index;
	
}

int main(int argc, char *argv[]) {
	
	// Welcome to the Server code!
	// Let's define some variables...

	// Connection settings:
	const char* hostName = "localhost";
	const uint16_t portNum = 3060;
	char buffer[256]; // bytes to communicate

	// Server's stuff:
	int serverSocket;
	fd_set socketReadSet; // look at select() manpage to learn about the sets

	// Clients' stuff:
	const int max_players = 4;
	int clientSockets[max_players]; // all of our connected clients
	int clientSocket; // we will fill this with the current client we're talking to


	// Create our server socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		error("Error creating socket...");
	}

	// set server socket to allow for multiple connections
	int option = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &option, sizeof(option)) < 0) {
		error("Error allowing for multiple connections...");
    }

	// Create our server and client address object
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	bzero((char*) &serverAddress, sizeof(serverAddress));
	bzero((char*) &clientAddress, sizeof(clientAddress));
	socklen_t clientLength = sizeof(clientAddress);
	// again, the client variables will be used for the current client

	// Populate our server address object
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNum);

	// Bind the server to its address
	if (bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
		error("Error while binding...");
	}

	// initialize all client sockets to 0 (no connection! - remember, sockets are just ints)
    for (int i = 0; i < max_players; i++) {
        clientSockets[i] = 0;
    }

	// Listen for connections to the server socket, up to 5 on waiting list
	if (listen(serverSocket, 5) < 0) {
		error("Error trying to listen...");
    }
	
	while (true) {
		// start off each iteration with our read set = {Server}
        FD_ZERO(&socketReadSet);
        FD_SET(serverSocket, &socketReadSet);
        int max_fds = serverSocket; // we will have to find the max file descriptor value
		
        // add the client sockets to our read set
        for (int i = 0; i < max_players; i++) {
			clientSocket = clientSockets[i];
            
			// if connected (valid socket descriptor), add it to the read set
			if (clientSocket > 0) {
				FD_SET(clientSocket, &socketReadSet);
			}
            
            // new highest file descriptor!
            if (clientSocket > max_fds) {
				max_fds = clientSocket;
			}
        }
		
        // wait indefinitely for activity on one of the sockets
        int activity = select(max_fds + 1, &socketReadSet, NULL/*socketWriteSet*/, NULL, NULL); // again, look at select() manpage to understand arguments
        if ((activity < 0) && (errno != EINTR)) {
            error("Error on select!");
        }
		
        // If the server is in the read set, there's an incoming connection
        if (FD_ISSET(serverSocket, &socketReadSet)) {
            if ((clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientLength)) < 0) {
                error("Error on accept!");
            }
             
            //add new client socket to the array of sockets
            for (int i = 0; i < max_players; i++) {
                // find the first empty position in the array
				if (clientSockets[i] == 0) {
                    clientSockets[i] = clientSocket;
                    printf("Added new client to clientSockets[%d] = %d\n", i, clientSocket);
					break;
                }
            }
        }
        
        // Find the socket there was activity on
        for (int i = 0; i < max_players; i++) {
            clientSocket = clientSockets[i];
			int value;
             
            if (FD_ISSET(clientSocket, &socketReadSet)) {
				// let's read in what the client wrote to us
				bzero(buffer, 256);
				int value = read(clientSocket, buffer, 255);

                if (value == 0) {
                    // The client disconnected
                    close(clientSocket);

					toldPlayerSeed[findSocketIndex(clientSocket, clientSockets)] = false;
                    clientSockets[i] = 0;
                } else {
					printf("Received message from Client %d: \"%s\"\n", clientSocket, buffer);
					
					int currentIndex = findSocketIndex(clientSocket, clientSockets);
					if(toldPlayerSeed[currentIndex] == false) //setup, basically just passing the seed around
					{
						toldPlayerSeed[currentIndex] = true;
						if(seed == -1)
						{
							seed = atoi(buffer);
							bzero(buffer, 256);
							int lS = sprintf(buffer, "%i", seed);
							send(clientSocket, buffer, strlen(buffer), 0);
						}
						else
						{
							bzero(buffer, 256);
							int lS = sprintf(buffer, "%i", seed);
							send(clientSocket, buffer, strlen(buffer), 0);
						}
					}
					else //repeating game loop, basically just movement
					{
						// ESSENTIAL SO SELECT() DOESN'T BLOCK
						send(clientSocket, buffer, strlen(buffer), 0);
						// i dont think the client ever does anything with this
						// BUT IT IS EXPERIMENTALLY NECESSARY
					}
					send(clientSocket, buffer, strlen(buffer), 0);

                }
            }
        }
	}

	// NOTE: Since there is a max of 4 players, if more than 4 try to join, they will be blocked indefinitely (even if a client disconnects to make room)
	close(serverSocket);
	return 0; 
}