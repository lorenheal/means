the global is 100.
TO outer THE x is 0 MEANS TO DO
	the local is 10.
	TO inner the y is 1 MEANS TO DO
		print the local.
		print the global.
		print the x.
		print the y.
	DONE.
	TO dinner  MEANS TO return "dinner".
	print inner.
	the local is the local plus the x.
	return the local.
DONE.
print outer 33.
 # outer runs, sets local to 10, then runs inner printing 10, 100, 33, 1
 # outer adds the local and its single argument, returning 43.
#10
#100
#33
#1
#
#43
