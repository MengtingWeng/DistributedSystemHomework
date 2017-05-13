ADC 2017
Project 1
The project on github: https://github.com/irisxia51244/ADC2017

======
Group Member
------
Mengting Weng (1750194)
Weijia Chen(1667998)
Zijun Qu(1650088)
Shihui Xia(1668066) 

General Purpose
------
Implementing a single-thread client-server application,a file server, 
connecting through a TCP. Client is able to send a single or multiple
message to the server. The proposed file server supports the following
operations: read, mkdir, rmdir, opendir, upload and download.

Usage
------
- To compile:
```
g++ -o server server.cpp
```
```
g++ -o client client.cpp
```
- First, to run the server:
```
./server
```
- Then, to run the client:
```
./client
```
- To make operations:
Follow the instruction, enter the corresponding number of the operation,
then, enter the desired file name.

Details
-------
This file server can support 6 operations:
1. read 
2. mkdir
3. rmdir
4. opendir
5. upload
6. download

Detail:
1. read: client should enter the function number "1" and then enter the file name to read the file. The content of this file will be printed on the screen. 
```
Please enter the function number:
1
[2017/03/22 16:38:05]
Enter the target file name
1.txt
[2017/03/22 16:38:25]
>>Successful read file
This is for the test of read. 
The homework 1: file server
```
2. mkdir: client should enter the function number "2" and then enter the folder name to create a new folder. We can check the result by function 4(opendir).
```
Please enter the function number:
2
[2017/03/22 16:58:41]
Enter the target file name
test_mkdir
[2017/03/22 16:58:55]
>>Successfully MKDIR 
[2017/03/22 16:59:09]
Client:Please choose the function...
1. read
2. mkdir
3. rmdir
4. opendir
5. upload
6. download
Please enter the function number:
4
[2017/03/22 16:59:17]
Enter the target file name
.
[2017/03/22 16:59:20]
.    ..    .git    .README.md.swp    client    README.md    server.cpp    server.out    test_mkdir
```
3. rmdir: client should enter the function number "3" and then enter the folder name to remove this folder. We can check the result by function 4(opendir).
```
Please enter the function number:
3
[2017/03/22 17:03:19]
Enter the target file name
test_mkdir
[2017/03/22 17:03:23]
>>Successfully RMDIR 
 
[2017/03/22 17:03:23]
Client:Please choose the function...
1. read
2. mkdir
3. rmdir
4. opendir
5. upload
6. download
Please enter the function number:
4
[2017/03/22 17:03:27]
Enter the target file name
.
[2017/03/22 17:03:28]
.    ..    .git    .README.md.swp    client    README.md    server.cpp    server.out  
```
4. opendir:client should enter the function number "4" and then enter the folder name to check the file name list under this folder.
```
Please enter the function number:
4
[2017/03/22 17:07:40]
Enter the target file name
test_opendir
[2017/03/22 17:07:47]
.    ..    file1    file2    file3
```
5. upload:client should enter the function number "5" and then enter the file name to upload the file to server. This file should exist in client locally.
```
Please enter the function number:
5
[2017/03/22 17:12:24]
Enter the target file name
3.txt
[2017/03/22 17:12:26]
>>Succcessfully upload file
```
6. download:client should enter the function number "6" and then enter the file name to download the file from server. This file should exist in server.
```
Please enter the function number:
5
[2017/03/22 17:12:24]
Enter the target file name
3.txt
[2017/03/22 17:12:26]
>>Succcessfully upload file

```
Enter "#" to end the connection.
