#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.hpp"

class Reader {
public:
    Reader(uint32_t topic_id,
           std::function<void(uint32_t, const std::string&)> callback) {
        topic_id_ = topic_id;
        callback_ = callback;

        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) {
            perror("socket");
            return;
        }

        int acik = 1;
        if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &acik, sizeof(acik)) < 0) {
            perror("SO_REUSEADDR");
        }

        sockaddr_in benim{};
        benim.sin_family = AF_INET;
        benim.sin_port = htons(7400);
        benim.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock_, (sockaddr*)&benim, sizeof(benim)) < 0) {
            perror("bind");
        }

        ip_mreq grup{};
        inet_pton(AF_INET, "239.255.0.1", &grup.imr_multiaddr);
        grup.imr_interface.s_addr = htonl(INADDR_ANY);

        if (setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                       &grup, sizeof(grup)) < 0) {
            perror("IP_ADD_MEMBERSHIP");
        }
    }

    ~Reader() {
        if (sock_ >= 0) {
            close(sock_);
        }
    }

    // Tek bir paket bekler, uygunsa callback'i cagirir.
    void bekle() {
        char tampon[1024];
        int n = recvfrom(sock_, tampon, sizeof(tampon), 0, nullptr, nullptr);
        if (n < 0) {
            perror("recvfrom");
            return;
        }
        if (n < (int)sizeof(MesajBasligi)) {
            return;
        }

        MesajBasligi baslik;
        memcpy(&baslik, tampon, sizeof(baslik));

        if (baslik.magic != MAGIC) {
            return;
        }
        if (baslik.payload_len != n - sizeof(baslik)) {
            return;
        }
        if (baslik.topic_id != topic_id_) {
            return;
        }

        std::string metin(tampon + sizeof(baslik), baslik.payload_len);
        callback_(baslik.sequence, metin);
    }

private:
    uint32_t topic_id_;
    int sock_ = -1;
    std::function<void(uint32_t, const std::string&)> callback_;
};
