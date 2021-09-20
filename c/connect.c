#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

int main(int argc, char **argv)
{
    struct sockaddr_l2 addr = { 0 };
    int s, status;
    char dest[18] = "01:23:45:67:89:AB";
    struct bt_security bt_sec;

    if(argc < 3)
    {
        fprintf(stderr, "usage: %s <bt_addr> <psm>\n", argv[0]);
        exit(2);
    }

    strncpy(dest, argv[1], 18);
    unsigned short psm;
    sscanf(argv[2], "%hu", &psm);

    // allocate a socket
    printf("socket(%d, %d, %d)\n", AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(psm);
    str2ba( dest, &addr.l2_bdaddr );
    addr.l2_bdaddr_type = BDADDR_LE_RANDOM;

    // connect to server
    printf("connect(%d, ..., %d)\n", s, sizeof(addr));
    printf("addr: ");
    unsigned char *raw = &addr;
    for (int i = 0; i < sizeof(addr); i++) {
        printf("%d ", raw[i]);
    }
    printf("\n");

    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        printf("Connected ok, delaying 1 second before write\n");
        sleep(1);
        printf("Writing message\n");
        status = write(s, "hello!\n", 7);
    }

    if( status < 0 ) perror("uh oh");

    printf("Wrote the message\n");
    close(s);
}
