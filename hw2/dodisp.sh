#!/bin/tcsh
set srcdir=~csci570b/public/cs570/hw2

/bin/rm -f f?.sort
foreach f (0 1 2 3 4 5 6 7 8 9)
	echo "===> $srcdir/f$f"
	./hw2 sort $srcdir/f$f > f$f.sort
	diff $srcdir/f$f.sort f$f.sort
end
/bin/rm -f f?.sort

/bin/rm -f f??.sort
foreach f (10 11 12 13 14 15 16 17 18 19)
	echo "===> $srcdir/f$f"
	./hw2 sort $srcdir/f$f > f$f.sort
	diff $srcdir/f$f.sort f$f.sort
end
/bin/rm -f f??.sort

foreach f (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19)
	echo "===> $srcdir/d$f"
	./hw2 display $srcdir/d$f > d$f.dpy
	diff $srcdir/d$f.dpy d$f.dpy
end
/bin/rm -f d?.dpy d??.dpy

cat $srcdir/f101 | ./hw2 sort
cat $srcdir/f102 | ./hw2 sort
cat $srcdir/f103 | ./hw2 sort
cat $srcdir/f104 | ./hw2 sort
cat $srcdir/f105 | ./hw2 sort
cat $srcdir/f106 | ./hw2 sort
cat $srcdir/f107 | ./hw2 sort

