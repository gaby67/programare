#include <stdio.h>
#include <gtk/gtk.h>
#include "socket.h"
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

enum{
	IP,
	PORT,
	FILENAME,
	COLOANE
};

typedef struct{
	char* ip;
	int port;
}Adresa;

GtkCellRenderer* cell;
GtkWidget *ip, *port, *list, *button, *label;

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
	AddColumn(list,"Adresa Client",IP);
	AddColumn(list,"Port",PORT);
	//AddColumn(list,"Nume Fisier",FILENAME);

	g_object_unref(G_OBJECT(store));
	return list;
}

GtkWidget *Labels(char* l1,char* l2){
	GtkWidget *vbox,*label,*label2;
	
	label = gtk_label_new(l1);
	label2 = gtk_label_new(l2);
	vbox = gtk_vbox_new(FALSE,2);

	gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,10);
	gtk_box_pack_start(GTK_BOX(vbox),label2,FALSE,FALSE,2);

	return vbox;
} 

GtkWidget *Fields(){
	GtkWidget *vbox;
	
	ip = gtk_entry_new();
	port = gtk_entry_new();
	vbox = gtk_vbox_new(FALSE,2);

	gtk_box_pack_start(GTK_BOX(vbox),ip,FALSE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(vbox),port,FALSE,TRUE,2);

	return vbox;
} 

GtkWidget *CreateWindow(){
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Node");
	gtk_window_set_default_size(GTK_WINDOW(window), 300,200);
	return window;
}

GtkWidget *AddFields(){
	GtkWidget* hbox;
	hbox = gtk_hbox_new(FALSE,2);
	gtk_box_pack_start(GTK_BOX(hbox),Labels("Adresa Hub:","Portul:"),FALSE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox),Fields(),TRUE,TRUE,2);
	return hbox;
}

GtkWidget *LoadItems(){
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE,2);
	label = gtk_label_new("Inceput");
	button = gtk_button_new_with_label("Connect");
	gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),AddScrollBar(CreateList()),TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(vbox),AddFields(),FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,2);
	return vbox;
}

void SendRegistration(NodeMsg* m,int hub){
	m->mtype = '1';
	send(hub,m,sizeof(NodeMsg),0);
}

void FileSize(GetFile f,GetText *t){
	struct stat64 st;
	stat64(f.filename, &st);
	t->complition = st.st_size;
}

void SelectText(GetFile f,GetText *t){
	int file = open(f.filename, O_LARGEFILE | O_RDONLY );
	lseek64(file,f.from,SEEK_CUR);
	t->length = read(file,&t->data,100);
	close(file);
}

void Cerere(int client){
	GetFile f;
	GetText t;
	do{
		recv(client,&f,sizeof(GetFile),0);
		FileSize(f,&t);
		SelectText(f,&t);
		send(client,&t,sizeof(GetText),0);
	}while(t.length > 0);
}

void ConnectToClients(int node){
	pthread_t t;
	struct sockaddr_in c;
	int client,l = sizeof(c);
	
	for( ; ; ){
		client = accept(node, (struct sockaddr *)&c,&l);	
		AddListItem(list,inet_ntoa(c.sin_addr),ntohs(c.sin_port));
		pthread_create(&t,NULL, (void*)Cerere,(void*)client);
	}
	close(node);
}

int ConnectToHub(char* address, int p){
	GetNode n;
	n.mtype = '2';
	printf("%s %d\n",address,p);
	int hub = SockConnect(address,p);
	if (hub < 0){
		gtk_entry_set_text(GTK_ENTRY(ip),"");
		gtk_entry_set_text(GTK_ENTRY(port),"");
		gtk_label_set_text(GTK_LABEL(label),"Hubul e offline");
		return 0;
	}

	return hub;
}

int GeneratePort(NodeMsg* m){
	int node;
	do{
		m->rvport = rand()%10000;
		node = SockCreate("127.0.0.1",m->rvport);
	}while(node == -1);
	return node;
}

int CreateSocket(NodeMsg* m){
		int node = GeneratePort(m);
		char text[100];
	    sprintf(text,"Am deschis Socket pe portul %d\n",m->rvport);
	    gtk_label_set_text(GTK_LABEL(label),text);
	    return node;
}

void Connection(Adresa* addr){
	NodeMsg m;
	int node = CreateSocket(&m);
	int hub = ConnectToHub((char*)addr->ip,addr->port);
	if (hub){
		gtk_label_set_text(GTK_LABEL(label),"Connected to Hub");
		SendRegistration(&m,hub);
		ConnectToClients(node);
	}
}

void ButtonPress(){
	Adresa* addr = malloc(sizeof(Adresa));
	const char* address = gtk_entry_get_text(GTK_ENTRY(ip));
	const char* p = gtk_entry_get_text(GTK_ENTRY(port));
	
	addr->ip = address;
	addr->port = atoi(p);

	pthread_t t;
	pthread_create(&t,NULL,(void*)Connection,(void *)addr);
}

int main(int argc, char* argv[]){
	GtkWidget *window;
	gtk_init(&argc,&argv);
	window = CreateWindow();

	gtk_container_add(GTK_CONTAINER(window),LoadItems());
	gtk_widget_show_all(window);
	
	g_signal_connect(button, "clicked",G_CALLBACK(ButtonPress),NULL);

	gtk_main();
	return 0;
}