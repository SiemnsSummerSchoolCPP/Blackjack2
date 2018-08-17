# Blackjack with Unix sockets.

This project was successfully run on both MacOS and Ubuntu 18.

## How to run:
  1. Run the BlackjackServer executable first. It's only argument is the port. Ex: './BlackjackServer 8000'.
  2. Once it's running, on different terminals run a some clients. First argument is the local IP of the server (127.0.0.1 if it's on the same machine or 192. ... if it's on a different computer). The second argument is the port.
  Ex: './BlackjackClient 127.0.0.1 8000'.

## How to play:
  As a client, type *help* for the available commands.

  Once everyone is connected, you can change your name or broadcast a message. To start the game, everyone has to type *ready*.

## Implementation
For this project I tried to use the MVC pattern in BlackjackServer & BlackjackClient. Also, to separate the logic from networking and Data, I used the n-tier architecture. I have also used Dependency Injection for easier manipulations.

The server runs on a single thread. It waits for any incoming actions, then it processes those actions. The client runs on 2 threads. One thread is responsible for incoming server actions and the second thread parses user input.

The socket messages are made from 3 parts: the length 4B, the msg header 4B and the message. The header is used to detect what kind of request it is. For example a change name request or bet request.
