#include <stdio.h>
#include <string.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

#include <wiringPi.h>

#define delayTime 250

int getIP()
{
        int fd;
        struct ifreq ifr;

        fd = socket(AF_INET, SOCK_DGRAM, 0);
        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(fd, SIOCGIFADDR, &ifr);

        close(fd);

        char *ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

        ip = strchr(ip, '.');
        ip = ip+1;
        ip = strchr(ip, '.');
        ip = ip+1;
        ip = strchr(ip, '.');
        ip = ip+1;

        return atoi(ip);
}

int main()
{
        wiringPiSetupPhys();

        // Set the relay pins to output
        pinMode(3, 1);
        pinMode(5, 1);
        pinMode(7, 1);
        pinMode(8, 1);

        // Get ip
        char ip = getIP();

        // Start intro with 3 blinks
        int i;
        for(i = 0; i != 3; i++){
                digitalWrite(3, 1);
                digitalWrite(5, 1);
                digitalWrite(7, 1);
                digitalWrite(8, 1);
                delay(delayTime);
                digitalWrite(3, 0);
                digitalWrite(5, 0);
                digitalWrite(7, 0);
                digitalWrite(8, 0);
                delay(delayTime);
        }

        delay(200);

        // I only have 4 leds
        // So the message is send in 2 series if needed
        for(i = 0; i != (ip > 15 ? 2 : 1); i++){
                digitalWrite(8, 1);
                delay(delayTime);
                digitalWrite(8, 0);
                delay(delayTime);
        }

        delay(200);
        // End intro

        // Show 192.168.1.* in binary
        if(ip & 1){
                digitalWrite(8, 1);
        }
        if(ip & 2){
                digitalWrite(7, 1);
        }
        if(ip & 4){
                digitalWrite(5, 1);
        }
        if(ip & 8){
                digitalWrite(3, 1);
        }

        if(ip >= 16){
                delay(3000);

                // Reset leds
                digitalWrite(3, 0);
                digitalWrite(5, 0);
                digitalWrite(7, 0);
                digitalWrite(8, 0);

                delay(500);
                if(ip & 16){
                        digitalWrite(8, 1);
                }
                if(ip & 32){
                        digitalWrite(7, 1);
                }
                if(ip & 64){
                        digitalWrite(5, 1);
                }
                if(ip & 128){
                        digitalWrite(3, 1);
                }
        }

        delay(5000);

        digitalWrite(3, 0);
        digitalWrite(5, 0);
        digitalWrite(7, 0);
        digitalWrite(8, 0);

        return 0;
}
