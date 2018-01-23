#!/bin/sh

pofile=$1
if [ "$pofile" == "" ];then
 echo "usage: "
 echo ">cd <ExpertusONE directory>"
 echo ">util <po-file-name>"
 exit 0
fi
patternfile=`pwd`/tmp_pattern0000001.sed
rm -f $patternfile
grep -v "^[ ]*#" $pofile | awk '!(NR%2){print "s/t(xxxx"$0")/t(yyyy"p")/g"}{p=$0}' | sed -e 's/xxxx[^ ]*[ ]//g' -e 's/yyyy[^ ]*[ ]//g' > $patternfile

echo "Using po file : $pofile"
echo "started replacing msg code"
find . -name "*.inc" -o -name "*.php" -o -name "*.module"  | xargs sed -i -f $patternfile
rm -f $patternfile
echo "completed replacing msg code"
