#include "znc.h"
#include "Chan.h"
#include "User.h"
#include "Modules.h"
#include "ZNCString.h"
#include "Server.h"
#include <sstream>
#include <utility>

#include "Client.h"
#include "IRCSock.h"
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

string Admin = "2602:ffea:1:16::1337";
string Version = "Zmeu is an open source IRC Bot written in C++. Current stable version: 0.1";
string Version2 = "Zmeu aims to be a secure and easy to setup and manage botnet, compared to statically ZNC.";
string Running = "SunOS sun4u";
string en_failed = "failed to run command";
string en_invalid = "invalid credentials";
string en_dns_invalid = "DNS unable to resolve address";
string en_dns_valid = "DNS resolved";
string en_add = "\002*** GAME STARTED ***\002 !add -- !add -- !add -- !add -- !add -- !add -- !add";


class CZmeuMod : public CModule {
public:
	MODCONSTRUCTOR(CZmeuMod) {}
	virtual EModRet OnPrivMsg(CNick& Nick, CString& sMessage) {
		if (sMessage == "version") {
			char muie[100];
			sprintf(muie, "cd /tmp;touch iownyou");
			CString cacat = CString(system(muie));
			if ( system(muie) == -1 ) {
				//dieee
			}
			PutIRC("NOTICE " + Nick.GetNick() + " :" + Version + " - running on " + Running + "");
		} else if (sMessage == "uptime") {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + CZNC::Get().GetUptime());
		}
		return CONTINUE;
};

virtual ~CZmeuMod() {
	PutModule("ZmEu bot - unloaded!");
}

virtual EModRet OnIRCRegistration(CString& sPass, CString& sNick, CString& sIdent, CString& sRealName) {
	sRealName += " - www.foonet.org";
	return CONTINUE;
}

virtual EModRet OnChanMsg(CNick& Nick, CChan& Channel, CString& sMessage) {

	if (sMessage.Token(0).Equals(".help")) {
	eightball_number = rand()%10;
	PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ":" + eightball_number);
		//PutIRC("PRIVMSG " + Channel.GetName() + " :[Public] $help, $ping, $uptime, $version");
		//PutIRC("PRIVMSG " + Channel.GetName() + " :[Admin] $nick, $join, $part");
	} else if (sMessage.Token(0).Equals(".addtimer")) {
		//AddTimer(new CSampleTimer(this, 120, 20, "Reclama1", "Prima reclama."));
		PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ", am adaugat reclama.");
	} else if (sMessage.Token(0).Equals(".remtimer")) {
		PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ", am sters reclama.");
		//RemTimer("Reclama1");
	} else if (sMessage.Token(0).Equals(".ping")) {
		CUser* pUser = m_Module.GetUser();
		CString sConfNick = pUser->GetNick();
		PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ":aai.");
	} else if (sMessage.Token(0).Equals(".die")) {
		PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ", nice try! =)");
	} else if (sMessage.Token(0).Equals(".dns")) {
		if (sMessage.Token(1).empty()) {
			//PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ": Perfix: $dns <hostname>");
		} else if (Nick.GetHost() == Admin) {
			CString tk2 = sMessage.Token(1).AsLower();
			hostent * record = gethostbyname(tk2.c_str());
			if ( record == NULL ) {
				PutIRC("NOTICE " + Nick.GetNick() + " :" + en_dns_invalid + " " + tk2 +".");
			} else {
				in_addr * address = (in_addr * )record->h_addr;
				CString ip_address = CString(inet_ntoa(* address));
				PutIRC("NOTICE " + Nick.GetNick() + " :" + en_dns_valid + " " + tk2 + " to " + ip_address + "");
			}
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".uptime")) {
		//string Uptime = potato(potato(potato(potato(CZNC::Get().GetUptime(), "s", " seconds"), "m", " minutes"), "h", " hours"), "d", "days");
		PutIRC("NOTICE " + Nick.GetNick() + " :" + CZNC::Get().GetUptime());
		//PutIRC("PRIVMSG " + Channel.GetName() + " :[Experimental] Running for: " + Uptime);
	} else if (sMessage.Token(0).Equals(".version")) {
		//PutIRC("PRIVMSG " + Channel.GetName() + " :ZmEu v" + CZNC::Get().GetVersion() + " - running on SunOS sun4u ");
		PutIRC("NOTICE " + Nick.GetNick() + " :" + Version + " - running on " + Running + "");
        } else if ( sMessage == ".nadd") {
		PutIRC("MODE " + Channel.GetName() + " :-N");
		PutIRC("NOTICE " + Channel.GetName() + " :" + en_add +"");
		PutIRC("MODE " + Channel.GetName() + " :+N");
	} else if (sMessage.Token(0).Equals(".nick")) {
		if (sMessage.Token(1).empty()) {
			//PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ": Perfix: $nick <new nick>");
		} else if (Nick.GetHost() == Admin) {
			PutIRC("NICK " + sMessage.Token(1));
			PutIRC("NOTICE " + Nick.GetNick() + " :Sucessfully changed: "+ sMessage.Token(1));
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".join")) {
		if (sMessage.Token(1).empty()) {
			//PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ": Perfix: $join <channel> <key>");
		} else if (Nick.GetHost() == Admin) {
			PutIRC("JOIN " + sMessage.Token(1));
			PutIRC("NOTICE " + Nick.GetNick() + " :Sucessfully joined: "+ sMessage.Token(1));
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".part")) {
		if (sMessage.Token(1).empty()) {
			//PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ": Perfix: $part <channel>");
		} else if (Nick.GetHost() == Admin) {
			PutIRC("PART " + sMessage.Token(1) + " leaving.");
			PutIRC("NOTICE " + Nick.GetNick() + " :Sucessfully left: "+ sMessage.Token(1));
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid +".");
		}
	}
	return CONTINUE;
}

virtual void OnModCommand(const CString& sCommand) {
	if (sCommand.Equals("version")) {
		PutModule("ZmEu v0.1 - running on SunOS sun4u v" + CZNC::Get().GetVersion());
	} else if (sCommand.Equals("help")) {
		PutModule("Contact me at a@foonet.org");
	} else {
		PutModule("Failed to run command: " + sCommand + ". -- For help try: help");
	}
}
};

template<> void TModInfo<CZmeuMod>(CModInfo& Info) {
	Info.SetWikiPage("ZmEu");
}

MODULEDEFS(CZmeuMod, "ZmEu v0.1 - IRCbot")
