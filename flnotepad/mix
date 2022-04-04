i=0
x=0
echo "<html><body><div>" > test.html
while test $i -lt 65000
do
	
	echo "&#${i};" >> test.html
	let i=${i}+1
	let x=${x}+1
	if test $x -gt 60; then
		echo "</div><div>" >> test.html
		x=0
	fi
done

echo "</div></body><html>" >> test.html 
