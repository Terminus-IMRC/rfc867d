/* https://tools.ietf.org/html/rfc867 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define SIZE 0x1000

int main()
{
	int sock, sock_client;
	struct sockaddr_in addr, client;
	socklen_t len;
	int ret;

	time_t tim;
	struct tm *tm;
	char str[SIZE];
	int str_len;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		fprintf(stderr, "%s:%d: error: socket: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(13);
	addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
	if (ret == -1) {
		fprintf(stderr, "%s:%d: error: bind: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	ret = listen(sock, 128);
	if (ret == -1) {
		fprintf(stderr, "%s:%d: error: listen: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (; ; ) {
		len = sizeof(client);
		sock_client = accept(sock, (struct sockaddr*) &client, &len);
		if (sock_client == -1) {
			fprintf(stderr, "%s:%d: error: accept: %s\n", __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}

		tim = time(NULL);
		tm = localtime(&tim);
		if (tm == NULL) {
			fprintf(stderr, "%s:%d: error: localtime\n", __FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}
		str_len = strftime(str, SIZE, "%A, %B %d, %Y %H:%M:%S-%Z\n", tm);

		write(sock_client, str, str_len);
		close(sock_client);
	}
	close(sock);

	return 0;
}
