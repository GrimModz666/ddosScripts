import psutil
import time
import sys
import threading
import curses
import socket 
import subprocess

def get_network_usage(interface='eno1'):
    net_io_prev = psutil.net_io_counters(pernic=True).get(interface)
    time.sleep(1)
    net_io_curr = psutil.net_io_counters(pernic=True).get(interface)

    if net_io_prev and net_io_curr:
        rx_bytes = net_io_curr.bytes_recv - net_io_prev.bytes_recv
        tx_bytes = net_io_curr.bytes_sent - net_io_prev.bytes_sent
        rx_mbps = (rx_bytes * 8) / (1024 * 1024)
        tx_mbps = (tx_bytes * 8) / (1024 * 1024)
        rx_pps = net_io_curr.packets_recv - net_io_prev.packets_recv
        tx_pps = net_io_curr.packets_sent - net_io_prev.packets_sent
        return rx_mbps, tx_mbps, rx_pps, tx_pps
    return 0, 0, 0, 0

def get_cpu_usage():
    return psutil.cpu_percent()

def get_memory_usage():
    mem = psutil.virtual_memory()
    mem_used = mem.used / (1024 * 1024)
    mem_total = mem.total / (1024 * 1024)
    return mem_used, mem_total

def get_src_ips(interface='eno1', n=5):
    src_ips = {}
    connections = psutil.net_connections(kind='inet')
    for conn in connections:
        src_ip = conn.raddr[0] if conn.status == 'ESTABLISHED' else conn.laddr[0]
        src_ips[src_ip] = src_ips.get(src_ip, 0) + 1
    return dict(sorted(src_ips.items(), key=lambda x: x[1], reverse=True)[:n])

def print_protocol_stats(win, protocol_stats):
    win.addstr("Protocol Statistics:\n", curses.color_pair(6))
    for (protocol,), count in sorted(protocol_stats.items(), key=lambda x: x[1], reverse=True):
        win.addstr("    Protocol: {}, Count: {}\n".format(protocol, count))


def print_separator(win):
    win.addstr("-" * 80 + "\n", curses.color_pair(1))

def print_network_usage(win, interface, rx_mbps, tx_mbps, rx_pps, tx_pps):
    win.addstr("Network Usage (Interface: {}):\n".format(interface), curses.color_pair(2))
    win.addstr("Incoming Speed: {:.2f} Mbps\n".format(rx_mbps))
    win.addstr("Outgoing Speed: {:.2f} Mbps\n".format(tx_mbps))
    win.addstr("Incoming Packets: {}\n".format(rx_pps))
    win.addstr("Outgoing Packets: {}\n".format(tx_pps))

def print_cpu_usage(win, cpu_usage):
    win.addstr("CPU Usage: {:.2f}%\n".format(cpu_usage), curses.color_pair(3))

def print_memory_usage(win, mem_used, mem_total):
    win.addstr("Memory Usage: {:.2f} MB / {:.2f} MB\n".format(mem_used, mem_total), curses.color_pair(4))

def print_top_src_ips(win, top_src_ips):
    win.addstr("Top {} Source IPs Sending Data:\n".format(len(top_src_ips)), curses.color_pair(5))
    for ip, count in top_src_ips.items():
        win.addstr("    {}: {} connections\n".format(ip, count))

def start_packet_capture():
    print("Packet capturing started.")
    tshark_cmd = ["tshark", "-i", "eno1", "-w", "capture.pcap"]
    return subprocess.Popen(tshark_cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

def stop_packet_capture(pcap_process):
    print("Packet capturing stopped.")
    # To stop tshark, we send SIGINT (Ctrl+C) to the subprocess
    pcap_process.send_signal(subprocess.signal.SIGINT)

def packet_capture_handler():
    # Capture threshold in bytes (400 MB)
    capture_threshold = 400 * 1024 * 1024
    capture_started = False
    pcap_process = None

    while True:
        incoming_bytes = psutil.net_io_counters(pernic=True)['eno1'].bytes_recv
        if incoming_bytes > capture_threshold and not capture_started:
            pcap_process = start_packet_capture()
            capture_started = True
        elif incoming_bytes <= capture_threshold and capture_started:
            stop_packet_capture(pcap_process)
            capture_started = False
        elif capture_started:
            # Print live tshark output while capturing
            line = pcap_process.stdout.readline().decode('utf-8')
            if line:
                print(line.strip())
        time.sleep(1)

def update_data(win, interface, top_src_ips_count, stop_event):
    while not stop_event.is_set():
        rx_mbps, tx_mbps, rx_pps, tx_pps = get_network_usage(interface)
        cpu_usage = get_cpu_usage()
        mem_used, mem_total = get_memory_usage()
        top_src_ips = get_src_ips(interface, top_src_ips_count)

        win.clear()  # Clear the screen before updating the content
        print_separator(win)
        print_network_usage(win, interface, rx_mbps, tx_mbps, rx_pps, tx_pps)
        print_separator(win)
        print_cpu_usage(win, cpu_usage)
        print_memory_usage(win, mem_used, mem_total)
        print_separator(win)
        print_top_src_ips(win, top_src_ips)
        print_separator(win)

        win.refresh()
        time.sleep(1)  # Update the data every second



def main(stdscr):
    interface = 'eno1'  # Change this to your network interface if needed
    top_src_ips_count = 5  # Change this to get more or fewer top source IPs

    # Set up color pairs for different text colors
    curses.init_pair(1, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(4, curses.COLOR_MAGENTA, curses.COLOR_BLACK)
    curses.init_pair(5, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(6, curses.COLOR_CYAN, curses.COLOR_BLACK)  # Protocol statistics color

    sick_banner = """
                 _   _           _   _ ____  _
 _ __  _ __ ___ | |_| |__   __ _| | | | __ )| | ___   __ _
| '_ \| '__/ _ \| __| '_ \ / _` | | | |  _ \| |/ _ \ / _` |
| |_) | | | (_) | |_| | | | (_| | |_| | |_) | | (_) | (_| |
| .__/|_|  \___/ \__|_| |_|\__,_|\___/|____/|_|\___/ \__, |
|_|                                                 |___/
"""
    stdscr.addstr(sick_banner, curses.color_pair(1))

    stop_event = threading.Event()

    update_thread = threading.Thread(target=update_data, args=(stdscr, interface, top_src_ips_count, stop_event))
    update_thread.start()
    packet_capture_thread = threading.Thread(target=packet_capture_handler)
    packet_capture_thread.daemon = True
    packet_capture_thread.start()

    while True:
        c = stdscr.getch()
        if c == ord('q'):
            stop_event.set()
            update_thread.join()
            break

if __name__ == "__main__":
    curses.wrapper(main)