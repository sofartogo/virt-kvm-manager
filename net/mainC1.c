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

#include <gtk/gtk.h>
#include <sys/stat.h>
#define PORT 3001
#define MAXDATASIZE 5000

int number;

void get_network_flow(char buf[1024], long long * rx_bytes, long long * tx_bytes)
{
	char *s1;
	char *s2;
	s1 = strstr(buf, "rx_bytes");
	s1 += 10;
	for(s2 = s1; *s2 != ','; s2 ++)
		;
	*s2 = '\0';
	*rx_bytes = atoll(s1);
	*s2 = ',';
	//printf("rx_bytes = %lld\n", *rx_bytes);
	s1 = strstr(buf, "tx_bytes");
	s1 += 10;
	for(s2 = s1; *s2 != '\n'; s2 ++)
		;
	*s2 = '\0';
	*tx_bytes = atoll(s1);
	//printf("tx_bytes = %lld\n", *tx_bytes);

}


/* *******************  netflow  ******************** */
static int timer; 
static struct timeval startTime;
static struct timeval endTime;
static float testnum = 1;
void cb_quit() 
{ 
	gtk_timeout_remove (timer); 
	gtk_main_quit(); 
} 

float cal_load (void) 
{ 	
	long long  rx_bytes, tx_bytes;
	int  sockfd, nbytes;
	char buff[1024] = {0};
	struct hostent *he;
	struct sockaddr_in srvaddr;

	//printf("netflow: %d\n", number);
	sprintf(buff, "netflow %d", number);
  
	if((he = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		exit(-1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("netflow socket error");
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

	if(write(sockfd, buff, strlen(buff)) == -1) {
		perror("send error");
		exit(-1);
	}

	if((nbytes = read(sockfd, buff, MAXDATASIZE)) == -1) {
			perror("read error");
			exit(-1);
	}

	buff[nbytes] = '\0';
	//printf("%s", buf);
	get_network_flow(buff, &rx_bytes, &tx_bytes);
	printf("rx_bytes = %lld, tx_bytes = %lld\n", rx_bytes, tx_bytes);
	close(sockfd);


	FILE * fp; 
	static long pre_user, pre_nice, pre_system, pre_idle;  
	long user, nice, system, idle; 
	long d_user, d_nice, d_system, d_idle; 
	static long total; 
	float scale; 
	char buf[1024]; 

	if ((fp = fopen ("/proc/stat", "r")) != NULL){ 
		while (fgets (buf, sizeof(buf), fp)){ 
			if( *buf == 'c' && *(buf+1) == 'p') break; 
		} 
		fclose (fp); 
	} 
	else 
		return 0.0; 

	static long pre_traffic;
	long traffic;
	long d_traffic;

	int Time;

	if (gettimeofday(&endTime, NULL) == -1) {
   	printf("Failed to get start time\n");
	}
	Time = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
	startTime = endTime;
	sscanf(buf, "cpu %ld %ld %ld %ld", &user, &nice, &system, &idle); 
	//printf("user:%ld nice:%ld system:%ld idle:%ld \n", user, nice ,system ,idle);

	//calculate delta value  

	testnum += 10;
	//traffic = testnum;
	
	//d_traffic = traffic - pre_traffic;
	d_traffic  = 1;




	pre_traffic = traffic;

	return d_traffic/0.2;//(d_traffic * 1000000)/ Time;

}

gint Repaint (gpointer da) 
{ 
	GtkWidget * drawing_area = (GtkWidget *)da; 
	GdkDrawable * drawable = drawing_area->window;
	GdkDrawable * tdrawable = drawing_area->window; 

	GdkGC *gc_rchart, *gc_tchart, *gc_rtext, *gc_ttext; 
	GdkColor color; 

	static unsigned rcoory[401]; 
	static unsigned tcoory[401]; 
	int da_width, da_height; 
	int x; 
	float load; 
	char buf[30]; 

	//green rchart 
	gc_rchart = gdk_gc_new (drawing_area->window); 
	color.red = 0; 
	color.green = 0xffff; 
	color.blue = 0; 
	gdk_gc_set_rgb_fg_color(gc_rchart, &color ); 
	//green tchart 
	gc_tchart = gdk_gc_new (drawing_area->window); 
	color.red = 0; 
	color.green = 0xffff; 
	color.blue = 0; 
	gdk_gc_set_rgb_fg_color(gc_tchart, &color );
	//red rtext
	gc_rtext = gdk_gc_new (drawing_area->window); 
	color.red = 0xffff; 
	color.green = 0; 
	color.blue = 0; 
	gdk_gc_set_rgb_fg_color(gc_rtext, &color ); 
	//red ttext
	gc_ttext = gdk_gc_new (drawing_area->window); 
	color.red = 0xffff; 
	color.green = 0; 
	color.blue = 0; 
	gdk_gc_set_rgb_fg_color(gc_ttext, &color ); 

	// size 
	da_width = drawing_area->allocation.width; 
	da_height = drawing_area->allocation.height; 
	
	// recieve
	gdk_draw_rectangle (drawable, drawing_area->style->white_gc, TRUE, 0, 0, da_width, 0.5 * da_height); 

	// chart line
	for (x=0; x<400; x++) 
		rcoory[x] = rcoory[x+1]; 

	load = cal_load();
	rcoory[x] = (int)(load * 0.03 * da_height); 

	for(x=0;x<da_width;x++){ 
		gdk_draw_line (drawable, gc_rchart, x, 0.5 * da_height, x, 0.5 * da_height - rcoory[x]); 
	} 

	// following code for drawing rtext 
	sprintf (buf, "R Flow:%.1f kb/s", load ); 

	PangoLayout *layout = gtk_widget_create_pango_layout( da, buf );
	//set font
	PangoFontDescription *fontdesc = pango_font_description_from_string( "Luxi Mono 12" ); 
	pango_layout_set_font_description( layout, fontdesc ); 
	gdk_draw_layout( drawable, gc_rtext, 5, 5, layout ); 
	pango_font_description_free( fontdesc ); 
	g_object_unref( layout ); 

	sprintf (buf, "R Total flow:%.1f kb", testnum ); 
	PangoLayout *layout1 = gtk_widget_create_pango_layout( da, buf );
	//set font
	PangoFontDescription *fontdesc1 = pango_font_description_from_string( "Luxi Mono 12" ); 
	pango_layout_set_font_description( layout1, fontdesc1 ); 
	gdk_draw_layout( drawable, gc_rtext, 180, 5, layout1 ); 
	pango_font_description_free( fontdesc1 ); 
	g_object_unref( layout1 ); 

	// 发送
	gdk_draw_rectangle (tdrawable, drawing_area->style->white_gc, TRUE, 0, 201, da_width, 0.5 * da_height); 

	// chart line
	for (x=0; x<400; x++) 
		tcoory[x] = tcoory[x+1]; 

	load = cal_load(); 
	tcoory[x] = (int)(load * 0.05 * da_height); 

	for(x=0;x<da_width;x++){ 
		gdk_draw_line (tdrawable, gc_tchart, x, da_height, x, da_height - tcoory[x]); 
	} 
	// following code for drawing ttext 
	sprintf (buf, "T Flow:%.1f kb/s", load ); 

	PangoLayout *layout2 = gtk_widget_create_pango_layout( da, buf );
	//set font
	PangoFontDescription *fontdesc2 = pango_font_description_from_string( "Luxi Mono 12" ); 
	pango_layout_set_font_description( layout2, fontdesc2 ); 
	gdk_draw_layout( drawable, gc_ttext, 5, 205, layout2 ); 
	pango_font_description_free( fontdesc2 ); 
	g_object_unref( layout2 ); 

	sprintf (buf, "R Total flow:%.1f kb", testnum ); 
	PangoLayout *layout3 = gtk_widget_create_pango_layout( da, buf );
	//set font
	PangoFontDescription *fontdesc3 = pango_font_description_from_string( "Luxi Mono 12" ); 
	pango_layout_set_font_description( layout3, fontdesc3 ); 
	gdk_draw_layout( drawable, gc_ttext, 180, 205, layout3 ); 
	pango_font_description_free( fontdesc3 ); 
	g_object_unref( layout3 ); 

	g_object_unref( G_OBJECT(gc_rchart) ); 
	g_object_unref( G_OBJECT(gc_tchart) );
	g_object_unref( G_OBJECT(gc_rtext) ); 
	g_object_unref( G_OBJECT(gc_ttext) ); 

	return TRUE; 
} 


static char netflow_doc[] = 
	"virt: netflow: netflow info of virtual machine\n"
	"--number must be provided.";

static char netflow_args_doc[] = 
	"";

static struct argp_option netflow_options[] = {
	{"number", 'n', "target number", 0 ,"number of virtual machine", 0},
	{NULL, '\0', NULL, 0, NULL ,0},
};

static error_t 
parse_netflow_opt(int key, char * arg, 
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

static struct argp netflow_argp = {netflow_options, parse_netflow_opt,
	netflow_args_doc, netflow_doc, NULL, NULL, NULL};

int netflow(int argc, char ** argv) 
{ 
	int idx;
	int err = argp_parse(&netflow_argp, argc, argv, 
			ARGP_IN_ORDER, &idx, NULL);
	if (err != 0) {
		printf("argp_parse error: %d\n", err);
		exit(-1);
	}
	if(number < 1 || number > 100) {
		printf("n is between 1 and 100\n");
		exit(-1);
	}

	GtkWidget *window; 
	GtkWidget *drawing_area; 

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL); 
	gtk_window_set_title (GTK_WINDOW(window), "Network traffic"); 
	drawing_area = gtk_drawing_area_new (); 
	gtk_container_add (GTK_CONTAINER(window), drawing_area); 

	gtk_signal_connect(GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC(cb_quit), NULL); 

	g_signal_connect( drawing_area, "expose_event", G_CALLBACK(Repaint), NULL ); 
 
	gtk_drawing_area_size (GTK_DRAWING_AREA(drawing_area), 400, 400); 

	gtk_widget_show (drawing_area); 
	gtk_widget_show(window); 
	if (gettimeofday(&startTime, NULL) == -1) {
   	printf("Failed to get start time\n");
		return -1;
	}
	timer = gtk_timeout_add (200, Repaint, (gpointer) drawing_area); 
	gtk_main(); 

	return 0; 
}


/* *******************  listall  ******************** */
void listall(int argc, char ** argv)
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
	//if(strcmp(buf, "listall") == 0) 
	//	printf("buf is listall\n");
  
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
	long long  rx_bytes, tx_bytes;
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

	//printf("list: %d\n", number);
	sprintf(buf, "list %d", number);
	//if(strcmp(buf, "list 1") == 0) 
	//	printf("buf is list 1\n");
  
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
	printf("%s", buf);
	//get_network_flow(buf, &rx_bytes, &tx_bytes);
	//printf("rx_bytes = %lld, tx_bytes = %lld\n", rx_bytes, tx_bytes);
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
	//printf("create %d\n", number);
	sprintf(buf, "create %d", number);
	//if(strcmp(buf, "create 1") == 0) 
	//	printf("buf is create 1\n");
  
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
	printf("\t%s netflow -n (1--100)\n", arg);
	printf("\t%s listall\n", arg);
	exit(-1);
}

/* **********************  main  ****************** */
int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	if(argc < 2)
		usage(argv[0]);
	if(strcmp(argv[1], "create") == 0) {
		create(argc - 1, &argv[1]);
	} else if (strcmp(argv[1], "list") == 0) {
		list(argc - 1, &argv[1]);
	} else if (strcmp(argv[1], "listall") == 0) {
		listall(argc - 1, &argv[1]);
	} else if (strcmp(argv[1], "netflow") == 0) {
		netflow(argc - 1, &argv[1]);
	} else
		usage(argv[0]);
	return 0;
}
