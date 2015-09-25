# Project email, IM, GUI wiki editor #

Hey Trevor. Question: does google code have an internal project email and IM system? It would be good to be able to send/receive emails, IMs and have the system archive them for future searching and persual as is possible in svn repos like Assembla. Also, is there a GUI wiki editor (would be nice but not neccessary)?. Also, I don't see an option to here that enables one to attach and link documents/files to a wiki page.

Trevor: I actually don't know...  maybe we can just edit wiki files like i'm doing right now...  As for a GUI wiki editor, i'm not sure, but I doubt it.  I suspect we can attach docs/files, but I'd have to read about the wiki.

# C++ mess #
I've got the Java project going. The C++ project blew up in my face. I am using the cygwin c++,gdb,make etc... What is g++-4 ? I tried replacing it with just g++ (in the make file) but then it couldn't 9 out of 10 of the header files referenced.

Trevor: did you install g++ v.4 through cygwin?  do you have cygwin/bin in your PATH variable?

# Your locks? #
  * **Lock1**: I assume is a straight-ahead CAS lock
  * **Lock2**: Bakery algorithm?
  * **Lock3**: You mention Anderson, but not 100% sure which of his algorithms it is

Trevor: these are locks 1, 2, 3 in the crummy-mellor paper! =]