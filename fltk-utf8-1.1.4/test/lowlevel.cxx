//
// "$Id: scroll.cxx,v 1.4.2.2 2000/06/05 21:21:24 mike Exp $"
//
// mac port test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <FL/filename.H>
#include <FL/fl_utf8.H>



int main(int argc, char** argv) 
{
	int n;
	struct dirent **namelist = NULL;
	char buf[1024];
	printf("ho : %s, %s\n", fl_getenv("HOME"), fl_getcwd(buf, 1024));
//	n = fl_filename_list("~", &namelist, fl_alphasort);
	n = fl_filename_list(fl_getcwd(buf, 1024), &namelist, fl_alphasort);
	  if (n < 0)
          perror("scandir");
           else {
               while(n--) {
                   printf("%s\n", namelist[n]->d_name);
                   free(namelist[n]);
               }
               free(namelist);
           }
	printf(" %d %d \n", fl_filename_isdir("~"), 
		fl_filename_isdir("~/.DS_Store"));
	return 0;
}

//
// End of "$Id: scroll.cxx,v 1.4.2.2 2000/06/05 21:21:24 mike Exp $".
//
