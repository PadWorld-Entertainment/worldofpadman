#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 27951

static void process_message(char *data, int len, struct sockaddr_in *client_addr, int sock, const char *motdBuffer) {
	// Check if the message has the correct header
	if (len < 4 || memcmp(data, "\xFF\xFF\xFF\xFF", 4) != 0) {
		// printf("Invalid header\n");
		return;
	}

	// Extract payload
	char *payload = data + 4;
	int payload_len = len - 4;

	// Check if the payload starts with "getmotd"
	if (payload_len < 7 || strncmp(payload, "getmotd", 7) != 0) {
		// printf("Invalid cmd\n");
		return;
	}

	// Parse the infostring
	char *infostring = payload + 10;
	int infostring_len = payload_len - 10 - 2; // Exclude trailing bytes
	if (infostring_len <= 0) {
		// printf("Invalid infostring\n");
		return;
	}

	// Convert infostring to a map
	char *key, *value;
	char *token = strtok(infostring, "\\");
	char challenge[BUFFER_SIZE] = {0};

	while (token) {
		key = token;
		token = strtok(NULL, "\\");
		if (!token)
			break;
		value = token;
		if (strcmp(key, "challenge") == 0) {
			strncpy(challenge, value, BUFFER_SIZE - 1);
		}
		token = strtok(NULL, "\\");
	}

	if (strlen(challenge) == 0) {
		// printf("Challenge not found\n");
		return;
	}

	// Construct the MOTD response
	char motd[BUFFER_SIZE];
	snprintf(motd, BUFFER_SIZE, "\xFF\xFF\xFF\xFFmotd \"\\motd\\%s\\challenge\\%s\"\n", motdBuffer, challenge);

	// Send the response
	sendto(sock, motd, strlen(motd), 0, (struct sockaddr *)client_addr, sizeof(*client_addr));
	// printf("MOTD sent: %s\n", motd);
}

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in server_addr, client_addr;
	char buffer[BUFFER_SIZE];
	const char *motdBuffer;
	socklen_t addr_len = sizeof(client_addr);

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <motd>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Set the MOTD
	motdBuffer = argv[1];
	printf("MOTD set to: %s\n", motdBuffer);

	// Create a UDP socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Bind the socket to the local address and port
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	printf("UDP server started on port %d\n", PORT);

	// Main loop to handle incoming datagrams
	while (1) {
		int len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
		if (len < 0) {
			perror("Receive failed");
			continue;
		}

		buffer[len] = '\0'; // Null-terminate the received data
		printf("Received %d bytes from %s:%d\n", len, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		process_message(buffer, len, &client_addr, sock, motdBuffer);
	}

	// Close the socket - Never reached but who knows...
	close(sock);
	return 0;
}
