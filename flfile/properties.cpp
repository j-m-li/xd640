/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
 *
 *                      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *      Neither the name of O'ksi'D nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   Author : Jean-Marc Lienher ( http://oksid.ch )
 *
 ******************************************************************************/


#include "properties.h"

#include <libintl.h>

#define _(String) gettext((String))

long Properties::tsize = 0;

int Properties::get_file_stat()
{
	char *ptr;
	int fd, ret;
	struct passwd *pwp;
	struct group *grp;
	struct stat *s = &file_status;
	FILE *fp;
	
	lstat(full_name, s);
	file_name->value(name);
	ptr = ctime(&s->st_ctime);
	if (ptr) {
		*(ptr + strlen(ptr) - 1) = '\0';
		created = strdup(ptr);
		file_created->value(created);
	}
	ptr = ctime(&s->st_mtime);
	if (ptr) {
		*(ptr + strlen(ptr) - 1) = '\0';
		modified = strdup(ptr);
		file_modified->value(modified);
	}
	ptr = ctime(&s->st_atime);
	if (ptr) {
		*(ptr + strlen(ptr) - 1) = '\0';
		accessed = strdup(ptr);
		file_accessed->value(accessed);
	}
	sprintf(buf, _("%ld Bytes"), s->st_size);
	size = strdup(buf);
	file_size->value(size);

	file_full_name->value(full_name);

	if (S_ISREG(s->st_mode)) {
		ret = -1;
		fd = open(full_name, O_RDONLY|O_NONBLOCK);
		if (fd > 0) {
			ret = read(fd, buf, 64);
			close(fd);
		}
		if (ret != -1) {
			int i = ret;
			while (i--) {
				if (buf[i] == '\0') buf[i] = ' ';
			}
			buf[ret] = '\0';
		} else {
			buf[0] = ' ';
			buf[1] = '\0';
		}
		content = strdup(buf);
	} else {
		content = strdup(" ");
	}
	file_content->value(content);
	file_content->position(0, 0);

	pwp = getpwuid(s->st_uid);	
	if (pwp) {
		owner = strdup(pwp->pw_name);
		file_owner->value(owner);
	}
	
	grp = getgrgid(s->st_gid);
	if (grp) {
		group = strdup(grp->gr_name);
		file_group->value(group);
	}

	if (s->st_mode & S_IRUSR) file_user_read->value(1);
	if (s->st_mode & S_IWUSR) file_user_write->value(1);
	if (s->st_mode & S_IXUSR) file_user_exec->value(1);
	if (s->st_mode & S_IRGRP) file_group_read->value(1);
	if (s->st_mode & S_IWGRP) file_group_write->value(1);
	if (s->st_mode & S_IXGRP) file_group_exec->value(1);
	if (s->st_mode & S_IROTH) file_other_read->value(1);
	if (s->st_mode & S_IWOTH) file_other_write->value(1);
	if (s->st_mode & S_IXOTH) file_other_exec->value(1);
	if (s->st_mode & S_ISUID) file_uid->value(1);
	if (s->st_mode & S_ISGID) file_gid->value(1);
	if (s->st_mode & S_ISVTX) file_sticky->value(1);

	sprintf(buf, "file -b '%s'", full_name);
	fp = popen(buf, "r");
	fgets(buf, 2048, fp);
	fclose(fp);
	buf[2047] = '\0';
	*(buf + strlen(buf) - 1) = '\0';
	type = strdup(buf);
	file_type->value(type);
	file_type->position(0, 0);

	return 1;
}

int Properties::fn(const  char  *file, const  struct  stat  *sb,
        int  flag,  struct FTW *s)
{
	tsize += sb->st_size;
	return 0;
}

char *Properties::get_sizes(char **lst, int nb)
{
	struct stat st;
	static char buf[80];

	tsize = 0;
	while (nb > 0) {
		nb--;
		if (!lstat(lst[nb], &st)) {
			if (S_ISREG(st.st_mode)) {
				tsize += st.st_size;
			} else if (S_ISDIR(st.st_mode)) {
				nftw(lst[nb], fn, 20,
					FTW_CHDIR | FTW_DEPTH | 
					FTW_MOUNT | FTW_PHYS);
			}
		}
	}

	snprintf(buf, 80, "%ld kb   (%ld bytes)", tsize / 1024, tsize);
	return buf;
}

int Properties::setup(char **lst, int nb, int mode) 
{
	full_name = lst[0];
	name = strrchr(full_name, '/') + 1;

	directory = strdup(full_name);
	char *ptr = strrchr(directory, '/');
	*ptr = '\0';

	ask = mode;
	begin();

	if (nb > 1) {
		char buf[80];
		Fl_Button *cancel;
		cancel = new Fl_Button(300, 270, 80, 25, _("Cancel"));
		cancel->callback(cb_cancel, this);
		label(directory);
		nb_files = new Fl_Output(200, 40, 180, 20, 
			_("Nb selected files:"));
		nb_files->color(FL_GRAY);
		snprintf(buf, 80, _("%d files"), nb);
		nb_files->value(buf);

		total_size = new Fl_Output(200, 70, 180, 20, 
			_("Total size:"));
		total_size->color(FL_GRAY);
		total_size->value(get_sizes(lst, nb));
		end();
		return 0;
	}

	label(name);
	Fl_Tabs *tabs = new Fl_Tabs(5, 5, 440, 260);

	/*******************************************************************/
	Fl_Group *stat = new Fl_Group(5, 25, 440, 260, _("Statistics"));
	file_full_name = new Fl_Output(100, 40, 280, 20, _("Full Name:"));
	file_full_name->color(FL_GRAY);
	file_name = new Fl_Input(100, 60, 280, 20, _("File Name:"));

	file_type = new Fl_Output(140, 90, 280, 20, _("File Type:"));
	file_type->color(FL_GRAY);
	file_size = new Fl_Output(140, 110, 280, 20, _("File Size:"));
	file_size->color(FL_GRAY);

	file_created = new Fl_Output(160, 140, 260, 20, 
							_("File Created on:"));
	file_created->color(FL_GRAY);
	file_modified = new Fl_Output(160, 160, 260, 20, 
							_("File Modified on:"));
	file_modified->color(FL_GRAY);
	file_accessed = new Fl_Output(160, 180, 260, 20, 
							_("File Accessed on:"));
	file_accessed->color(FL_GRAY);

	file_content = new Fl_Multiline_Output(100, 210, 
							320, 40, _("Content:"));
	file_content->color(FL_GRAY);
	file_content->value("kh\0x34\0x11\n");

	stat->end();
	Fl_Group::current()->resizable(stat);
	/*******************************************************************/
	Fl_Group *perm = new Fl_Group(5, 25, 440, 260, _("Permissions"));
	perm->hide();
	Fl_Box * file_perm = new Fl_Box(FL_FLAT_BOX,10, 40, 180, 20, 
							_("File Permissions")); 
	file_perm->color((Fl_Color)47);
	file_perm->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	
	Fl_Box * file_user_lab = new Fl_Box(FL_FLAT_BOX,10, 70, 90, 20, 
								_("User"));
	file_user_lab->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	file_user_lab->color((Fl_Color)47);
	file_user_read = new Fl_Check_Button(110, 70, 70, 20, _("Read")); 
	file_user_write = new Fl_Check_Button(180, 70, 70, 20, _("Write")); 
	file_user_exec = new Fl_Check_Button(250, 70, 70, 20, _("Exec")); 
	file_uid = new Fl_Check_Button(320, 70, 70, 20, _("Set UID")); 
	
	Fl_Box *file_grp_lab = new Fl_Box(FL_FLAT_BOX,10, 90, 90, 20, 
								_("Group"));
	file_grp_lab->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	file_grp_lab->color((Fl_Color)47);
	file_group_read = new Fl_Check_Button(110, 90, 70, 20, _("Read")); 
	file_group_write = new Fl_Check_Button(180, 90, 70, 20, _("Write")); 
	file_group_exec = new Fl_Check_Button(250, 90, 70, 20, _("Exec")); 
	file_gid = new Fl_Check_Button(320, 90, 70, 20, _("Set GID")); 

	Fl_Box * file_other_lab = new Fl_Box(FL_FLAT_BOX,10, 110, 90, 20, 
								_("Other"));
	file_other_lab->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	file_other_lab->color((Fl_Color)47);
	file_other_read = new Fl_Check_Button(110, 110, 70, 20, _("Read")); 
	file_other_write = new Fl_Check_Button(180, 110, 70, 20, _("Write")); 
	file_other_exec = new Fl_Check_Button(250, 110, 70, 20, _("Exec")); 
	file_sticky = new Fl_Check_Button(320, 110, 70, 20, _("Sticky")); 

	Fl_Box * file_ownership = new Fl_Box(FL_FLAT_BOX, 10, 150, 180, 20, 
							_("File ownership")); 
	file_ownership->color((Fl_Color)47);
	file_ownership->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	file_owner = new Fl_Input(100, 180, 240, 20, _("Owner"));
	file_group = new Fl_Input(100, 200, 240, 20, _("Group"));
	perm->end();
	Fl_Group::current()->resizable(perm);
	/*******************************************************************/

	tabs->end();

	Fl_Button *cancel = new Fl_Button(210, 270, 80, 25, _("Cancel"));
	cancel->callback(cb_cancel, this);
	Fl_Button *ok  = new Fl_Return_Button(300, 270, 80, 25, _("OK"));
	ok->callback(cb_ok, this);
	end();
	get_file_stat();
	return 1;
}

int  Properties::exec(const Fl_Window* modal_for)
{
        show();
        return Fl::run();
}

void Properties::cb_cancel(Fl_Widget *, void *data)
{
	Properties *self = (Properties *)data;

	free((void *)self->created);
	free((void *)self->modified);
	free((void *)self->accessed);
	free((void *)self->size);
	free((void *)self->directory);
	free((void *)self->content);
	free((void *)self->owner);
	free((void *)self->group);
	free((void *)self->type);

	self->hide();
}

void Properties::cb_ok(Fl_Widget *w, void *data)
{
	Properties *self = (Properties *)data;
	int ret;
	mode_t mod = 0;
	mode_t omod = self->file_status.st_mode & ~S_IFMT;
	struct passwd *pwp;
        struct group *grp;
	errno = 0;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (self->file_user_read->value()) mod |= S_IRUSR;
	if (self->file_user_write->value()) mod |= S_IWUSR;
	if (self->file_user_exec->value()) mod |= S_IXUSR;
	if (self->file_group_read->value()) mod |= S_IRGRP;
	if (self->file_group_write->value()) mod |= S_IWGRP;
	if (self->file_group_exec->value()) mod |= S_IXGRP;
	if (self->file_other_read->value()) mod |= S_IROTH;
	if (self->file_other_write->value()) mod |= S_IWOTH;
	if (self->file_other_exec->value()) mod |= S_IXOTH;
	if (self->file_uid->value()) mod |= S_ISUID;
	if (self->file_gid->value()) mod |= S_ISGID;
	if (self->file_sticky->value()) mod |= S_ISVTX;
	if (mod != omod) {
		if (self->ask) {
			ret = fl_ask(_("Do you really want to change"
				" permissions on \"%s\"?"), self->name);
		} else {
			ret = 1;
		}
		if (ret == 1) {
			ret = chmod(self->full_name, mod);
			if (ret == -1) {
				fl_alert("chmod: %s", strerror(errno));
			}
		}
	}
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (strcmp(self->group, self->file_group->value())) {
		if (self->ask) {
			ret = fl_ask(_("Do you really want to change group "
					"from \"%s\" to \"%s\"?"),
					self->group,
					self->file_group->value());
		} else {
			ret = 1;
		}
		if (ret == 1) {
			grp = getgrnam(self->file_group->value());
			if (grp) {
				ret = lchown(self->full_name, (unsigned int)-1, 
							grp->gr_gid);
				if (ret == -1) {
					fl_alert("lchown: %s", strerror(errno));
				}
			} else {
				fl_alert("getgrnam: %s", strerror(errno));
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (strcmp(self->owner, self->file_owner->value())) {
		if (self->ask) {
			ret = fl_ask(_("Do you really want to change owner "
					"from \"%s\" to \"%s\"?"),
					self->owner,
					self->file_owner->value());
		} else {
			ret = 1;
		}
		if (ret == 1) {
			pwp = getpwnam(self->file_owner->value());
			if (pwp) {
				ret = lchown(self->full_name, pwp->pw_uid, 
							(unsigned int)-1);
				if (ret == -1) {
					fl_alert("lchown: %s", strerror(errno));
				}
			} else {
				fl_alert("getpwnam: %s", strerror(errno));
			}
		} 
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	if (strcmp(self->name, self->file_name->value())) {
		if (self->ask) {
			ret = fl_ask(_("Do you really want to rename "
					"\"%s\" to \"%s\"?"),
					self->name, 
					self->file_name->value());
		} else {
			ret = 1;
		}
		if (ret == 1) {
			sprintf(self->buf, "%s/%s", self->directory, 
						self->file_name->value());
			ret = rename(self->full_name, self->buf);
			if (ret == -1) {
				fl_alert("rename: %s", strerror(errno));
			}
		}
	}
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	cb_cancel(w, data);	
}
