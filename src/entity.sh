#!/bin/sh

files="byalpha.html"


ifs=${IFS}

echo
echo struct entity_struct {
echo 	const char *name\;
echo	unsigned short value\;
echo }\;
echo

n=0;
rm -f tmp.txt
for f in ${files}
do
	IFS="
"
	lines=`grep U0 ${f}`
	
	for l in ${lines} 
	do
		name=`echo ${l} | cut -d',' -f1`
		value=`echo ${l} | cut -d',' -f2 | cut -d'U' -f2`
		minus=`echo ${value} | grep '\-'`
		if test x${minus} = x; then 
			echo -ne "{\"${name}\", \t0x${value}},\n" >> tmp.txt
			let "n=${n} + 1"
		fi
	done
	IFS=${ifs};	
done

echo -ne "#define NB_ENTITIES ${n} \n"
echo entity_struct entities[${n}] = { 
cat tmp.txt | uniq | sort
rm -f tmp.txt
echo }\;
echo /\* nb entities : ${n} \*/
