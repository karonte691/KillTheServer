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




#include <string>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

using namespace std;

string resolveSite(string site)
{
    hostent * record = gethostbyname(site.c_str());
    if(record == NULL)
    {
        cout << "[Error]Impossible resolve the hostname" << endl;
        return "";
    }
    in_addr * address = (in_addr * )record->h_addr;
	string ip_address = inet_ntoa(* address);
	return ip_address;
}

int getrand(int min,int max){
	return(rand()%(max-min)+min);
}
//generate random ip
char* GenRandomIp()
{
    int A=0, B=0, C=0, D=0;
    char *random_ip = "%c.%c.%c.%c";

    A=getrand(1, 255);
    B=getrand(1, 255);
    C=getrand(1, 255);
    D=getrand(1, 255);

    sprintf(random_ip, (char *)A, (char *)B, (char *)C, (char *)D);

    return random_ip;
}
