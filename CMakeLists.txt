cmake_minimum_required(VERSION 3.15)
project(Proactor)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proactor main.cpp AsynchronousOperation.h ASynchronousOperationProcessor.h Observer.h CompletionEventQueue.h Proactor.h Initiator.h)