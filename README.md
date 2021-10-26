# Shared-Memory-Project

This is a project to show off the producer-consumer problem. It is based on the example code provided here: https://man7.org/linux/man-pages/man3/shm_open.3.html

## How to run
- Start by compiling the code with the two commands:
  - gcc producer.c -pthread -lrt -o producer
  - gcc consumer.c -pthread -lrt -o consumer
  
 - Then you can run the programs with the following command:
  - ./producer & ./consumer &
  
 ## What it does
 The producer program stores two randomly generated ints into a shared memory and reports which ints were added and where. The consumer is then alerted that it can pull the data from the shared memory, it then consumes the items in the shared memory and reports what it consumed and from where. Finally, the producer reports that the values have indeed been consumed and loops, repeating another pair of ints 500 times.

Example output screenshot: https://imgur.com/a/3rboTX4
![alt text](https://i.imgur.com/AeNREkg.png)
