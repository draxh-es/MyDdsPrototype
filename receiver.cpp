#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(){

int sock = socket(AF_INET,SOCK_DGRAM,0);

if(sock < 0){

std::cerr << "socket olusturulamadi\n";

return 1;

}


sockaddr_in benim{};
benim.sin_family = AF_INET;
benim.sin_port =htons(7400);
benim.sin_addr.s_addr = INADDR_ANY;

if(bind(sock,(sockaddr*)&benim,sizeof(benim))<0){
std::cerr<<"bind basarisiz";

close(sock);

return 1;

}

std::cout<<"7400 portu dinleniyor... \n";

char tampon[1024];
sockaddr_in gonderen{};
socklen_t gonderen_boyut = sizeof(gonderen);

int n = recvfrom(sock,tampon,sizeof(tampon)-1,0,(sockaddr*)&gonderen,&gonderen_boyut);

if(n<0){

std::cerr<< "recvfrom basarisiz\n";


close(sock);

return 1;

}

tampon[n] = '\0';

std::cout<<"gelen mesaj:  "<< tampon<< "\n";


close(sock);
return 0;


















}
