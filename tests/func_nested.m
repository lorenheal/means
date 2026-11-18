the global is 100.
TO outer THE x AS AN INTEGER MEANS TO DO
	the local is 10.
	TO inner MEANS TO DO
		print the local.
		print the global.
		print the x.
	DONE.
	inner.
	the local is the local plus the x.
	return the local.
DONE.
print outer 99.
 # outer runs, sets local to 10, then runs inner printing 10, 100, 99, 
 # outer adds the local and its single argument, returning 109.
#10
#100
#99
#109
