/*
 * =====================================================================================
 *
 *       Filename:  knock.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/13/2012 10:42:51 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <argp.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 3001
#define MAXDATASIZE 5000

int number;

/* *******************  listall  ******************** */
void listall(int argc, char **argv)
{
	int sockfd, nbytes;
	char buf[1024] = {0};
	struct hostent *he;
	struct sockaddr_in srvaddr;
/*
	int idx;
	int err = argp_parse(&list_argp, argc, argv, 
			ARGP_IN_ORDER, &idx, NULL);
	if (err != 0) {
		printf("argp_parse error: %d\n", err);
		exit(-1);
	}
	if(number < 1 || number > 100) {
		printf("n is between 1 and 100\n");
		exit(-1);
	} 

	printf("list: %d\n", number);
*/
	sprintf(buf, "listall");
	if(strcmp(buf, "listall") == 0) 
		printf("buf is listall\n");
  
	if((he = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		exit(-1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("listall socket error");
		exit(-1);
	}
	bzero(&srvaddr, sizeof(srvaddr));

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr = *((struct in_addr *)he->h_addr);

	if(connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(struct sockaddr)) == -1) {
		perror("connect error");
		exit(-1);
	}
	if(write(sockfd, buf, strlen(buf)) == -1) {
		perror("send error");
		exit(-1);
	}

	if((nbytes = read(sockfd, buf, MAXDATASIZE)) == -1) {
		perror("read error");
		exit(-1);
	}

	buf[nbytes] = '\0';
	printf("%s\n", buf);
	close(sockfd);
}


/* *********************  list  ********************** */
static char list_doc[] = 
	"list: list info of virtual machine\n"
	"--number must be provided.";

static char list_args_doc[] = 
	"";

static struct argp_option list_options[] = {
	{"number", 'n', "target number", 0 ,"number of virtual machine", 0},
	{NULL, '\0', NULL, 0, NULL ,0},
};

static error_t 
parse_list_opt(int key, char * arg, 
		struct argp_state * state /*__attribute__((unused))*/ )
{
	switch (key) {
	case 'n' :
		number = atoi(arg);
		return 0;
	default:
		return ARGP_ERR_UNKNOWN;
	}
}

static struct argp list_argp = {list_options, parse_list_opt,
	list_args_doc, list_doc, NULL, NULL, NULL};

void list(int argc, char ** argv)
{
	int sockfd, nbytes;
	char buf[1024] = {0};
	struct hostent *he;
	struct sockaddr_in srvaddr;

	int idx;
	int err = argp_parse(&list_argp, argc, argv, 
			ARGP_IN_ORDER, &idx, NULL);
	if (err != 0) {
		printf("argp_parse error: %d\n", err);
		exit(-1);
	}
	if(number < 1 || number > 100) {
		printf("n is between 1 and 100\n");
		exit(-1);
	} 

	printf("list: %d\n", number);
	sprintf(buf, "list %d", number);
	if(strcmp(buf, "list 1") == 0) 
		printf("buf is list 1\n");
  
	if((he = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		exit(-1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("list socket error");
		exit(-1);
	}
	bzero(&srvaddr, sizeof(srvaddr));

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr = *((struct in_addr *)he->h_addr);

	if(connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(struct sockaddr)) == -1) {
		perror("connect error");
		exit(-1);
	}
	if(write(sockfd, buf, strlen(buf)) == -1) {
		perror("send error");
		exit(-1);
	}

	if((nbytes = read(sockfd, buf, MAXDATASIZE)) == -1) {
		perror("read error");
		exit(-1);
	}

	buf[nbytes] = '\0';
	printf("%s\n", buf);
	close(sockfd);
}

/* ************  create  ***************/
static char create_doc[] = 
	"create: create new virtual machine\n"
	"--number must be provided.";

static char create_args_doc[] = 
	"";

static struct argp_option create_options[] = {
	{"number", 'n', "target number", 0 ,"number of virtual machine", 0},
	{NULL, '\0', NULL, 0, NULL ,0},
};

static error_t 
parse_create_opt(int key, char * arg, 
		struct argp_state * state /*__attribute__((unused))*/ )
{
	switch (key) {
	case 'n' :
		number = atoi(arg);
		return 0;
	default:
		return ARGP_ERR_UNKNOWN;
	}
}

static struct argp create_argp = {create_options, parse_create_opt,
	create_args_doc, create_doc, NULL, NULL, NULL};

void create(int argc, char ** argv)
{
	int sockfd, nbytes;
	char buf[1024] = {0};
	struct hostent *he;
	struct sockaddr_in srvaddr;

	int idx;
	int err = argp_parse(&create_argp, argc, argv, 
			ARGP_IN_ORDER, &idx, NULL);
	if (err != 0) {
		printf("argp_parse error: %d\n", err);
		exit(-1);
	}
	if(number < 1 || number > 100) {
		printf("n is between 1 and 100\n");
		exit(-1);
	} 
	printf("create %d\n", number);
	sprintf(buf, "create %d", number);
	if(strcmp(buf, "create 1") == 0) 
		printf("buf is create 1\n");
  
	if((he = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		exit(-1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("create socket error");
		exit(-1);
	}
	bzero(&srvaddr, sizeof(srvaddr));

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr = *((struct in_addr *)he->h_addr);

	if(connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(struct sockaddr)) == -1) {
		perror("connect error");
		exit(-1);
	}
	if(write(sockfd, buf, strlen(buf)) == -1) {
		perror("send error");
		exit(-1);
	}

	if((nbytes = read(sockfd, buf, MAXDATASIZE)) == -1) {
		perror("read error");
		exit(-1);
	}

	buf[nbytes] = '\0';
	printf("%s\n", buf);
	close(sockfd);
}

/* ***************  usage  ***************** */
static void usage(const char * arg)
{
	printf("usage:\n");
	printf("\t%s create -n (1--100)\n", arg);
	printf("\t%s list -n (1--100)\n", arg);
	printf("\t%s listall\n", arg);
	exit(-1);
}

/* **********************  main  ****************** */
int main(int argc, char **argv)
{
	if(argc < 2)
		usage(argv[0]);
	if(strcmp(argv[1], "create") == 0) {
		create(argc - 1, &argv[1]);
	} else if (strcmp(argv[1], "list") == 0) {
		list(argc - 1, &argv[1]);
	} else if (strcmp(argv[1], "listall") == 0) {
		listall(argc - 1, &argv[1]);
	} else
		usage(argv[0]);
	return 0;
}
