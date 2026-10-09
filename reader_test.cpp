#include <cstdlib>
#include "reader.hpp"
//asdasd
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "kullanim: ./reader_test <topic_id>\n";
        return 1;
    }
    uint32_t topic = atoi(argv[1]);

    Reader okuyucu(topic, [](uint32_t seq, const std::string& metin) {
        std::cout << "seq " << seq << " | " << metin << "\n";
    });

    std::cout << "topic " << topic << " dinleniyor\n";

    while (true) {
        okuyucu.bekle();
    }
    return 0;
}
