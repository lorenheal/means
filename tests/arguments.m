the args are 1 and 2 and "three".
the blargs are "Point 1" and "b" and "pt three".

 #list argument defaults to empty
 TO show the argues are empty  MEANS TO
DO
	print "s[" and the argues and "]" .
DONE.

 # list argument required
TO prove the argues are like empty MEANS TO
DO
	print "p[" and the argues and "]" .
DONE.

prove.
#prove: too few arguments (got 0, need 1)
show.
#s[ ]
show empty.
#s[ ]
prove empty.
#p[ ]
show 1 and 2.
#s[ 1 2 ]
prove 1 and 2.
#p[ 1 2 ]
show empty and "1" and "2".
#s[ 1 2 ]
prove "2" and empty and "1".
#p[ 2 1 ]
show the args.
#s[ 1 2 three ]
prove the blargs.
#p[ Point 1 b pt three ]
show empty and the args.
#s[ 1 2 three ]
prove empty and the blargs.
#p[ Point 1 b pt three ]
