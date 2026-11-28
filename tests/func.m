 #
 # zero-arg function
 #
TO fortytwo MEANS TO DO
	print 42.
DONE.
fortytwo.
#42

 # zero-arg, returns a number
TO fortythree MEANS TO DO
	return 43.
DONE.
print fortythree.
#43

 #
 # function with one arg and  no return (void-like)
 #
TO say the message is "" MEANS 
	to print "s[" and the message and "]". 
 #
TO bray the message is "" MEANS to do
	return "b[" and the message and "]".
done.
say "hi".
#s[ hi ]
 # side effect of say, not result.
say "lo".
#s[ lo ]
print bray "blo".
#b[ blo ]

 #
 # function taking an arg, doing something to it locally, returning a value
 #
TO double THE n is 0 MEANS to return the n times 2. 
 #
 # once more, but with a block
 #
TO redouble THE n is 0.0 MEANS to do return the n times 2. done.
TO redouble THE n is 0 MEANS to do return the n times 2. done.

the x is 7.
print the x.
#7
the y is double the x.
print the y.
#14
print double the y.
#28
print ".." and redouble the y.
#.. 28
