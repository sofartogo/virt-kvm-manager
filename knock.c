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
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <string.h>
#include <error.h>
#include <argp.h>

//static void customErrorFunc(void * userdata, virErrorPtr err)
//{
//	fprintf(stderr, "failure of libvirt libvirt call:\n");
//	fprintf(stderr, " Code: %d\n", err->code);
//	fprintf(stderr, " Domain: %d\n", err->domain);
//	fprintf(stderr, " Message: %s\n", err->message);
//	fprintf(stderr, " Level: %d\n", err->level);
//	fprintf(stderr, " str1: %s\n", err->str1);
//	fprintf(stderr, " str2: %s\n", err->str2);
//	fprintf(stderr, " str3: %s\n", err->str3);
//	fprintf(stderr, " int1: %d\n", err->int1);
//	fprintf(stderr, " int2: %d\n", err->int2);
//}

char * vir_ip = NULL;

static char create_doc[] = 
	"create: create new virtual machine\n"
	"--ipaddress must be provided.";

static char create_args_doc[] = 
	"";

static struct argp_option create_options[] = {
	{"ipaddress", 'i', "target ipaddress", 0 ,"name of virtual ipaddress", 0},
	{NULL, '\0', NULL, 0, NULL ,0},
};

static error_t 
parse_create_opt(int key, char * arg, 
		struct argp_state * state /*__attribute__((unused))*/ )
{
	switch (key) {
	case 'i' :
		vir_ip = arg;
		return 0;
	default:
		return ARGP_ERR_UNKNOWN;
	}
}

static struct argp create_argp = {create_options, parse_create_opt,
	create_args_doc, create_doc, NULL, NULL, NULL};

int create_virtual_machine(int argc, char ** argv)
{
	int idx;
	int err = argp_parse(&create_argp, argc, argv, 
			ARGP_IN_ORDER, &idx, NULL);
	if (err != 0) {
		printf("argp_parse error: %d\n", err);
		exit(-1);
	}
	if(vir_ip == NULL) {
		printf("use -i or --ipaddress to set the virtual ipaddress");
		exit(-1);
	} 
	
	virConnectPtr conn;
	conn = virConnectOpenAuth("qemu+ssh://root@127.0.0.1/system", virConnectAuthPtrDefault, 0);
	if(conn == NULL) {
		fprintf(stderr, "failed to open connection to qemu:///system\n");
		return 1;
	} else {
		fprintf(stderr, "successful to open connection to qemu:///system\n");
	}
	
	//char *caps;
	//caps = virConnectGetCapabilities(conn);
	//fprintf(stdout, "capabilities:\n%s\n",caps);
	//free(caps);

	//char * host;
	//host = virConnectGetHostname(conn);
	//fprintf(stdout, "hostname:%s\n", host);
	//free(host);

	//int vcpus;
	//vcpus = virConnectGetMaxVcpus(conn, NULL);
	//fprintf(stdout, "maximum support virtual CPUs: %d\n",vcpus);

	//unsigned long long node_free_memory;
	//node_free_memory = virNodeGetFreeMemory(conn);
	//fprintf(stdout, "free memory : %llu\n",node_free_memory);

	//virNodeInfo nodeinfo;
	//virNodeGetInfo(conn, &nodeinfo);
	//fprintf(stdout, "model: %s\n", nodeinfo.model);
	//fprintf(stdout, "memory size: %lukb\n", nodeinfo.memory);
	//fprintf(stdout, "number of CPUs: %u\n", nodeinfo.cpus);
	//fprintf(stdout, "MHZ of CPUs: %u\n", nodeinfo.mhz);
	//fprintf(stdout, "number of NUMA nodes: %u\n", nodeinfo.nodes);
	//fprintf(stdout, "number of CPU sockets: %u\n", nodeinfo.sockets);
	//fprintf(stdout, "number of CPU cores per socket: %u\n", nodeinfo.cores);
	//fprintf(stdout, "number of CPU threads per core: %u\n", nodeinfo.threads);

	//virNodeInfo nodeinfo;
	//unsigned long long * freemem;
	//int i, numnodes;
	//virNodeGetInfo(conn, &nodeinfo);
	//freemem = malloc(nodeinfo.nodes * sizeof(unsigned long long));
	//numnodes = virNodeGetCellsFreeMemory(conn, freemem, 0, nodeinfo.nodes);
	//for(i = 0; i< numnodes; i++)
	//	fprintf(stdout, "Node %d: %llukb free memory\n", i, freemem[i]);
	//free(freemem);

	//fprintf(stdout, "virtualization type: %s\n",virConnectGetType(conn));

	//unsigned long ver;
	//virConnectGetVersion(conn, &ver);
	//fprintf(stdout, "version: %lu\n", ver);

	//virConnectGetLibVersion(conn, &ver);
	//fprintf(stdout, "libvirt version: %lu\n", ver);
	
	//char * uri;
	//uri = virConnectGetURI(conn);
	//fprintf(stdout, "canonical URI: %s\n", uri);
	//free(uri);

	//fprintf(stdout, "connection is encrypted: %d\n", virConnectIsEncrypted(conn));

	//fprintf(stdout, "connection is secure: %d\n", virConnectIsSecure(conn));

	//virSecurityModel secmodel;
	//virNodeGetSecurityModel(conn, &secmodel);
	//fprintf(stdout, "security model: %s\n", secmodel.model);
	//fprintf(stdout, "security DOI: %s\n", secmodel.doi);
	
	//virSetErrorFunc(NULL, customErrorFunc);
	//if(virConnectGetVersion(conn, NULL) < 0)
	//	fprintf(stderr, "virConnectGetVersion failed\n");

	const char * xmlconfig = "<domain type = 'kvm'><name>demo1</name><title>a first demo xml to create a vm.</title><memory>131072</memory><vcpu>1</vcpu><os><type arch='i686' machine='pc'>hvm</type><boot dev='hd'/><bootmenu enable='no'/></os><feature><acpi/><apic/><pae/></feature><clock offset='localtime'/><on_poweroff>destroy</on_poweroff><on_reboot>restart</on_reboot><on_crash>restart</on_crash><devices><emulator>/usr/bin/qemu</emulator><disk type='file' device='disk'><source file='/home/ww/Work/libvirt/w1.disk.img'/><target dev='hda' bus='virtio'/><driver name='qemu' type='raw'/></disk><interface type='network'><model type='virtio'/><source network='default'/><mac address='1c:75:08:59:f6:22'/></interface><input type='mouse' bus='ps2'/><graphics type='vnc' port='5910' autoport='no' listen='127.0.0.1'/></devices></domain>";
	//virDomainPtr dom = virDomainDefineXML(conn, xmlconfig);
	virDomainPtr dom = virDomainLookupByName(conn, "demo1");
	//virDomainPtr dom = virDomainCreateXML(conn, xmlconfig, VIR_DOMAIN_NONE);
	//if(!dom) {
	//	printf("guest booted failed.\n");
	//	exit(-1);
	//}
	
	//char * xml_desc;
	//if((xml_desc = virDomainGetXMLDesc(dom, VIR_DOMAIN_XML_SECURE)) != 0) {
	//	printf("failed to get xmldesc\n");
	//	exit(-1);
	//}
	//printf("xml_desc = %s\n", xml_desc);
	//printf("haha\n");
	if(virDomainCreate(dom) < 0) {
		virDomainFree(dom);
		printf("cannot boot guest.\n");
		exit(-1);
	}
	printf("guest %s has booted.\n", virDomainGetName(dom));
	//FILE *fp;
	//char *xml;
	//xml = virDomainGetXMLDesc(dom, 0);
	//if((fp = fopen(virDomainGetName(dom), "w")) ==NULL) {
	//	printf("cannot open file test\n");
	//}
	//fprintf(fp, xml);
	//fclose(fp);
	//free(xml);

	//if(virDomainReboot(dom, 0) != 0){
	//	printf("reboot failed.\n");
	//}
	
	//virStreamPtr stream = virStreamNew(conn, 1);
	//virDomainOpenConsole(dom, NULL, stream, 0);
	
	//if(virDomainShutdown(dom) != 0) {
	//	printf("failed to shutdown\n");
	//	exit(-1);
	//}
	//printf("shutdown.\n");

	//if(virDomainDestroy(dom) != 0) {
	//	printf("destroy dom failed.\n");
	//	exit(-1);
	//}
	//printf("%s destroy!\n", virDomainGetName(dom));

	//if(virDomainCreate(dom) != 0) {
	//	printf("gust booted again failed.\n");
	//	exit(-1);
	//}

	//printf("guest %s booted again!\n", virDomainGetName(dom));

	virDomainInfoPtr domInfo;
	if(virDomainGetInfo(dom, domInfo) != 0) {
		printf("get domain info failed!\n");
		exit(-1);
	}
	printf("state: %d, maxMem: %lu, memory: %lu, nrVirtCpu: %u, cpuTime: %llu\n", domInfo->state, domInfo->maxMem, domInfo->memory, domInfo->nrVirtCpu, domInfo->cpuTime);

	virDomainInterfaceStatsPtr stats;
	if(virDomainInterfaceStats(dom, "vnet0", stats, sizeof(virDomainInterfaceStatsStruct)) != 0) {
	//	printf("get interface info failed\n");
	//	exit(-1);
	} 
	printf("rx_bytes: %lld, tx_bytes: %lld\n", stats->rx_bytes, stats->tx_bytes);

	if(virDomainDestroy(dom) != 0) {
		printf("destroy dom failed.\n");
		exit(-1);
	}
	printf("%s destroy!\n", virDomainGetName(dom));
	virDomainFree(dom);
	virConnectClose(conn);
	return 0;
	
}
static void usage(const char * arg)
{
	printf("usage:\n");
	printf("\t%s create\n", arg);
	exit(-1);
}

int main(int argc, char **argv)
{
	if(argc < 2)
		usage(argv[0]);
	if(strcmp(argv[1], "create") == 0) {
		create_virtual_machine(argc - 1, &argv[1]);
	}
	printf("vir_ip = %s\n", vir_ip);
	return 0;
}
