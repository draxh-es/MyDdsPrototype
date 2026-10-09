#include "writer.hpp"

int main(){

Writer imu(1);
Writer sicaklik(2);


imu.yaz("merhaba");
imu.yaz("uuuuu");
sicaklik.yaz("ahahah");

return 0;
}
