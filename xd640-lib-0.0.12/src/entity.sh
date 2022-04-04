#!/bin/sh

files="HTMLlat1.ent HTMLspecial.ent HTMLsymbol.ent"


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
	lines=`grep CDATA ${f} | grep \&#`
	
	for l in ${lines} 
	do
		name=`echo ${l} | cut -d' ' -f2`
		value=`echo ${l} | cut -d'#' -f2 | cut -d';' -f1`
		echo -ne "{\"${name}\", \t${value}},\n" >> tmp.txt
		let "n=${n} + 1"
	done
	IFS=${ifs};	
done

echo -ne "#define NB_ENTITIES ${n} \n"
echo entity_struct entities[${n}] = { 
cat tmp.txt | sort
rm -f tmp.txt
echo }\;
echo /\* nb entities : ${n} \*/
