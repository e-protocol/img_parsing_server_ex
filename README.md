# img_parsing_server_ex

## Description
Example of server processing images with openCV. Client upload on server img.jpg via POST request. 
Server detects all faces on image, drawing rectangle boundries and bluring faces inside that boundries.

## Instructions
- start server app
- type start - server starts, also has additional commands, type help to see them
- default server port 8080
- upload image with curl like so:
curl -vX POST -H "filename: test2.jpg" --data-binary "@$HOME/Pictures/test2.jpg"  0.0.0.0:8080 -o test3.jpg
NOTE! The http request should be POST and contains "filename:" field in header
- response from server is processed image
- this response contains coordinates of faces boundries and Content-Length in header fields, example of boundries filed:
"bbx: {{508,358},{788,638}}" - where "bbx" is a field of header and {{ Top left Point (x,y) }, { Bottom right Point (x,y) }}

## TODO
1) Server built with Boost 1.83, should be changed to custom epoll controlling semantics for higher operation performance
specified by target machine
2) Server saves image as temp file in /tmp directory. Need to get rid of it and pass raw bytes payload directly to openCV operation modules
for better performance. Because using filesystem to make 2 file write operations and 2 read operations is long enough.
To achieve this required some kind of algorithm to retrieve image meta-data to locate where raw data bytes starts and ends

P.S. Tested with .jpg image, resolution 1280x720
Debian x64, Boost 1.83, C++17, OpenCV 4.10