/**
 * @file      socket_datagram_ipc.cpp
 * @author    Atakan S.
 * @date      01/01/2019
 * @version   1.0
 * @brief     Example IPC Messaging using Unix Domain Socket Datagram.
 *
 * @copyright Copyright (c) 2019 Atakan SARIOGLU ~ www.atakansarioglu.com
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <memory.h>


int main(int argc,char **argv) {
	// Unix domain socket file address.
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "/tmp/socket_9009m");

	char paras[128], dparas[128];
	int pIdx = 0;
	char *pptr, *vptr, *ppsave, *vpsave;

	if(argc < 3 )
	{
		printf("set9009 -d 0/1(rx/tx) -f xxxx.xxx  (MHz) -a xx.x (db) -o xx.x (db) -s xx.x(db) -g 1(rx_agc) -q 1(sks_rx_agc) -m 0/1(zaibo) -e 0/1(rx_gain_mode)\n\r");
		return -1;
	}

	memset(paras, '\0', sizeof(paras));
	memset(dparas, '\0', sizeof(dparas));

	while(++pIdx < argc)
	{
		pptr = strstr(argv[pIdx], "-f");
		if(pptr != NULL)
		{
			strcat(paras, " FREQ=");
			strcat(paras, argv[++pIdx]);
		}

		pptr = strstr(argv[pIdx], "-a");
		if(pptr != NULL)
		{
			strcat(paras, " ATT=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-d");
		if(pptr != NULL)
		{
			strcat(dparas, " DEV=");
			strcat(dparas, argv[++pIdx]);
		}
        pptr = strstr(argv[pIdx], "-o");
		if(pptr != NULL)
		{
			strcat(paras, " ATTOUT=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-s");
		if(pptr != NULL)
		{
			strcat(paras, " ATTATTOUT=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-g");
			if(pptr != NULL)
		{
			strcat(paras, " RXGAINMGC=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-q");
			if(pptr != NULL)
		{
			strcat(paras, " READSKSGAIN=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-b");
			if(pptr != NULL)
		{
			strcat(paras, " SETDATABW=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-r");
		if(pptr != NULL)
		{
			strcat(paras, " RTREG=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-w");
		if(pptr != NULL)
		{
			strcat(paras, " WTREG=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-v");
		if(pptr != NULL)
		{
			strcat(paras, " WTVAL=");
			strcat(paras, argv[++pIdx]);
		}
        pptr = strstr(argv[pIdx], "-h");
			if(pptr != NULL)
		{
			strcat(paras, " RRREG=");
			strcat(paras, argv[++pIdx]);
		}
		 pptr = strstr(argv[pIdx], "-j");
			if(pptr != NULL)
		{
			strcat(paras, " WRREG=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-k");
		if(pptr != NULL)
		{
			strcat(paras, " WRVAL=");
			strcat(paras, argv[++pIdx]);
		}

	
			pptr = strstr(argv[pIdx], "-z");
		if(pptr != NULL)
		{
			strcat(paras, " DDST=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-p");
		if(pptr != NULL)
		{
			strcat(paras, " DDSA=");
			strcat(paras, argv[++pIdx]);
		}  
		pptr = strstr(argv[pIdx], "-p");
			if(pptr != NULL)
		{
			strcat(paras, " DDSO=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-m");
			if(pptr != NULL)
		{
			strcat(paras, " ZBSWITCH=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-n");
			if(pptr != NULL)
		{
			strcat(paras, " AUTOREG=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-u");
			if(pptr != NULL)
		{
			strcat(paras, " DEBUGDATA=");
			strcat(paras, argv[++pIdx]);
		}  
			pptr = strstr(argv[pIdx], "-x");
			if(pptr != NULL)
		{
			strcat(paras, " SETMONIBW=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-i");
			if(pptr != NULL)
		{
			strcat(paras, " INFO=");
			strcat(paras, argv[++pIdx]);
		}
			pptr = strstr(argv[pIdx], "-c");
			if(pptr != NULL)
		{
			strcat(paras, " CAL=");
			strcat(paras, argv[++pIdx]);
		}
		 pptr = strstr(argv[pIdx], "-y");
			if(pptr != NULL)
		{
			strcat(paras, " RMEM=");
			strcat(paras, argv[++pIdx]);
		}
		 pptr = strstr(argv[pIdx], "-t");
			if(pptr != NULL)
		{
			strcat(paras, " WMEM=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-l");
		if(pptr != NULL)
		{
			strcat(paras, " WMEMVAL=");
			strcat(paras, argv[++pIdx]);
		}
		pptr = strstr(argv[pIdx], "-e");
		if(pptr != NULL)
		{
			strcat(paras, " RXGAINMODE=");
			strcat(paras, argv[++pIdx]);
		}
	}
	strcat(dparas, paras);
	printf("set_dev:: %s\n", dparas);
	/*
	printf("paras: %s\n", paras);

	pptr = strtok_r(paras, " ", &ppsave);
	while(pptr != NULL)
	{
		printf("token :: %s\n", pptr);
		vptr = strtok_r(pptr, "=", &vpsave);
		if(strcmp(vptr, "FREQ")==0)
		{
			printf("set freq: ... ... ....");
			vptr = strtok_r(NULL, "=", &vpsave);
			printf("val :: %s\n", vptr);
		}
		if(strcmp(vptr, "ATT")==0)
		{
			printf("set att: ... ... ....");
			vptr = strtok_r(NULL, "=", &vpsave);
			printf("val :: %s\n", vptr);
		}
		pptr = strtok_r(NULL, " ", &ppsave);
	}
	//printf("argv[1]: %s;; %d\n", argv[1], strnlen(argv[1],24));
	return 0;  a b c d e f g h i j k l m n o p q r s t u v w x y z      
	*/

		// Create a unix domain socket.
		int fd;
		if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
			printf("Sender: Cannot create socket");
			close(fd);
			return -1;
		} 
		size_t data_len = strnlen(dparas, 120);
         if(sendto(fd, dparas, data_len, 0, (struct sockaddr *)&address, sizeof(address)) < data_len) {
	   //if(sendto(fd, dparas, strnlen(dparas,120), 0, (struct sockaddr *)&address, sizeof(address)) < sizeof(dparas)) {
				printf("Client: Cannot send %s", dparas );
				close(fd);
				return -1;
			}
			else {
				printf ("set new paras: %s\n", dparas);
			}

		// Close the socket.
		close(fd);

	// Exit.
	return 0;
}