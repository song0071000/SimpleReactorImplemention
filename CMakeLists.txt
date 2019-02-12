#author:walterzhao 
#e-mail:519362600@qq.com

cmake_minimum_required (VERSION 2.8)

project(reactor)

add_executable(./bin/reactor_server reactor_server_test.cc event_demultiplexer.cc reactor.cc global.cc)
add_executable(./bin/reactor_client reactor_client_test.cc event_demultiplexer.cc reactor.cc global.cc)
