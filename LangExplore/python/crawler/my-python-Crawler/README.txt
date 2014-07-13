My Python Crawler.
A simple crawler where I took takes care of ValueError, HTTPError, URLError, InvalidURL, HTMLParseError with try-catch. 

Only distinct URLs and those URLs that will open in browser without errors are stored.
A URL is assumed  to be giving error and (thus Bad URL) if it gives a response code higher than equal to 400.
I also maintained a temporary cache of 5000 good and bad urls to speed up the process. 


The System stores output in the following files:
URLs in		->  url_names.txt in a folder called URLs.
Images in		-> folder called images
Text info in	-> (.txt) files in a folder called text
Videos in		-> folder called Video # Currenty Video part is not functioning properly

If these folders are not there they are created.


To run the app pasete all the files in a folder where Python, BeautifulSoup, Numpy,YAML and NTLK are installed.

(The version of Tools in which I made these are
Python 		-> 2.5
BeautifulSoup 	-> 3.1.0.1
PyYAML		-> 3.10.win32-py2.5
numpy		-> 1.6.1-win32-superpack
Ntlk		-> 2.0.1.win32
)

After pasting the folder run "main.py"
and enter the URL as asked
then enter the No. of links as asked, if not given the value is defaulted to 20.

The system will crawl and print the urls as it finds them, not more than 10 from a perticular url
and afert finding them will print them again on the screen and in a file called url_name.txt in Directory URLs
the images goes in a directory called "images" and text in (.txt) files that get stored in a directory called "text".

Sample inputs:
Starting URL  -> http://youtube.com
No of URLs needed  -> 20

Output for Sample Urls are available at @

