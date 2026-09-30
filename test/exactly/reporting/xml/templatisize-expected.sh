echo "version"
for f in *.txt
do
   echo $f
   ex -s -c 'g/Criterion/s/Criterion v2.3.3-123-g01577d6 (bleeding)/Criterion <VERSION>/' -c x $f
done
echo "time"
for f in *.txt; do echo $f; ex -s -c 'g/time/s/time="[^"]*"/time="<TIME>"/' -c x $f; done
echo "re"
for f in *.txt; do echo $f; ex -s -c 'g/re/s/ (re)$//' -c x $f; done
