Reactor_Implemention
====================
This is a simple implementation of reactor which use epoll as the event multiplexer and the min-heap as the manage container of timed-task. If you want to use it in industrial environment, i think the "ACE" is best solution.

You can compile the code by using the following shell command:

>g++ -o reactor_server reactor_server_test.cc event_demultiplexer.cc reactor.cc global.cc
>g++ -o reactor_client reactor_client_test.cc event_demultiplexer.cc reactor.cc global.cc

After that, two executable programs will be made.

You can use them in this way:

>./reactor_server 127.0.0.1 6852
>./reactor_client 127.0.0.1 6852
