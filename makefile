all:
	g++ RPS_Client.cpp -o RPS_Client
	g++ RPS_Server.cpp -o RPS_Server

clean:
	rm -f RPS_Client
	rm -f RPS_Server