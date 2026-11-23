the args are 1 and 2 and "three".
the blargs are "Point 1" and "b" and "pt three".

 #list argument defaults to empty
 TO show the argues are empty  MEANS TO
DO
	print "[" and the argues and "]" .
DONE.

 # list argument required
TO prove the argues are like empty MEANS TO
DO
	print "[" and the argues and "]" .
DONE.

show.
prove.
#prove: Too few arguments (got 0, need 1)
show empty.
#[ ]
prove empty.
#[ ]
show 1 and 2.
#[ 1 2 ] 
prove 1 and 2.
#[ 1 2 ] 
show empty "1" and "2".
#[ ]
prove empty "1" and "2".
#[ ]
show empty and "1" and "2".
#[ 1 2 ]
prove empty and "2" and "1".
#[ 2 1 ]
show the args.
#[ 1 2 three ]
prove the blargs.
#[ Point 1 b pt three ]
show empty the args.
#[ ]
prove empty the blargs.
#[ ]
