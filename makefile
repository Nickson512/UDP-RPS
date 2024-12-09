all:
	g++ RPS_Client.cpp -o RPS_Client
	g++ RPS_Server.cpp -o RPS_Server
	g++ Simple_Server.cpp -o Simple_Server

clean:
	rm -f RPS_Client
	rm -f RPS_Server
	rm -f Simple_Server