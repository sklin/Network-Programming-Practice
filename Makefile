template:
	gcc template_server.c -o server
	gcc template_client.c -o client
echo:
	gcc echo_server.c -o server
	gcc echo_client.c -o client
daytime:
	gcc daytime_server.c -o server
	gcc daytime_client.c -o client
fork:
	gcc fork_echo_server.c -o server
	gcc fork_echo_client.c -o client
simple:
	gcc simple_server.c -o server
	gcc simple_client.c -o client
clean:
	rm -f server
	rm -f client
