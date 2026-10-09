#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "message.hpp"
#include <cstdlib>


int main(int argc,char* argv[]){
if(argc < 2){
std::cerr<< "kullanım ./receiver <topic_id>\n";
return 1;

}

uint32_t ilgilenilen_topic = atoi(argv[1]);

int sock = socket(AF_INET,SOCK_DGRAM,0);

if(sock < 0){

std::cerr << "socket olusturulamadi\n";

return 1;

}


sockaddr_in benim{};
benim.sin_family = AF_INET;
benim.sin_port =htons(7400);
benim.sin_addr.s_addr = INADDR_ANY;

int acik = 1;
if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR, &acik,sizeof(acik))<0){

perror("SO_REUSEADDR");

close(sock);
return 1;


}

if(bind(sock,(sockaddr*)&benim,sizeof(benim))<0){
std::cerr<<"bind basarisiz";

close(sock);

return 1;

}

ip_mreq grup{};

inet_pton(AF_INET,"239.255.0.1",&grup.imr_multiaddr);
grup.imr_interface.s_addr = htonl(INADDR_ANY);


if(setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,
		&grup,sizeof(grup))<0){

perror("IP_ADD_MEMBERSHIP");
close(sock);
return 1;



}


std::cout<<"7400 portu dinleniyor  topic  "<<ilgilenilen_topic<<"\n";

while(true){

char tampon[1024];
sockaddr_in gonderen{};
socklen_t gonderen_boyut = sizeof(gonderen);

int n = recvfrom(sock,tampon,sizeof(tampon),0,(sockaddr*)&gonderen,&gonderen_boyut);

if(n<0){

std::cerr<< "recvfrom basarisiz\n";


close(sock);

return 1;

}

if(n<(int)sizeof(MesajBasligi)){

	std::cerr<<"paket cok kisaa:  "<<n<<"  byte\n";
	close(sock);
	return 1;


}

MesajBasligi baslik;
memcpy(&baslik,tampon,sizeof(baslik));

if(baslik.magic != MAGIC){

std::cerr<<"magic yanlis, paket atiliyor\n";

close(sock);

return 1;

}

uint32_t gercek_payload = n- sizeof(baslik);

if(baslik.payload_len != gercek_payload){

	std::cerr<<"payload uzunlugu uyusmuyor\n";

	close(sock);

}

if(baslik.topic_id != ilgilenilen_topic){
continue;
}


std::string metin(tampon + sizeof(baslik), baslik.payload_len);

std::cout<<"topic id :   "<< baslik.topic_id 
<<"sequence :   "<< baslik.sequence <<
"payload_len :   "<< baslik.payload_len<<"|" << metin <<"\n";
}

close(sock);
return 0;

}
