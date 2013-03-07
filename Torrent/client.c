#include <stdio.h>
#include <gtk/gtk.h>
#include "socket.h"
#include <unistd.h>
#include <fcntl.h>
GtkWidget *window,*progress,*vbox,*pick,*button;
GtkWidget *textfield,*ConnectNodes;
NodList nodlist;

typedef struct {
	int id;
	char *filename;
} Thread;

typedef struct{
	char* filename;
	int size;
} FileSize;


char* TorrentFile;
FileSize fs;

void MsgBox(char* text){
	GtkWidget* dialog = gtk_message_dialog_new (
	 			  GTK_WINDOW(window),
                  GTK_DIALOG_DESTROY_WITH_PARENT,
                  GTK_MESSAGE_INFO,
                  GTK_BUTTONS_CLOSE,
                  "%s",text);
	 gtk_dialog_run (GTK_DIALOG (dialog));
	 gtk_widget_destroy (dialog);
}

void SetProgressBar(double percent){
	gtk_widget_show(progress);
	if ((int)(percent*100) >= 0){
		char text[20];
		sprintf(text,"%d%%",(int)(percent*100));
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),percent);
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress), text);
		if (percent == 1){
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress), "Download Complete");
		}
	}
}

double GetSize(){
	struct stat64 st;
	stat64(fs.filename, &st);
	double comp = (double)(st.st_size)/fs.size;
	return comp;
}

void SetText(char* filename,GetText t,GetFile file){
	int f = open(filename, O_CREAT|O_WRONLY|O_LARGEFILE , 0777);
	lseek(f,file.from,SEEK_SET);
	write(f,t.data,t.length);
	close(f);
}

void Scriere(GetText t, GetFile file){
	int f;
	char name[100];
	sprintf(name,"./tmp/%s",file.filename);
	pthread_t tid;

	fs.filename = name;
	fs.size = t.complition;
	SetText(name,t,file);	
}


GetFile SetSendPackage(Thread* thread){
	GetFile f;
	f.mtype = '4';
	strcpy(f.filename,thread->filename);
	f.from = thread->id*100; 
	return f;
}

int ConnectToNode(struct sockaddr_in n){
	int server = socket(AF_INET, SOCK_STREAM, 0);
	int err = connect(server, (struct sockaddr*)&n,sizeof(struct sockaddr_in));
	if (err < 0){
		printf("Eroare la connect la portul %d\n",ntohs(n.sin_port));
		return 0;
	}
	return server;
}

void Conexiune(Thread* thread){
	GetText t;
	GetFile f = SetSendPackage(thread);
	int server = ConnectToNode(nodlist.addr[thread->id]);
	if (server){
		do{
		 	send(server, &f, sizeof(f), 0);
			recv(server, &t, sizeof(GetText),0);
			Scriere(t,f);
			f.from = nodlist.length*100 + f.from;
		}while(t.length > 0);
	}
	close(server);
}

void CreateThreads(char* file){
	pthread_t t[nodlist.length];
	int i;
	for(i=0;i<nodlist.length;i++){
		Thread thread;
		thread.id=i;
		thread.filename = file;
		pthread_create(&t[i],NULL,(void*)Conexiune,(void*)&thread);
	}
	for(i=0;i<nodlist.length;i++)
 		pthread_join(t[i],NULL);
}

void GetNodList(int hub){
	GetNode n;
	n.mtype = '2';

	send(hub,&n,sizeof(GetNode),0);
	recv(hub,&nodlist,sizeof(NodList),0);

	char nrnod[30];
	sprintf(nrnod,"Noduri Conectate %d",nodlist.length);
	gtk_label_set_text(GTK_LABEL(ConnectNodes),nrnod);
}


int ConnectToHub(char* ip, char* p){
	GetNode n;
	n.mtype = '2';
	int port = atoi(p);
	int hub = SockConnect(ip,port);
	if (hub < 0){
		gtk_entry_set_text(GTK_ENTRY(textfield),"");
		gtk_label_set_text(GTK_LABEL(ConnectNodes),"Hubul e offline");
		return 0;
	}

	return hub;
}

void PrintSize(){
	double comp = 0;
	while (1){
		comp = GetSize();
		SetProgressBar(comp);
		sleep(1);
	}
}

void ReadTorrentFile(char filename[]){
	FILE* f = fopen(filename, "r");
	char lines[3][20];
	int i,length;

	for(i=0;i<3;i++){
		fgets(lines[i],sizeof(lines[i]),f);
		length = strlen(lines[i]) - 1;
		if (lines[i][length] == '\n'){
			lines[i][length] = '\0';
		}
	}	
	int hub = ConnectToHub(lines[0],lines[1]);
	if (hub){
		pthread_t t;
		pthread_create(&t,NULL,(void*)PrintSize,NULL);
		
		GetNodList(hub);
		CreateThreads(lines[2]);
	}
}

GtkWidget* ProgressBar(){
	progress = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),0);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress), "0%");	
	gtk_progress_bar_set_orientation(
		GTK_PROGRESS_BAR(progress),
		GTK_PROGRESS_LEFT_TO_RIGHT
	);	
	return progress;	
}

void PickTorrent(){
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new(
		"Open File",
		GTK_WINDOW(window),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL,0,
		GTK_STOCK_OPEN, 1,
		NULL);
	gtk_dialog_run(GTK_DIALOG (dialog));
	TorrentFile = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	gtk_widget_destroy(dialog);
	
	gtk_entry_set_text(GTK_ENTRY(textfield),TorrentFile);

}

static void StartDownload(){
	pthread_t t[2];
	pthread_create(&t[0],NULL,(void*)ReadTorrentFile,(void*)TorrentFile);
}

void CreateWindow(){
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Client");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 20);
}

void Buttons(){
	pick = gtk_button_new_with_label("Pick");
	button = gtk_button_new_with_label("Download");

	g_signal_connect(pick,"clicked", G_CALLBACK(PickTorrent), NULL);
	g_signal_connect(button,"clicked", G_CALLBACK(StartDownload), NULL);
}

GtkWidget* HorizontalPanel(){
	GtkWidget *hbox,*label,*message;

	hbox = gtk_hbox_new(FALSE,2);
	textfield = gtk_entry_new();
	label = gtk_label_new("Torrentul:");

	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(hbox),textfield,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox),pick,FALSE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,TRUE,2);
	return hbox;
}


int main(int argc , char *argv[]){	
	gtk_init(&argc,&argv);
	vbox = gtk_vbox_new(FALSE,2);
	CreateWindow();
	Buttons();
	ConnectNodes = gtk_label_new("No Torrent Selected");
	gtk_box_pack_start(GTK_BOX(vbox),HorizontalPanel(),FALSE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(vbox),ConnectNodes,FALSE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(vbox),ProgressBar(),FALSE,TRUE,2);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	gtk_widget_show_all(window);
	gtk_widget_hide(progress);

	gtk_main();
	return 0;	 
}