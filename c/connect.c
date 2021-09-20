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

    if(argc < 2)
    {
        fprintf(stderr, "usage: %s <bt_addr>\n", argv[0]);
        exit(2);
    }

    strncpy(dest, argv[1], 18);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(192);
    str2ba( dest, &addr.l2_bdaddr );
    addr.l2_bdaddr_type = BDADDR_LE_RANDOM;
    
    printf("About to connect\n");
    sleep(2);

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        printf("Connected\n");
        sleep(1);
        printf("Writing message\n");
        status = write(s, "hello!\n", 7);
    }

    if( status < 0 ) perror("uh oh");

    printf("Wrote the message\n");
    sleep(5);
    printf("Closing\n");

    close(s);
}
