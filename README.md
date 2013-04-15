cmpsc311project7
================

Cmpsc311 Project7

README for pr7

Author:	Jake Jones
Email:	jaj5333@psu.edu

Author:	Scott Cheloha
Email:	ssc5145@psu.edu


Time Allotment:
	~10 hours reading textbooks
	~15 hours creating base shell
	~15 hours implementing exec features, etc
	~5 hours extra credit work added

NOTE: We compiled and tested for Mac OS X 10.8.2|3

Builtin functions work as expected.
Command file execution works as expected.
Shell never fails.  There are small bugs when mixing foreground and background Sleep(1).
Background command execution works as expected.
Background job control works as expected.
Signal handling under non OS X systems is flawed.
Execlp is not perfect.
kill background function implemented.
limits function implemented.
toggle implemented for e,v,d.
set implemented for echo,verbose,debug,exec.

If we were to do this project again, we would explicitly restart slow system calls after SIGINT interuption.  OS X, being BSD based, does this automatically, but systems like Solaris and various Linux systems do not.