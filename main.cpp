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

#include "synattack.h"
#include "httpattack.h"
#include "killapacheattack.h"
#include "icmpattack.h"

using namespace std;

int main()
{
    int sc=0;
    cout << "#############################" << endl;
    cout << "#KTS v 0.1                  #" << endl;
    cout << "#############################" << endl;
    cout << "\n\n###############################################################################" << endl;
    cout << "Whatever you do with this tool is uniquely your responsibility."
         << "If you are not \nauthorized to punch holes in the network you are attacking be aware that such \naction"
         << "might get you in trouble with a lot of law enforcement agencies." << endl;
    cout << "###############################################################################" << endl << endl;
    cout << "Menu:" << endl;
    cout << "1)Syn Flood attack" << endl;
    cout << "2)ICMP Flood attack" << endl;
    cout << "3)HTTP web attack using HTTP protocol" << endl;
    cout << "4)HTTP web attack using Apache Vuln(Killapache)" << endl;
    cout << "choose: ";
    cin >> sc;
    switch(sc)
    {
        case 1:
            SynFloodAttackInit();
            break;
        case 2:
            ICMPFloodInit();
            break;
        case 3:
            HTTPFloodAttackInit();
            break;
        case 4:
            KillApacheAttackInit();
            break;
        default:
            cout << "[Error] invalid argument";
    }
    return 0;
}
