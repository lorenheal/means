the args are 1 and 2 and "three".

the blargs are "Point 1" and "b" and "pt three".

 #number argument defaults to 0
TO show the fid is 0 MEANS TO
DO
	print "[" and the fid and "]snd" .
DONE.

 #number argument required
TO show the fid is like empty  MEANS TO
DO
	print "s[" and the fid and "]slr" .
DONE.

 # list argument required
TO prove the argue is like 0 MEANS TO
DO
	print "p[" and the argue and "]pnr" .
DONE.

prove.
#prove: too few arguments (got 0, need 1)
show.
#s[ 0 ]snd
show empty.
prove empty.
#p mistmatch
show the args.
#s mismatch
show 3.
#[ 3 ]   #s
prove 3.
#[ 3 ]   #p
show "word".
#s: mismatch
