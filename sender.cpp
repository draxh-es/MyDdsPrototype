#include<iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.hpp"
#include <cstring>

void gonder(int sock,const sockaddr_in& hedef,uint32_t topic_id
,uint32_t sequence,const std::string& payload){

MesajBasligi baslik{};
baslik.magic = MAGIC;
baslik.topic_id = topic_id;
baslik.sequence = sequence;
baslik.payload_len = payload.size();

char paket[1024];

memcpy(paket,&baslik,sizeof(baslik));
memcpy(paket+sizeof(baslik),payload.c_str(), payload.size());



int toplam = sizeof(baslik) + payload.size();

    if (sendto(sock, paket, toplam, 0,
               (const sockaddr*)&hedef, sizeof(hedef)) < 0) {
        perror("sendto");
    }

}

int main(){

int sock = socket(AF_INET,SOCK_DGRAM,0);

if(sock < 0){
	std::cerr << "socket olusturulamadi\n";
	return 1;

}

sockaddr_in hedef{};
hedef.sin_family = AF_INET;
hedef.sin_port = htons(7400);
inet_pton(AF_INET,"239.255.0.1",&hedef.sin_addr);
uint32_t tur = 0;
uint32_t seq1 = 0;
uint32_t seq2 = 0;



while(true) {

gonder(sock,hedef,1,seq1,"merhaba dds #" + std::to_string(seq1));

std::cout << "topic 1 | seq" << seq1 <<" gonderildi\n";

seq1++;

if( tur%2 == 0){

gonder(sock,hedef,2,seq2,"sicaklik="+std::to_string(20+seq2%5));
std::cout<< "topic 2 | seq " << seq2 <<" gonderildi\n";
seq2++;
}
tur++;
sleep(1);
}
close(sock);
return 0;


}

