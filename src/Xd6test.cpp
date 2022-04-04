#include <stdio.h>
#include "Xd6XmlTree.h"
#include "Xd6XmlParser.h"

int main (int argc, char **argv)
{
	Xd6XmlParser *parser;
	char *text = "hello, this is a test";
	char *text1 = "yesss hello, this is a sub test";


	parser = new Xd6XmlParser();

//	parser->tree->css->init_html();
/*
	parser->tree->add_text(text, strlen(text));
	
	parser->tree->add_element("go", 2);
	parser->tree->set_element_style();
	parser->tree->add_text(text1, strlen(text1));
	parser->tree->close_element("go", 2);
	
	parser->status = 0x1;
*/

	parser->parse_file(argv[1]);

//	parser->tree->dump_text(NULL);

	delete(parser->tree->root);
}


