/*
 * =============================================================================
 *
 *       Filename:  virt.c
 *
 *    Description:  node for virtual machine
 *
 *        Version:  0.0.1
 *        Created:  20/02/2012 12:07:43 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  wangwei (WW), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
/*service.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#define MYPORT 3001 /*开放的端口号*/
#define BACKLOG 5  /*指定套接字可以接受的最大未接受客户机请求的数目*/
char buf[10240] = {0};


char * getDomainInterfacePath(virDomainPtr dom)
{
	char * ret;
	char *p;
	char *xmldesc;
	xmldesc = virDomainGetXMLDesc(dom, 0);
	ret = strstr(xmldesc, "vnet");
	for(p = ret ; *p != '\''; p++)
		;
	*p = '\0';
	return ret;
}

void resume_all()
{
	int * state = malloc(sizeof(int));
	int i;
	int doms[100]={0};
	virConnectPtr conn;
	memset(buf, '\0', 10240);
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///system\n");
		sprintf(buf, "failed to open connection to qemu:///system");
		return;
	} else
		printf("success to open connection to qemu:///system\n");

	if(virConnectListDomains(conn, doms, 100) == -1) {
		virConnectClose(conn);
		printf("failed to list connect domains\n");
		sprintf(buf, "failed to list connect domains");
		return;
	}
	if(doms[0] == 0) {
		printf("no domain is running\n");
		sprintf(buf,"no domain is running\n");
		virConnectClose(conn);
		return;
	}
	virDomainPtr dom;
	//virDomainInfoPtr dominfo = malloc(sizeof(virDomainInfo));
	//virDomainInterfaceStatsPtr stats = malloc(sizeof(virDomainInterfaceStatsStruct)) ;
	for(i = 0; i < 100; i++) {
		//printf("doms[%d] = %d\t", i, doms[i]);
		fflush(stdout);
		if(doms[i] == 0)
			break;
		dom = virDomainLookupByID(conn, doms[i]);
		if(dom == NULL)
			printf("dom error\n");
		
		if((virDomainGetState(dom, state, NULL, 0)) != 0) {
			sprintf(buf, "get domain state failed\n");
			return;
		}
		printf("state : %d\n", *state);
		if(*state != 3) {
			sprintf(buf, "%s is not pausinging\n", virDomainGetName(dom));
			virConnectClose(conn);
			continue;
		}

		if(virDomainResume(dom) == -1) {
			printf("resume %s failed\n", virDomainGetName(dom));
			sprintf(buf, "resume %s failed\n", virDomainGetName(dom));
			virConnectClose(conn);
			return;
		} else {
			printf("resume %s successfully\n", virDomainGetName(dom));
			sprintf(&buf[strlen(buf)], "resume %s successfully\n", virDomainGetName(dom));
		}
	}
	virDomainFree(dom);
	virConnectClose(conn);

	return ;

}

void resume(int number)
{
	int * state = malloc(sizeof(int));
	char number_buf[10] = {0};
	virConnectPtr conn;
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	} else 
		printf("success to open connection to qemu:///session\n");

	sprintf(number_buf, "demo%d", number);
	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(dom == NULL)
		printf("dom error\n");
	
	if((virDomainGetState(dom, state, NULL, 0)) != 0) {
		sprintf(buf, "get domain state failed\n");
		return;
	}
	printf("state : %d\n", *state);
	if(*state != 3) {
		sprintf(buf, "%s is not pausing\n", number_buf);
		virConnectClose(conn);
		return;
	}
	
	if(virDomainResume(dom) == -1) {
		printf("resume %s failed\n", virDomainGetName(dom));
		sprintf(buf, "resume %s failed\n", virDomainGetName(dom));
		virConnectClose(conn);
		return;
	} else {
		printf("resume %s successfully\n", virDomainGetName(dom));
		sprintf(buf, "resume %s successfully\n", virDomainGetName(dom));
	}
	
	virDomainFree(dom);
	virConnectClose(conn);

	return ;
}



void suspend_all()
{
	char * vir_name;
	int i;
	int doms[100]={0};
	virConnectPtr conn;
	memset(buf, '\0', 10240);
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///system\n");
		sprintf(buf, "failed to open connection to qemu:///system");
		return;
	} else 
		printf("success to open connection to qemu:///system\n");

	if(virConnectListDomains(conn, doms, 100) == -1) {
		printf("failed to list connect domains\n");
		sprintf(buf, "failed to list connect domains");
		return;
	}
	if(doms[0] == 0) {
		printf("no domain is running\n");
		sprintf(buf,"no domain is running\n");
		virConnectClose(conn);
		return;
	}
	virDomainPtr dom;
	for(i = 0; i < 100; i++) {
		//printf("doms[%d] = %d\t", i, doms[i]);
		fflush(stdout);
		if(doms[i] == 0)
			break;
		dom = virDomainLookupByID(conn, doms[i]);
		if(dom == NULL)
			printf("dom error\n");
		else 
			vir_name = virDomainGetName(dom);

		if(virDomainSuspend(dom) == -1) {
			printf("suspend %s failed\n", vir_name);
			sprintf(buf, "suspend %s failed\n", vir_name);
			virConnectClose(conn);
			return;
		} else {
			printf("suspend %s successfully\n", vir_name);
			sprintf(&buf[strlen(buf)], "suspend %s successfully\n", vir_name);
		}

		
	}
	virDomainFree(dom);
	virConnectClose(conn);

	return ;

}

void suspend(int number)
{
	int * state = malloc(sizeof(int));

	char * vir_name;
	char number_buf[10] = {0};
	virConnectPtr conn;
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	} else 
		printf("success to open connection to qemu:///session\n");

	sprintf(number_buf, "demo%d", number);
	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(dom == NULL)
		printf("dom error\n");
	else 
		vir_name = virDomainGetName(dom);
	
	if((virDomainGetState(dom, state, NULL, 0)) != 0) {
		sprintf(buf, "get domain state failed\n");
		return;
	}
	printf("state : %d\n", *state);
	if(*state != 1) {
		sprintf(buf, "%s is not running\n", number_buf);
		virConnectClose(conn);
		return;
	}
	
	if(virDomainSuspend(dom) == -1) {
		printf("suspend %s failed\n", vir_name);
		sprintf(buf, "suspend %s failed\n", vir_name);
		virConnectClose(conn);
		return;
	} else {
		printf("suspend %s successfully\n", vir_name);
		sprintf(buf, "suspend %s successfully\n", vir_name);
	}
	
	virDomainFree(dom);
	virConnectClose(conn);

	return ;
}



void shutdown_all()
{
	char * vir_name;
	int i;
	int doms[100]={0};
	virConnectPtr conn;
	memset(buf, '\0', 10240);
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///system\n");
		sprintf(buf, "failed to open connection to qemu:///system");
		return;
	}
	printf("success to open connection to qemu:///system\n");

	if(virConnectListDomains(conn, doms, 100) == -1) {
		printf("failed to list connect domains\n");
		sprintf(buf, "failed to list connect domains");
		return;
	}
	if(doms[0] == 0) {
		printf("no domain is running\n");
		sprintf(buf,"no domain is running\n");
		virConnectClose(conn);
		return;
	}
	virDomainPtr dom;
	for(i = 0; i < 100; i++) {
		//printf("doms[%d] = %d\t", i, doms[i]);
		fflush(stdout);
		if(doms[i] == 0)
			break;
		dom = virDomainLookupByID(conn, doms[i]);
		if(dom == NULL)
			printf("dom error\n");
		else 
			vir_name = virDomainGetName(dom);

		if(virDomainShutdown(dom) == -1) {
			printf("shutdown %s failed\n", vir_name);
			sprintf(buf, "shutdown %s failed\n", vir_name);
			virConnectClose(conn);
			return;
		} else {
			printf("shutdown %s successfully\n", vir_name);
			sprintf(&buf[strlen(buf)], "shutdown %s successfully\n", vir_name);
		}

		
	}
	virDomainFree(dom);
	virConnectClose(conn);

	return ;

}

void shut_down(int number)
{
	int * state = malloc(sizeof(int));

	char * vir_name;
	char number_buf[10] = {0};
	virConnectPtr conn;
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	} else 
		printf("success to open connection to qemu:///session\n");

	sprintf(number_buf, "demo%d", number);
	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(dom == NULL)
		printf("dom error\n");
	else 
		vir_name = virDomainGetName(dom);
	
	if((virDomainGetState(dom, state, NULL, 0)) != 0) {
		sprintf(buf, "get domain state failed\n");
		return;
	}
	//printf("state : %d\n", *state);
	if(*state != 1) {
		sprintf(buf, "%s is not running\n", number_buf);
		virConnectClose(conn);
		return;
	}
	
	if(virDomainShutdown(dom) == -1) {
		printf("shutdown %s failed\n", vir_name);
		sprintf(buf, "shutdown %s failed\n", vir_name);
		virConnectClose(conn);
		return;
	} else {
		printf("shutdown %s successfully\n", vir_name);
		sprintf(buf, "shutdown %s successfully\n", vir_name);
	}
	
	virDomainFree(dom);
	virConnectClose(conn);

	return ;
}

void destroy_all()
{
	char * vir_name;
	int i;
	int doms[100]={0};
	virConnectPtr conn;
	memset(buf, '\0', 10240);
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///system\n");
		sprintf(buf, "failed to open connection to qemu:///system");
		return;
	}
	printf("success to open connection to qemu:///system\n");

	if(virConnectListDomains(conn, doms, 100) == -1) {
		printf("failed to list connect domains\n");
		sprintf(buf, "failed to list connect domains");
		return;
	}
	if(doms[0] == 0) {
		printf("no domain is running\n");
		sprintf(buf,"no domain is running\n");
		virConnectClose(conn);
		return;
	}
	virDomainPtr dom;
	for(i = 0; i < 100; i++) {
		//printf("doms[%d] = %d\t", i, doms[i]);
		fflush(stdout);
		if(doms[i] == 0)
			break;
		dom = virDomainLookupByID(conn, doms[i]);
		if(dom == NULL)
			printf("dom error\n");
		else 
			vir_name = virDomainGetName(dom);

		if(virDomainDestroy(dom) == -1) {
			printf("destroy %s failed\n", vir_name);
			sprintf(buf, "destroy %s failed\n", vir_name);
			virConnectClose(conn);
			return;
		} else {
			printf("destroy %s successfully\n", vir_name);
			sprintf(&buf[strlen(buf)], "destroy %s successfully\n", vir_name);
		}

		
	}
	virDomainFree(dom);
	virConnectClose(conn);

	return ;

}

void destroy(int number)
{
	int * state = malloc(sizeof(int));

	char * vir_name;
	char number_buf[10] = {0};
	virConnectPtr conn;
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	} else 
		printf("success to open connection to qemu:///session\n");

	sprintf(number_buf, "demo%d", number);
	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(dom == NULL)
		printf("dom error\n");
	else 
		vir_name = virDomainGetName(dom);
	
	if((virDomainGetState(dom, state, NULL, 0)) != 0) {
		sprintf(buf, "get domain state failed\n");
		return;
	}
	
	//printf("state : %d\n", *state);
	if(*state != 1) {
		sprintf(buf, "%s is not running\n", number_buf);
		virConnectClose(conn);
		return;
	}
	
	if(virDomainDestroy(dom) == -1) {
		printf("destroy %s failed\n", vir_name);
		sprintf(buf, "destroy %s failed\n", vir_name);
		virConnectClose(conn);
		return;
	} else {
		printf("destroy %s successfully\n", vir_name);
		sprintf(buf, "destroy %s successfully\n", vir_name);
	}
	
	virDomainFree(dom);
	virConnectClose(conn);

	return ;
}

void netflow_all()
{
	long long rx_bytes = 0;
	long long tx_bytes = 0;
	char * vnet;
	int i;
	int doms[100]={0};
	virConnectPtr conn;
	memset(buf, '\0', 10240);
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	}
	printf("success to open connection to qemu:///session\n");
	if(virConnectListDomains(conn, doms, 100) == -1) {
		printf("failed to list connect domains\n");
		sprintf(buf, "failed to list connect domains");
		return;
	}
	if(doms[0] == 0) {
		printf("no domain is running\n");
		sprintf(buf,"no domain is running\n");
		virConnectClose(conn);
		return;
	}

	virDomainPtr dom;
	virDomainInterfaceStatsPtr stats = malloc(sizeof(virDomainInterfaceStatsStruct)) ;
	for(i = 0; i < 100; i++) {
	
		printf("doms[%d] = %d\t", i, doms[i]);
		fflush(stdout);
		if(doms[i] == 0)
			break;
		

		dom = virDomainLookupByID(conn, doms[i]);
		if(dom == NULL)
			printf("dom error\n");
		vnet = getDomainInterfacePath(dom);
		//printf("vnet = %s\n", vnet);
		if(virDomainInterfaceStats(dom, vnet, stats, sizeof(virDomainInterfaceStatsStruct)) != 0) {
			printf("get interface info failed\n");
			sprintf(buf, "get interface info failed");
			return;
		}
		
		virDomainFree(dom);
		rx_bytes += stats->rx_bytes;
		tx_bytes += stats->tx_bytes;
		printf("rx_bytes: %lld, tx_bytes: %lld\n", rx_bytes, tx_bytes);
	}
	sprintf(buf, "rx_bytes: %lld, tx_bytes: %lld\n", rx_bytes, tx_bytes);	

	virConnectClose(conn);

	printf("success close conn\n");
	return ;

}


void netflow(int number)
{
	int * state = malloc(sizeof(int));
	char * vnet;
	char number_buf[10] = {0};
	virConnectPtr conn;
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	}
	printf("success to open connection to qemu:///session\n");

	sprintf(number_buf, "demo%d",number);
	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(dom == NULL )
		printf("dom error\n");
	if((virDomainGetState(dom, state, NULL, 0)) != 0) {
		sprintf(buf, "get domain state failed\n");
		return;
	}

	//printf("state : %d\n", *state);
	if(*state != 1) {
		sprintf(buf, "%s is not running\n", number_buf);
		return;
	}
	
	vnet = getDomainInterfacePath(dom);

	virDomainInterfaceStatsPtr stats = malloc(sizeof(virDomainInterfaceStatsStruct));
	if(virDomainInterfaceStats(dom, vnet, stats, sizeof(virDomainInterfaceStatsStruct)) != 0) {
		printf("get interface info failed\n");
		sprintf(buf, "get interface info failed");
		return;
	} 
	printf("rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);
	sprintf(&(buf[strlen(buf)]), "rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);




	
	virDomainFree(dom);
	virConnectClose(conn);

	return ;
}

void list_all()
{
	char * vnet;
	int i;
	int doms[100]={0};
	virConnectPtr conn;
	memset(buf, '\0', 10240);
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	}
	printf("success to open connection to qemu:///session\n");

	if(virConnectListDomains(conn, doms, 100) == -1) {
		printf("failed to list connect domains\n");
		sprintf(buf, "failed to list connect domains");
		return;
	}
	if(doms[0] == 0) {
		printf("no domain is running\n");
		sprintf(buf,"no domain is running\n");
		virConnectClose(conn);
		return;
	}
	virDomainPtr dom;
	virDomainInfoPtr dominfo = malloc(sizeof(virDomainInfo));
	virDomainInterfaceStatsPtr stats = malloc(sizeof(virDomainInterfaceStatsStruct)) ;
	for(i = 0; i < 100; i++) {
		//printf("doms[%d] = %d\t", i, doms[i]);
		fflush(stdout);
		if(doms[i] == 0)
			break;
		dom = virDomainLookupByID(conn, doms[i]);
		if(dom == NULL)
			printf("dom error\n");
		if(virDomainGetInfo(dom, dominfo) != 0) {
			printf("get domain info failed.\n");
			sprintf(buf, "get domain info failed");
			return;
		}
		printf("%s :\nstate: %d, maxMem: %lu, memory: %lu, nrVirtCpu: %u, cpuTime:%llu\n", virDomainGetName(dom), dominfo->state, dominfo->maxMem, dominfo->memory, dominfo->nrVirtCpu, dominfo->cpuTime);
		sprintf(&buf[strlen(buf)], "%s :\nstate: %d, maxMem: %lu, memory: %lu, nrVirtCpu: %u, cpuTime:%llu\n", virDomainGetName(dom), dominfo->state, dominfo->maxMem, dominfo->memory, dominfo->nrVirtCpu, dominfo->cpuTime);
		
		vnet = getDomainInterfacePath(dom);
		//printf("vnet = %s\n", vnet);
		if(virDomainInterfaceStats(dom, vnet, stats, sizeof(virDomainInterfaceStatsStruct)) != 0) {
			printf("get interface info failed\n");
			sprintf(buf, "get interface info failed");
			return;
		}
		printf("rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);
		sprintf(&(buf[strlen(buf)]), "rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);

		
	}
	free(dominfo);
	free(dom);
	virConnectClose(conn);

	return ;

}

void list(int number)
{
	int * state = malloc(sizeof(int));
	char * vnet;
	char number_buf[10] = {0};
	//printf("in list_virt\n");
	virConnectPtr conn;
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///session\n");
		sprintf(buf, "failed to open connection to qemu:///session");
		return;
	}
	printf("success to open connection to qemu:///session\n");

	sprintf(number_buf, "demo%d",number);
	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(dom == NULL )
		printf("dom error\n");
	if((virDomainGetState(dom, state, NULL, 0)) != 0) {
		sprintf(buf, "get domain state failed\n");
		return;
	}

	//printf("state : %d\n", *state);
	if((*state != 1) && (*state != 3)) {
		sprintf(buf, "%s is not running\n", number_buf);
		return;
	}
	virDomainInfoPtr dominfo = malloc(sizeof(virDomainInfo));
	if(virDomainGetInfo(dom, dominfo) != 0) {
		printf("get domain info failed.\n");
		sprintf(buf, "get domain info failed.");
		return;
	}
	
	printf("state: %d, maxMem: %lu, memory: %lu, nrVirtCpu: %u, cpuTime:%llu\n", dominfo->state, dominfo->maxMem, dominfo->memory, dominfo->nrVirtCpu, dominfo->cpuTime);

	sprintf(buf, "state: %d, maxMem: %lu, memory: %lu, nrVirtCpu: %u, cpuTime:%llu\n", dominfo->state, dominfo->maxMem, dominfo->memory, dominfo->nrVirtCpu, (dominfo->cpuTime)/1000000000);
	

	vnet = getDomainInterfacePath(dom);

	printf("vnet = %s\n", vnet);
	virDomainInterfaceStatsPtr stats = malloc(sizeof(virDomainInterfaceStatsStruct));
	if(virDomainInterfaceStats(dom, vnet, stats, sizeof(virDomainInterfaceStatsStruct)) != 0) {
		printf("get interface info failed\n");
		sprintf(buf, "get interface info failed");
		return;
	} 
	printf("rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);
	sprintf(&(buf[strlen(buf)]), "rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);
	
	virDomainFree(dom);
	virConnectClose(conn);

	return ;
}

void create_all()
{
	int i;
	char number_buf[10] = {0};
	virConnectPtr conn;
	//printf("in create_virt\n");
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///system\n");
		sprintf(buf, "failed to open connection to qemu:///system");
		return;
	}
	virDomainPtr dom;
	printf("success to open connection to qemu:///system\n");
	for(i = 1; i <= 100; i++) {
		sprintf(number_buf, "demo%d", i);
		dom = virDomainLookupByName(conn, number_buf);
		if(virDomainCreate(dom) < 0) {
			printf("fail to boot guest %s.\n", number_buf);
			sprintf(&buf[strlen(buf)], "fail to boot guest %s.\n", number_buf);
			continue;
		}
		printf("guest %s has boot.\n", virDomainGetName(dom));
		sprintf(&buf[strlen(buf)], "guest %s has boot.\n", virDomainGetName(dom));
	}

	virDomainFree(dom);
	virConnectClose(conn);
	return;

}

void create(int number)
{
	char number_buf[10] = {0};
	sprintf(number_buf, "demo%d", number);
	virConnectPtr conn;
	//printf("in create_virt\n");
	conn = virConnectOpen("qemu:///system");
	if(conn == NULL) {
		printf("failed to open connection to qemu:///system\n");
		sprintf(buf, "failed to open connection to qemu:///system");
		return;
	}
	printf("success to open connection to qemu:///system\n");

	virDomainPtr dom = virDomainLookupByName(conn, number_buf);
	if(virDomainCreate(dom) < 0) {
		virDomainFree(dom);
		printf("fail to boot guest.\n");
		sprintf(buf, "fail to boot guest");
		return;
	}
	printf("guest %s has boot.\n", virDomainGetName(dom));
	sprintf(buf, "guest %s has boot.", virDomainGetName(dom));
	virDomainFree(dom);
	virConnectClose(conn);
	return;
}

void main()
{
	int vir_num;
	int nbytes;
	int sockfd, new_fd;
	struct sockaddr_in srvaddr;
	struct sockaddr_in cliaddr;
	int sin_size;
	/*创建套接字描述符*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}

	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	bzero(&srvaddr, sizeof(srvaddr));
	/*用自己的ip地址和端口信息填充一个internet套接字地址结构*/
	srvaddr.sin_family = AF_INET;                 
	srvaddr.sin_port = htons(MYPORT);
	/*函数bind将服务器的地址和套接字帮定在一起*/
	if(bind(sockfd, (struct sockaddr *)&srvaddr, sizeof(struct sockaddr)) == -1) {
		perror("bind error");
		exit(1);
	}
	/*listen函数告诉内核，这个套接字可以接受来自客户机的请求*/
	if(listen(sockfd, BACKLOG) == -1) {
		perror("listen error");
		exit(1);
	}
	/*处理客户机的请求，调用函数accept来获得一个客户机的连接*/
	for(;;)
	{
		sin_size = sizeof(struct sockaddr_in);
		if((new_fd = accept(sockfd, (struct sockaddr *)&cliaddr, &sin_size))==-1) {
			perror("accept error");
			continue;
		}
		printf("server: got connection from %s \n", inet_ntoa(cliaddr.sin_addr));
		/*接受客户数据*/
		if((nbytes = read(new_fd, buf, 10240)) == -1) {
			perror("read error");
			exit(-1);
		}
		//printf("%s\n", buf);
		/*向客户起写数据*/
		char *s;
		if((s = strstr(buf, "createall"))) {
			create_all();
		} else if((s = strstr(buf, "create"))) {
			s += 7;
			vir_num = atoi(s);
			create(vir_num);
		} else if((s = strstr(buf, "listall")))
			list_all();
		else if((s = strstr(buf, "list"))) {
			s += 5;
			vir_num = atoi(s);
			list(vir_num);
		} else if((s = strstr(buf, "netflowall")))
			netflow_all();
		else if((s = strstr(buf, "netflow"))) {
			s += 8;
			vir_num = atoi(s);
			netflow(vir_num);
		} else if((s = strstr(buf, "destroyall"))) 
			destroy_all();
		else if((s = strstr(buf, "destroy"))) {
			s += 8;
			vir_num = atoi(s);
			destroy(vir_num);
		} else if((s = strstr(buf, "shutdownall")))
			shutdown_all();
		else if((s = strstr(buf, "shutdown"))) {
			s += 9;
			vir_num = atoi(s);
			shut_down(vir_num);
		} else if((s = strstr(buf, "suspendall")))
			suspend_all();
		else if((s = strstr(buf, "suspend"))) {
			s += 8;
			vir_num = atoi(s);
			suspend(vir_num);
		} else if((s = strstr(buf, "resumeall"))) 
			resume_all();
		else if((s = strstr(buf, "resume"))) {
			s += 7;
			vir_num = atoi(s);
			resume(vir_num);
		}

		write(new_fd, buf, 10240);
		close(new_fd);
		//close(sockfd);
		memset(buf, 0, 10240);
	}
    close(sockfd);
}

