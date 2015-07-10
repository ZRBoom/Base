# Base
#Basic classes and sources of projects
I Android:
  1 CommandPromt: sources of apk for android to launch shell scripts.
  2 intercom_first: this program send and receive voice data by UDP multicasting.
  3 intercom_first_with_error_detect: like 2, but don`t send data, when absolute value of voice frame is less parametеr Noise_Level
and the second diffence is intellectual buffer of received packets. Both project multithreaded reproduce the voice of all users.
II Rid_sol: It includes all possible functions to implement the code Reed-Solomon. Also, it includes some docs to understand
algoritm of this error controlled code.
III WinPcap:
  1 ArpSniffer: realization of arp-sniffer based on WinPcap and its examples.
  2 SourcesBuffer: buffer that cat be used in UDP proxy
  3 SourcesSend: UDP client with IP and MAC spoofing
  4 SourcesServer: implementaion of UDP server
