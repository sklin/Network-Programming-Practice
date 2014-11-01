template:
	gcc template_server.c -o server
	gcc template_client.c -o client
echo:
	gcc echo_server.c -o server
	gcc echo_client.c -o client
daytime:
	gcc daytime_server.c -o server
	gcc daytime_client.c -o client
