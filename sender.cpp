#include<iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.hpp"
#include <cstring>

int main(){

int sock = socket(AF_INET,SOCK_DGRAM,0);

if(sock < 0){
	std::cerr << "socket olusturulamadi\n";
	return 1;

}

sockaddr_in hedef{};
hedef.sin_family = AF_INET;
hedef.sin_port = htons(7400);
inet_pton(AF_INET,"127.0.0.1",&hedef.sin_addr);

uint32_t sayac = 0;

while(true) {

std::string payload = "merhaba dds #"+ std::to_string(sayac);

MesajBasligi baslik{};
baslik.magic = MAGIC;
baslik.topic_id = 1;
baslik.sequence = 0;
baslik.payload_len = payload.size();

char paket[1024];

memcpy(paket,&baslik,sizeof(baslik));
memcpy(paket+sizeof(baslik),payload.c_str(), payload.size());

int toplam = sizeof(baslik) + payload.size();


sendto(sock,paket,toplam,0,(sockaddr*)&hedef,sizeof(hedef));

std::cout<<"gönderildi sequence "<<sayac<<"\n";

sayac++;

sleep(1);

}
close(sock);

return 0;


}

