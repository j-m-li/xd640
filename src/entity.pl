#!/usr/bin/perl

$in_pre = 0;
$l = 0;
@a = {0};
open(FILE2, "<entities.html");
while(<FILE2>) {
	if ($in_pre) {
		if ($_ =~m/<\/pre>/ig) {
			$in_pre = 0;
		} elsif ($in_pre > 2) {
			@n = split(' ', substr($_, 0, 32));
			if ($n[0] ne "") {
				$b = substr($_, 50, 4);
				$a[$l] = "{\"$n[0]\", \t0x$b},\n";
				$l++;
			}
		} else {
			$in_pre++;
		}
	} elsif ($_ =~m/<pre>/ig) {
		$in_pre = 1;
	}
}
close(FILE2);

print "struct entity_struct {\n";
print "   const char *name;\n";
print "   unsigned short value;";
print "};\n\n";
print "#define NB_ENTITIES $l \n\n";
print "entity_struct entities[${n}] = {\n";
$i = 0;
@c = @a;
@c = sort { ($a) cmp ($b)} @c;
while ($i < $l) {
	print $c[$i];
	$i++;
}
print "};\n";
print "/* nb entities : $l */\n";

