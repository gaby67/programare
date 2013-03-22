#include <stdio.h>
#include <gtk/gtk.h>
#include "socket.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

enum{
	IP,
	PORT,
	COLOANE
};

GtkCellRenderer* cell;
GtkWidget *list, *textarea;
NodList nodlist; 

void AddListItem(GtkWidget *w,char * ip, int port){
	GtkListStore *store;
	GtkTreeIter iter;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(w)));
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(
		store,
		&iter,
		IP, ip,
		PORT, port,
		-1
	);	
}	

void AddColumn(GtkWidget *l, char* column_name,int i){
	GtkTreeViewColumn *column;
	column = gtk_tree_view_column_new_with_attributes(
		column_name,cell,"text",i,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(l), column);
}

GtkWidget *AddScrollBar(GtkWidget *w){
	GtkWidget *sc_win;
	sc_win = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sc_win),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(sc_win), w);
	return sc_win;
}

GtkWidget *CreateList(){
	GtkListStore *store;

	store = gtk_list_store_new(COLOANE, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);
	list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

	cell = gtk_cell_renderer_text_new();
	AddColumn(list,"Adresa Nod",IP);
	AddColumn(list,"Port Nod",PORT);
	
	g_object_unref(G_OBJECT(store));
	return list;
}

GtkWidget *LoadItems(){
	GtkWidget *vbox,*label2;
	vbox = gtk_vbox_new(FALSE,2);
	label2 = gtk_label_new("Console:");

	textarea = gtk_text_new(NULL, NULL);
 	gtk_text_set_editable(GTK_TEXT(textarea), FALSE);

	gtk_box_pack_start(GTK_BOX(vbox),AddScrollBar(CreateList()),TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(vbox),label2,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(vbox),textarea,TRUE,TRUE,2);
	return vbox;
}

GtkWidget *CreateWindow(){
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Hub");
	gtk_window_set_default_size(GTK_WINDOW(window), 300,350);
	return window;
}

GtkWidget ConsoleShow(char * text){

	time_t tim = time(NULL);
	struct tm tm = *localtime(&tim);

	char t2[100];
	sprintf(t2,"%s at %d-%d-%d %d:%d:%d\n",text,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		
	gtk_text_insert(
		GTK_TEXT(textarea),
		NULL,
		NULL,
		NULL,
		t2,
		-1
	);
}

void ConexiuniNoduri(struct sockaddr_in* nod){
	if (nodlist.length == 10){
		ConsoleShow("Numarul maxim de noduri a fost atins!");
		return;
	}
	AddListItem(list,inet_ntoa(nod->sin_addr),ntohs(nod->sin_port));
	nodlist.addr[nodlist.length++] = *nod;

	char console[100];
	sprintf(console,"Numarul de noduri %d",nodlist.length);
	ConsoleShow(console);
}

void ListaNoduri(int client){
	ConsoleShow("S-a conectat un client");
	send(client,&nodlist,sizeof(NodList),0);
}

void NodeConnected(NodeMsg* msg,struct sockaddr_in c){
	pthread_t t;
	c.sin_port = htons(msg->rvport);
	pthread_create(&t,NULL, (void*)ConexiuniNoduri,(void*)&c);
}

void ClientConnected(int client){
	pthread_t t;
	pthread_create(&t,NULL, (void*)ListaNoduri,(void*)client);
}

void ClientOrNod(int client,struct sockaddr_in c){
	void * msg = malloc(sizeof(char*));
	char request;
	recv(client,msg,sizeof(void*),0);
	memcpy(&request,msg,sizeof(char));
	if (request == '1'){
		NodeConnected((NodeMsg*)msg,c);
	}
	if (request == '2'){
		ClientConnected(client);
	}
}

int CreateSocket(){
	int server;
	do{
		server = SockCreate("127.0.0.1",12345);	
		ConsoleShow("Eroare la conexiune");
		sleep(1);
	}while(server < 0);
	ConsoleShow("Conectare Reusita");

	return server;
}

void Conexiune(){
	int client;
	int server = CreateSocket();
	struct sockaddr_in c;
	int l = sizeof(c);

	for( ; ; ){
		int client = accept(server, (struct sockaddr*)&c,&l);
		ClientOrNod(client,c);
	}
}

int main(int argc, char* argv[]){
	GtkWidget *window;
	pthread_t t;
	gtk_init(&argc,&argv);
	window = CreateWindow();

	pthread_create(&t,NULL,(void*)Conexiune,NULL);
	gtk_container_add(GTK_CONTAINER(window),LoadItems());
	gtk_widget_show_all(window);
	
	gtk_main();
	return 0;
}