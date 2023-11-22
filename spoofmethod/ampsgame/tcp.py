from scapy.all import *

def process_packet(packet):
    if TCP in packet:
        tcp = packet[TCP]
        if tcp.flags.S:  # SYN packet
            print(f"SYN Sequence Number: {tcp.seq}")
        elif tcp.flags.A:  # ACK packet
            print(f"ACK Sequence Number: {tcp.ack}")
            print(f"Sequence Number: {tcp.seq}")

# Replace 'dest_host' and 'dest_port' with the target host and port
dest_host = '158.69.43.247'
dest_port = 22

# Send SYN packet to initiate the connection
syn_packet = IP(dst=dest_host) / TCP(dport=dest_port, flags='S')
syn_ack_packet = sr1(syn_packet)

# Capture the SYN/ACK response packet
if syn_ack_packet:
    process_packet(syn_ack_packet)

# Close the connection
if syn_ack_packet and TCP in syn_ack_packet:
    ack_packet = IP(dst=dest_host) / TCP(sport=syn_ack_packet[TCP].dport,
                                         dport=syn_ack_packet[TCP].sport,
                                         flags='A', seq=syn_ack_packet[TCP].ack, ack=syn_ack_packet[TCP].seq + 1)
    send(ack_packet)
