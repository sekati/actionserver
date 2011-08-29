/**
 * Sekati ActionServer
 * @version 1.1.0
 * @author Jason M Horwitz <hello@sekati.com>, Sekati LLC
 * @author Markus Stoeger <markus_stoeger@gmx.at>
 * @see http://www.sekati.com
 * @copyright (C) 2004-2011 Markus Stoeger, Jason M Horwitz, Sekati LLC. All Rights Reserved.
 * @license Released under the MIT License: http://www.opensource.org/licenses/mit-license.php 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include "daemon.h"
#include "logging.h"

#define NUM_CLIENTS 500
#define MSG_BUF_SIZE 8192
#define MSG_END '\0'
#define MAX_ROOM_LENGTH 20
#define LOG_CHANNEL "*gimmeallchannel*"
#define PORT 2229
#define LOG_FILE "/tmp/actionserver.log"

typedef struct {
	int s; // socket
	char room[MAX_ROOM_LENGTH];
	unsigned char buf[MSG_BUF_SIZE];
	int buf_len;
} t_client;

int main() {
	int i;
	int ss; // server socket
	t_client clients[NUM_CLIENTS];
	int smax;
	struct sockaddr_in sin;
	fd_set fds;
	int yes = -1;

	log_start(LOG_FILE);

	log_write("%s - ActionServer starting...\n", log_fmt_time());

	if (daemonize() != 0) {
		exit(0);
	}

	log_write("%s - daemonization successful...\n", log_fmt_time());

	for (i = 0; i < NUM_CLIENTS; i++) {
		clients[i].s = -1;
	}

	signal(SIGPIPE, SIG_IGN);

	ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ss == -1) {
		log_write("%s - error creating server socket! exiting...\n", log_fmt_time());
		return -1;
	}

	smax = ss;

	if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		log_write("%s - error setting socket option! exiting...\n", log_fmt_time());
		return -1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = INADDR_ANY;

	if (-1 == bind(ss, (struct sockaddr *)&sin, sizeof(sin))) {
		log_write("%s - error binding socket! exiting...\n", log_fmt_time());
		return -1;
	}

	if (-1 == listen(ss, 5)) {
		log_write("%s - error listening on socket! exiting...\n", log_fmt_time());
		return -1;
	}

	log_write("%s - server is up and running!\n", log_fmt_time());

	while (1) {
		FD_ZERO(&fds);

		FD_SET(ss, &fds);

		for (i = 0; i < NUM_CLIENTS; i++) {
			if (clients[i].s != -1) {
				FD_SET(clients[i].s, &fds);
			}
		}

		if (select(smax+1, &fds, NULL, NULL, NULL) == -1) {
			log_write("%s - error select()ing on socket! exiting...\n", log_fmt_time());
			return -1;
		}

		if (FD_ISSET(ss, &fds)) {
			int s_tmp;
			struct sockaddr_in sin_tmp;
			socklen_t addrlen_tmp = sizeof(sin_tmp);
			unsigned int ip;

			s_tmp = accept(ss, (struct sockaddr *)&sin_tmp, &addrlen_tmp);

			if (s_tmp != -1) {
				for (i = 0; i < NUM_CLIENTS && clients[i].s != -1; i++);

				if (i == NUM_CLIENTS) {
					log_write("%s - rejecting incoming connection from %s... max. clients reached!\n", log_fmt_time(), inet_ntoa(sin_tmp.sin_addr));
					close(s_tmp);
				} else {
					log_write("%s - accepted incoming connection from %s (id %d)!\n", log_fmt_time(), inet_ntoa(sin_tmp.sin_addr), i);
					clients[i].s = s_tmp;
					clients[i].buf_len = 0;
					clients[i].room[0] = MSG_END;
					if (s_tmp > smax) smax = s_tmp;
				}
			}
		}

		for (i = 0; i < NUM_CLIENTS; i++) {
			int num_read;
			int buf_free = MSG_BUF_SIZE-clients[i].buf_len;

			if (clients[i].s == -1)
				continue;

			if (!FD_ISSET(clients[i].s, &fds))
				continue;

			num_read = recv(clients[i].s, clients[i].buf+clients[i].buf_len, buf_free, 0); /* use MSG_NOSIGNAL? */

			switch (num_read) {
			case -1:
				log_write("%s - error receiving from client... disconnecting client (id %d)!\n", log_fmt_time(), i);
			case 0:
				close(clients[i].s);
				clients[i].s = -1;
				log_write("%s - client (id %d) disconnected!\n", log_fmt_time(), i);
				break;
			default: {
				int j;

				clients[i].buf_len += num_read;

				do {
					for (j = 0; j < clients[i].buf_len && clients[i].buf[j] != MSG_END; j++);

					if (j == clients[i].buf_len && clients[i].buf_len == MSG_BUF_SIZE) {
						log_write("%s - client (id %d) buffer full... disconnecting client!\n", log_fmt_time(), i);
						close(clients[i].s);
						clients[i].s = -1;
						log_write("%s - client (id %d) disconnected!\n", log_fmt_time(), i);
					} else if (j < clients[i].buf_len) {
						t_client *c;

						if (strlen(clients[i].room) == 0) {
							if (j > MAX_ROOM_LENGTH-1) {
								log_write("%s - client (id %d) requested a too long room name... disconnecting client!\n", log_fmt_time(), i);
								close(clients[i].s);
								clients[i].s = -1;
								log_write("%s - client (id %d) disconnected!\n", log_fmt_time(), i);
							} else {
								memcpy(clients[i].room, clients[i].buf, j);
								clients[i].room[j] = MSG_END;
								log_write("%s - client (id %d) joined room %s!\n", log_fmt_time(), i, clients[i].room);
							}
						} else {
							for (c = clients; c < clients+NUM_CLIENTS; c++) {
								if (c->s != -1 && c->s != clients[i].s &&
								    strlen(c->room) != 0 &&
								    (strcmp(clients[i].room, c->room) == 0 ||
								     strcmp(c->room, LOG_CHANNEL) == 0)) {
									if (send(c->s, clients[i].buf, j+1, 0) == -1) { /* use MSG_NOSIGNAL? */
										log_write("%s - error sending to client (id %d)... disconnecting client!\n", log_fmt_time(), i);
										close(c->s);
										c->s = -1;
										log_write("%s - client (id %d) disconnected!\n", log_fmt_time(), i);
									}
								}
							}
						}
	
						memmove(clients[i].buf, clients[i].buf+j+1, clients[i].buf_len-j-1);
						clients[i].buf_len -= j+1;
					}
				} while (j < clients[i].buf_len);

				break;
			}
			}
		}
	}

	return 0;
}

