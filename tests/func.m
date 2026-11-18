 #
 # zero-arg function
 #
TO fortytwo MEANS TO DO
	print 42 and newline.
DONE.
fortytwo.
#42

 # zero-arg, returns a number
TO fortythree MEANS TO DO
	return 43 and newline.
DONE.
print fortythree.
#43

 #
 # function with one arg and  no return (void-like)
 #
TO say the message is "" MEANS to print the message and " ". 
 #
TO bray the message is "" MEANS to do
	print the message and newline.
done.
say "hi".
#hi
 # side effect of say, not result.
print say "lo".
print bray "blo".
#lo
#blo

 #
 # function taking an arg, doing something to it locally, returning a value
 #
TO double THE n is 0 MEANS to return the n times 2. 
 #
 # once more, but with a block
 #
TO redouble THE n is 0.0 MEANS to do return the n times 2. done.

the x is 7.
print the x and newline.
#7
the y is double the x.
print the y and newline.
#14
print double the y and newline.
#28
print ".. " and redouble the y and newline.
#.. 28
