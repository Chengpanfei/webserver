cmake_minimum_required(VERSION 3.10)
project(webserver)

set(CMAKE_CXX_STANDARD 11)

add_executable(webserver main.cpp WebServer.cpp WebServer.h ServerSocket.cpp ServerSocket.h SocketException.cpp SocketException.h EventLoop.cpp EventLoop.h Socket.cpp Socket.h ByteBuffer.cpp ByteBuffer.h Handler.h http/HttpDecoder.cpp http/HttpDecoder.h Message.h http/HttpResponse.h http/HttpResponse.cpp http/HttpRequest.h http/HttpRequest.cpp http/HttpProcessor.cpp http/HttpProcessor.h http/HttpEncoder.cpp http/HttpEncoder.h)