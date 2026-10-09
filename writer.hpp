#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.hpp"

class Writer {
public:
    Writer(uint32_t topic_id) {
        topic_id_ = topic_id;
        seq_ = 0;

        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) {
            perror("socket");
        }

        hedef_ = sockaddr_in{};
        hedef_.sin_family = AF_INET;
        hedef_.sin_port = htons(7400);
        inet_pton(AF_INET, "239.255.0.1", &hedef_.sin_addr);
    }

    ~Writer() {
        if (sock_ >= 0) {
            close(sock_);
        }
    }

    void yaz(const std::string& payload) {
        MesajBasligi baslik{};
        baslik.magic = MAGIC;
        baslik.topic_id = topic_id_;
        baslik.sequence = seq_;
        baslik.payload_len = payload.size();

        char paket[1024];
        memcpy(paket, &baslik, sizeof(baslik));
        memcpy(paket + sizeof(baslik), payload.c_str(), payload.size());

        int toplam = sizeof(baslik) + payload.size();

        if (sendto(sock_, paket, toplam, 0,
                   (const sockaddr*)&hedef_, sizeof(hedef_)) < 0) {
            perror("sendto");
        }

        std::cout << "[Writer] topic " << topic_id_
                  << " | seq " << seq_ << " gonderildi\n";

        seq_++;
    }

private:
    uint32_t topic_id_;
    uint32_t seq_;
    int sock_;
    sockaddr_in hedef_;
};
