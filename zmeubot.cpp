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

string Admin = "hostname.foonet.org";
string Version = "Zmeu is an open source IRC Bot written in C++. Current stable version: 0.1";
string Version2 = "Zmeu aims to be a secure and easy to setup and manage botnet, compared to statically ZNC.";
string Running = "SunOS sun4u"; // false version
string en_failed = "failed to run command"; // invalid commands
string en_invalid = "invalid credentials"; // u dont have permissions
string en_dns_invalid = "DNS unable to resolve address"; // cant resolve ip/dns
string en_dns_valid = "DNS resolved"; // yay resolved

class CZmeuMod : public CModule {
public:
	MODCONSTRUCTOR(CZmeuMod) {}
	virtual EModRet OnPrivMsg(CNick& Nick, CString& sMessage) {
		// public msgs, anyone can exec, to prevend floods remove the line
		// line:start
		if (sMessage == "version") {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + Version + " - running on " + Running + "");
		} else if (sMessage == "uptime") {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + CZNC::Get().GetUptime());
		}
		// line:end
		return CONTINUE;
};

virtual ~CZmeuMod() {
	PutModule("ZmEu Bot for ZNC - UNLOADED !");
}

virtual EModRet OnIRCRegistration(CString& sPass, CString& sNick, CString& sIdent, CString& sRealName) {
	// addin pubs on ur realnames.
	sRealName += " - www.foonet.org";
	return CONTINUE;
}

virtual EModRet OnChanMsg(CNick& Nick, CChan& Channel, CString& sMessage) {

	if (sMessage.Token(0).Equals(".help")) {
		//eightball_number = rand()%10;
		//PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ":" + eightball_number);
		if (Nick.GetHost() == Admin) {
			PutIRC("NOTICE " + Nick.GetNick() + " :[PUBLIC CMDS] .dns, .uptime, .version");
			PutIRC("NOTICE " + Nick.GetNick() + " :[ADMIN CMDS] .help, .ping, .die, .nick, .join, .part, .+ads/.-ads");
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".+ads")) {
		if (Nick.GetHost() == Admin) {
			AddTimer(new CSampleTimer(this, 120, 20, "Reclama1", "my first adsense".")); // 120 secs
			PutIRC("NOTICE " + Nick.GetNick() + " :Ads now its activated.");
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".-ads")) {
		if (Nick.GetHost() == Admin) {
			RemTimer("Reclama1")
			PutIRC("NOTICE " + Nick.GetNick() + " :Ads now its disabled.");
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".ping")) {
		if (Nick.GetHost() == Admin) {
			CUser* pUser = m_Module.GetUser();
			CString sConfNick = pUser->GetNick();
			PutIRC("PING "+ Nick.GetNick() +"");
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".die")) {
		PutIRC("NOTICE " + Nick.GetNick() + " :nice try! =)");
	} else if (sMessage.Token(0).Equals(".dns")) {
		if (sMessage.Token(1).empty()) {
			PutIRC("PRIVMSG " + Channel.GetName() + " :Syntax: .dns <ip/hostname>");
		}
			CString tk2 = sMessage.Token(1).AsLower();
			hostent * record = gethostbyname(tk2.c_str());
			if ( record == NULL ) {
				PutIRC("PRIVMSG " + Channel.GetName() + " :" + en_dns_invalid + " " + tk2 +".");
			} else {
				in_addr * address = (in_addr * )record->h_addr;
				CString ip_address = CString(inet_ntoa(* address));
				PutIRC("PRIVMSG " + Channel.GetName() + " :" + en_dns_valid + " " + tk2 + " to " + ip_address + "");
			}
	} else if (sMessage.Token(0).Equals(".uptime")) {
		//string Uptime = potato(potato(potato(potato(CZNC::Get().GetUptime(), "s", " seconds"), "m", " minutes"), "h", " hours"), "d", "days");
		if (Nick.GetHost() == Admin) {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + CZNC::Get().GetUptime());
			//PutIRC("PRIVMSG " + Channel.GetName() + " :[Experimental] Running for: " + Uptime);
		} else {
			
		}
	} else if (sMessage.Token(0).Equals(".version")) {
		//PutIRC("PRIVMSG " + Channel.GetName() + " :ZmEu v" + CZNC::Get().GetVersion() + " - running on SunOS sun4u ");
		PutIRC("NOTICE " + Nick.GetNick() + " :" + Version + " - running on " + Running + "");
	} else if (sMessage.Token(0).Equals(".nick")) {
		if (sMessage.Token(1).empty()) {
			//PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() + ": Perfix: $nick <new nick>");
		} else if (Nick.GetHost() == Admin) {
			PutIRC("NICK " + sMessage.Token(1));
			PutIRC("NOTICE " + Nick.GetNick() + " :Nickname sucessfully changed to "+ sMessage.Token(1));
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".join")) {
		if (sMessage.Token(1).empty()) {
			PutIRC("NOTICE " + Nick.GetNick() + " :Syntax: .join <channel> <key>");
		} else if (Nick.GetHost() == Admin) {
			PutIRC("JOIN " + sMessage.Token(1));
			PutIRC("NOTICE " + Nick.GetNick() + " :Join successfully to "+ sMessage.Token(1));
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid + ".");
		}
	} else if (sMessage.Token(0).Equals(".part")) {
		if (sMessage.Token(1).empty()) {
			PutIRC("NOTICE " + Nick.GetNick() + " :Syntax: .part <channel>");
		} else if (Nick.GetHost() == Admin) {
			PutIRC("PART " + sMessage.Token(1) + " leaving.");
			PutIRC("NOTICE " + Nick.GetNick() + " :Part successfully to "+ sMessage.Token(1));
		} else {
			PutIRC("NOTICE " + Nick.GetNick() + " :" + en_invalid +".");
		}
	}
	return CONTINUE;
}

virtual void OnModCommand(const CString& sCommand) {
	if (sCommand.Equals("version")) {
		// this cmds available only on console
		// please dont remove or change and support the author
		// thank you
		PutModule("ZmEu Bot v0.1 - running on " + Running + " v" + CZNC::Get().GetVersion());
	} else if (sCommand.Equals("help")) {
		PutModule("Contact me at a@foonet.org");
		PutModule("---------------");
		PutModule("Your donations pay for projects, staff, servers and protective infrastructure.");
		PutModule("BTC: 1zmeu5BeWBprWyPv5ntNZKR7uThXaG9ic");
		PutModule("---------------");
	} else {
		PutModule("Failed to run command: " + sCommand + ". -- For help try: help");
	}
}
};

template<> void TModInfo<CZmeuMod>(CModInfo& Info) {
	Info.SetWikiPage("zmeubot");
}

MODULEDEFS(CZmeuMod, "ZmEu Bot v0.1 for ZNC")
