if (-f cs570.h) then
mv cs570.h cs570.h.submitted
endif
if (-f my570list.h) then
mv my570list.h my570list.h.submitted
endif
if (-f my570list.hpp) then
mv my570list.hpp my570list.hpp.submitted
endif
cp ~csci570b/public/cs570/hw2/cs570.h .
cp ~csci570b/public/cs570/hw2/my570list.h .
cp ~csci570b/public/cs570/hw2/my570list.hpp .

set srcdir=~csci570b/public/cs570/hw2

#
# (B1)
# for the following commands, each correct answer gets 2 point
# "diff" command should produce nothing
#
/bin/rm -f f?.sort
foreach f (0 1 2 3 4 5 6 7 8 9)
    echo "===> $srcdir/f$f"
    ./hw2 sort $srcdir/f$f > f$f.sort
    diff $srcdir/f$f.sort f$f.sort
end
/bin/rm -f f?.sort

#
# (B2)
# for the following commands, each correct answer gets 2 point
# "diff" command should produce nothing
#
/bin/rm -f f??.sort
foreach f (10 11 12 13 14 15 16 17 18 19)
    echo "===> $srcdir/f$f"
    cat $srcdir/f$f | ./hw2 sort > f$f.sort
    diff $srcdir/f$f.sort f$f.sort
end
/bin/rm -f f??.sort