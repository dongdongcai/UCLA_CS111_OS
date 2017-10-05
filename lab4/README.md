# INFORMATION
My name : Dongdong Cai
My ID# : 304587528

# INCLUDED FILES
* tls_client.c: the source for the C program required by the spec 
* 6 screenshots for UDP_client
	1. First 10 lines of output of ./udp_client
	2. Output of ./get_tshark 8000, i.e. data from packet sniffer
	3. Output of ./udp_client after ./set_rate 1
	4. Output of ./get_tshark 8000 after ./set_rate 1
	5. Output of ./udp_client after ./start_attack
	6. Output of ./get_tshark 8000 after ./start_attack
* 6 screenshots for the single thread TLS_client responses
	1. First 10 lines of output of ./tls_client
	2. Output of ./get_tshark 5000, i.e. data from packet sniffer
	3. Output of ./tls_client after ./set_rate 1
	4. Output of ./get_tshark 5000 after ./set_rate 1
	5. Output of ./tls_client after ./start_attack
	6. Output of ./get_tshark 5000 after ./start_attack
* 1 screenshot for the dual thread TLS_client response
* Lab4-SecC-Q6.txt: The log file correspongding to the previous screenshot.
* this README file

# DESCRIPTION OF SCREENSHOTS AND LOG FILE
Lab4-SecA-Q2:
The output include the IP address of my Edison, the IP address and port of the server application, the sending rate of the message and the message the Edison sent and received. As it can be seen from the screenshot, Edison sent the heartbeat rate to the server and the server just echo the message.

Lab4-SecA-Q4:
As we can see from the screenshot, the packet sniffer application can record the conversation between the client and server and even the specific time of each message, which shows the current application is not secure enough since everyone else can access to the private data of patient.

Lab4-SecA-Q5b and Lab4-SecA-Q5d:
From the output, we can see that the ./set_rate can change the message sending rate of client by sending a message to the server first can the server would ask the client to change its sending rate. We can examine the result of ./set_rate using wireshark. The time interval of each message sending was initially 3 and then became 1 after I called ./set_rate 1.

Lab4-SecA-Q6b and Lab4-SecA-Q6d:
From the output and the output of wireshark, the ./start_attack can also set the rate of client to 15, which can be verified by the result of wireshark. It can also show the client is in secure since every one else can set the rate of the client.

Lab4-SecB-Q3:
The output is similar as Lab4-SecA-Q2. But there is two more line of output shows that the communication between client and server is protected by AES256-SHA and a certification.

Lab4-SecB-Q5:
In this screenshot, we can see that I can no longer see the conversation between client and server. What I get is some garbage message(encrypted message). What's more, we can find out that now the communication between client and server is based on TCP since the client would send the server an ACK once it reveices a response from server. However, since the communication is based on TCP, the latency of messages would be larger than previous section.

Lab4-SecB-Q6b and Lab4-SecB-Q6d:
It can be seen from the screenshot, I can still set the rate of client by calling ./set_rate, which can be verified by wireshark
However, the client would show inconsistency between message sent and message received. The reason is that the server would send response twice after ./set_rate and the result of wireshark shows something as "TCP retransmission". I believe this may be the reason for the inconsistency.

Lab4-SecB-Q7b and Lab4-SecB-Q7d:
As expected, I can no longer set the rate of client using ./start_attack since authentication is needed when try to send message to set the rate of client.

Lab4-SecC-Q3:
The screenshot shows that the inconsistency no longer exist for tls_client after ./set_rate.

What I did to solve the problem:
I added one more thread for the client which keep reading message from SSL connection and I also used a global valuable to show if the client have received the response for the message it send. In the extra thread, we the client get a message from the connection, it would examine if the global valuable is true, which means the client have not received the response for the message it send, and then the client would output the message and recored it into the log file. If the global valuable is false, this means the received message is a redundant message and the client would just ignore it. And the client would not send new message if the global valuable is true.

Lab4-SecC-Q6.txt:
The log file just record the messages between client and server. The log file is almost the same as the Lab4-SecC-Q3 except that it does not include some status messages, such as current rate and range of client.
