the args are 1 and 2 and "three".

the blargs are "Point 1" and "b" and "pt three".

 #number argument defaults to 0
TO show the fid is 0 MEANS TO
DO
	print "s[" and the fid and "]snd" .
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
#[rmf0:p] match failure with null function (prove, argc 0)
show.
#s[ 0 ]snd
show empty.
#s[ ]slr
prove empty.
#[rmf0:p] match failure with null function (prove, argc 1)
show the args.
#s[ 1 2 three ]slr
show 3.
#s[ 3 ]slr
prove 3.
#p[ 3 ]pnr
show "word".
#s[ word ]slr
