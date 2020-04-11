/**
    quick and dirty to script a tty
    Copyright:   Marius C.  https::github.com/comarius
*/
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <iostream>
#include "tty.h"

#define BPS_SPEED   115200
#define IMPOSIBLE_STRING "_im_po_si_b_le_string_"

int main(int argc, char *argv[])
{
    bool debug    = false;
    bool allout   = false;
	bool sendonly = false;
	bool dump = false;
	int  repeat   = 0;
	int  repeatint   = 1;
    int  timeout  = 300;
	uint8_t		wada[128];
	int			lwada=0;
	
    if(argc<3){	//		1		2		3	   4	
        std::cout << "/dev/tty COMMAND EXPECT <SEND> <REP CNT INT> <GET> <DEBUG>  \n";
        return 1;
    }
	
	std::string expect = argv[3];
	std::string tos = argv[2];
	std::string received;
	
	if(expect=="*")
    {
        expect = IMPOSIBLE_STRING;
        allout = true;
        timeout = 5;
    }
	int irepeat = 0;
	for(int p=4;p<argc;p++)
	{
		std::string verb = argv[p];
		
		if(verb=="GET")
		{
			allout=true;
			continue;
		}
		if(verb=="DEBUG")
		{
			debug=true;
			continue;
		}
		if(verb=="REP") // <REP CND TOUT>
		{
			irepeat = 2; // bnext [arams are counter and delay between repeats
			continue;
		}
		if(verb=="SEND")	// send only
		{
			sendonly=true;
			continue;
		}
		if(::atoi(argv[p]))
		{
			if(irepeat==2)
			{
				repeat=atoi(argv[p]);
				irepeat--;
				continue;
			}
			if(irepeat==1)
			{
				repeatint=atoi(argv[p]);
				irepeat--;
				continue;
			}
			timeout=atoi(argv[p]);
		}
	}


    Tty t(argv[1],BPS_SPEED,"8N1");
    if(t.iopen(3))
    {
        int sent=0;
        int l = 0;
        std::string tos = argv[2];

        if (tos=="UP"){
            l=1;
            uint8_t c[]={0x18,0,0};
			::memcpy(wada,c,2);
			lwada=1;
			if(debug) std::cout << "<UP\n";
            sent= t.bwrite((const uint8_t*)c, 1);
        }
        else if (tos=="CTRLC"){
            l=1;
            uint8_t c[]={0x3,0};
			::memcpy(wada,c,2);
			lwada=1;
			if(debug) std::cout << "<CTRLC\n";
            sent= t.bwrite((const uint8_t*)c, 1);
        }
		else if (tos=="CR"){
            l=1;
            uint8_t c[]={13,0};
			::memcpy(wada,c,1);
			lwada=1;
			if(debug) std::cout << "<CR\n";
            sent = t.bwrite((const uint8_t*)c, 1);
        }
		else if (tos=="CRLF"){
            l=2;
            uint8_t c[]={13,10,0};
			::memcpy(wada,c,3);
			lwada=2;
			if(debug) std::cout << "<CR\n";
            sent = t.bwrite((const uint8_t*)c, 2);
        }
		else if (tos=="LF"){
            l=1;
            uint8_t c[]={10,0};
			::memcpy(wada,c,2);
			lwada=2;
			if(debug) std::cout << "<CR\n";
            sent = t.bwrite((const uint8_t*)c, 1);
        }
        else
        {
            tos+="\r\n";
            l = tos.length();
			if(debug) std::cout << tos << "\n";
			::memcpy(wada,tos.c_str(),l);
			lwada=l;
            sent= t.bwrite((const uint8_t*)tos.c_str(), l);
        }
        if(sent != l){
            std::cout << "#error\n";
            return -1;
        }
		if(sendonly)
		{
			return 0;
		}
        
        time_t      now = time(0);
        time_t fut = time(0) + timeout;
        if(debug) std::cout << "#repeating " << repeat <<  " times\r\n";
		
        while(time(0) < fut)
        {
            size_t  rec;
            uint8_t out[12];    /*  try to catch almost every line and react */
            if((rec=t.bread(out,sizeof(out)-1))>0)
            {
                if(debug) std::cout << ">{" << out <<  "}"  << std::flush;
                received.append((const char*)out,rec);
                if(received.find(expect)!=std::string::npos)
                {
					if(debug) std::cout << "\r\n# String:" << expect << " was found \r\n";
					::msleep(repeatint);
					if(repeat>0)
					{
						if(debug) 
						{
							char outhex[128]={0};
							char dig[32];

							for(int c=0;c<lwada;c++)
							{
								sprintf(dig,"%02X-", (int)wada[c]);
								strcat(outhex,dig);
							}
							if(debug) std::cout << "<{" << outhex << "}\n";
						}
	                    t.bwrite((const uint8_t*)wada, lwada);
						::msleep(repeatint);
						--repeat;
						continue;
					}
					::msleep(repeat*repeatint*2);
					if(allout && (rec=t.bread(out,sizeof(out)-1))>0)
					{
						received.append((const char*)out,rec);
					}
				    std::cout << expect << std::flush;
					return 0;
                }
                if(received.length()>32768)
                {
                    std::cout << "# OVERFLOWING!\r\n";
                    std::string loco = received.substr(16384);
                    received = loco;
                }
            }
            ::msleep(16);
        }
        if(allout){
            std::cout << received << std::flush;
        }
        return 0;
    }
    std::cout << "# Cannot open " << argv[1] << ". Is in use ? \r\n";
    return -1;
}

