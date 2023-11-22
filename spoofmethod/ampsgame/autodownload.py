import os
import sys
import time

def download():
	print("DOWNLOADING LIST")
	os.system("rm -rf *.list; rm -rf *.txt")
	os.system("cd ampsgame; rm -rf *.list; rm -rf *.txt")
	os.system("wget 64.112.72.102:8000/dns.list -O african4000.txt")
	os.system("wget 64.112.72.102:8000/dvr.list")
	os.system("wget 64.112.72.102:8000/stun.list")
	os.system("wget 64.112.72.102:8000/cldap.list")
	os.system("wget 64.112.72.102:8000/ipsec.list")
	os.system("wget 64.112.72.102:8000/mdns.list")
	os.system("wget 64.112.72.102:8000/wsd.list")
	os.system("wget 64.112.72.102:8000/snmp.list")
	os.system("wget 64.112.72.102:8000/ntp.list")
	os.system("wget 64.112.72.102:8000/sip.list")
	os.system("wget 64.112.72.102:8000/afs.list")
	os.system("wget 64.112.72.102:8000/source.list")
	os.system("wget 64.112.72.102:8000/mdns.list")
	os.system("wget 64.112.72.102:8000/nat-pmp-live.list.list")
	os.system("wget 64.112.72.102:8000/plex.list")
	os.system("wget 64.112.72.102:8000/ard.list")
	os.system("wget 64.112.72.102:8000/steam.list")
	os.system("mv *.list /root/ampsgame/; mv african4000.txt /root/ampsgame/")
	print('done 259200 seconds till update')
	time.sleep(259200)
	download()

download()