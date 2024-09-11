#include <stdio.h>

//trades bytes between even and odd indices
void trade(void *buf, int buflen){
    //Convert buf to unsigned int
    unsigned char *p = (unsigned char *) buf;

    //dont swap the last byte if odd length

    //iterate through buffer and start "trading"
    for(int i = 0; i < buflen; i++){
        //if index is even, trade with one next to
        if(i % 2 == 0){
            unsigned char temp = p[i];
            p[i] = p[i + 1];
            p[i + 1] = temp;
        }
    }
}

int main(){

    unsigned char data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    int buflen = sizeof(data); // 7 bytes

    printf("Before trade:\n");
    for (int i = 0; i < buflen; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    
    trade(data, buflen);
    
    printf("After trade:\n");
    for (int i = 0; i < buflen; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    return 0;
}

