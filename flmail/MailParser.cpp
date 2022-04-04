/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 *   Author : Jean-Marc Lienher ( http://oksid.ch )
 *
 ******************************************************************************/


#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libintl.h>
#include <dirent.h>
#include <sys/stat.h>
#include "xd640/Xd6XmlUtils.h"
#include "MailParser.h"


#define _(String) gettext((String))

extern char **environ;
extern time_t mutt_parse_date(const char *s);

static unsigned char fromhex(char cc)
{
	unsigned char c = (unsigned char) cc;
	if (c >= 0x41 && c <= 0x46) {
		return c - 0x41 + 10;
	} else {
		return c - 0x30;
	}
}

MailParser::MailParser()
{
	buf_len = 1024;
	xml_len = 6 * buf_len;
	xml = (char*)malloc(xml_len);
	buf = (char*)malloc(buf_len);

	Message_ID = NULL;
	Subject = NULL;
	To = NULL;
	Reply_To = NULL;
	Cc = NULL;
	From = NULL;
	Date = NULL;
	Content_Type = NULL;
	Content_Transfer_Encoding = NULL;
}

MailParser::~MailParser()
{
	free(xml);
	free(buf);
	free(Message_ID);
	free(Subject);
	free(To);
	free(Reply_To);
	free(Cc);
	free(From);
	free(Date);
	free(Content_Type);
	free(Content_Transfer_Encoding);
}

static int selfile(const struct dirent *d)
{
	int i;
	return sscanf(d->d_name, "download.%d.txt", &i);
}

void MailParser::double_buffer()
{
	buf_len *= 2;
	xml_len *= 2;
	buf = (char*) realloc(buf, buf_len);
	xml = (char*) realloc(xml, xml_len);
}

int MailParser::decode_header()
{
	int l, n, ql;
	char *ptr;
	int len;

	len = strlen(buf);
	l = 0;
	n = 0;
	ptr = buf;
	
	while (buf[l] && buf[l] != ':') l++;
	if (!buf[l]) return len;
	l += 2;
	n = l;
	if (buf[l] == '=' && buf[l + 1] == '?' && buf[len - 1] == '=' &&
		buf[len - 2] == '?')
	{
		l += 2;
		buf[len - 2] = '\0';
	} else {
		return len;
	}
	if (!strncmp("utf-7?", buf + l, 6)) {
		l += 6;
		if (buf[l] == 'Q') {
			l += 2;
			ql = quoted2utf8(buf + l, strlen(buf + l));
			ql = utf72utf8(buf + l, ql);
			while (ql > 0) {
				ql--;
				buf[n++] = buf[l];
				l++;
			}
		} else {
			return len - 2;
		}
	} if (!strncmp("utf-8?", buf + l, 6)) {
		l += 6;
		if (buf[l] == 'Q') {
			l += 2;
			ql = quoted2utf8(buf + l, strlen(buf + l));
			while (ql > 0) {
				ql--;
				buf[n++] = buf[l];
				l++;
			}
		} else {
			return len - 2;
		}
	} else {
		while (buf[l] != '?') l++;
		l++;
		if (buf[l] == 'Q') {
			l += 2;
			ql = quoted2utf8(buf + l, strlen(buf + l));
			l = fl_latin12utf((unsigned char*)buf + l, ql, buf + n);
			n += l;	
		} else {
			return len - 2;
		}
	} 
	

	buf[n] = '\0';
	return n;
}

int MailParser::get_header_line(FILE *fp) 
{
	int c;
	int i = 0;
	buf[0] = '\0';
	while ((c = fgetc(fp)) != EOF) {
		if (buf_len < i + 10) {
			double_buffer();
		}
		if (c == '\n') {
			if (i == 0) return 0;
			if ((c = fgetc(fp)) == '\t' || c == ' ') {
				buf[i] = ' ';
			} else {
				ungetc(c, fp);
				break;
			}
		} else {
			buf[i] = (unsigned char) c;
		}
		i++;
	}
	buf[i] = '\0';
	return decode_header();
}

void MailParser::mail2html(const char *f, const char *p, FILE *index)
{
	FILE *file_fp;
	FILE *box_fp;
	int l;
	char *t;
	snprintf(buf, 1024, "%s/%s", p, f);
	box_fp = fopen(buf, "r");
	if (!box_fp) return;
	//unlink(buf);
	snprintf(buf, 1024, "%s/%s.html", p, f);
	file_fp = fopen(buf, "w");
	fprintf(file_fp, "<html><head></head><body><form>\n");

	while ((l = get_header_line(box_fp)) > 0) {
		char *ptr;
	
		ptr = buf;
		while (*ptr && *ptr != ':') ptr++;
		if (*ptr) {
			*ptr = '\0';
			ptr++;
			if (*ptr) ptr++;
		}

		if (!strcmp("Message-ID", buf)) {
			Message_ID = strdup(ptr);
		} else if (!strcmp("Subject", buf)) {
			Subject = strdup(ptr);
		} else if (!strcmp("To", buf)) {
			To = strdup(ptr);
		} else if (!strcmp("Reply-To", buf)) {
			Reply_To = strdup(ptr);
		} else if (!strcmp("Cc", buf)) {
			Cc = strdup(ptr);
		} else if (!strcmp("From", buf)) {
			From = strdup(ptr);
		} else if (!strcmp("Date", buf)) {
			Date = strdup(ptr);
		} else if (!strcmp("Content-Type", buf)) {
			Content_Type = strdup(ptr);
		} else if (!strcmp("Content-Transfer-Encoding", buf)) {
			Content_Transfer_Encoding = strdup(ptr);
		}
		fprintf(file_fp, "<input type=\"hidden\"");
		utf2xml(buf, strlen(buf), xml);
		fprintf(file_fp, " name=\"%s", xml);
		utf2xml(ptr, strlen(ptr), xml);
		fprintf(file_fp, "\" value=\"%s\" >\n", xml);
	}

	if (Subject) {
		utf2xml(Subject, strlen(Subject), xml);
		fprintf(file_fp, _("<b>Subject:</b> %s <br/>\n"), xml);		
	}
	if (From) {
		utf2xml(From, strlen(From), xml);
		fprintf(file_fp, _("<b>From:</b> %s <br/>\n"), xml);		
	}
	if (To) {
		utf2xml(To, strlen(To), xml);
		fprintf(file_fp, _("<b>To:</b> %s <br/>\n"), xml);		
	}
	if (Cc) {
		utf2xml(Cc, strlen(Cc), xml);
		fprintf(file_fp, _("<b>Cc:</b> %s <br/>\n"), xml);		
	}
	if (Reply_To) {
		utf2xml(Reply_To, strlen(Reply_To), xml);
		fprintf(file_fp, _("<b>Reply-To:</b> %s <br/>\n"), xml);
	}
	if (Date) {
		utf2xml(Date, strlen(Date), xml);
		fprintf(file_fp, _("<b>Date:</b> %s <br/><hr/>\n"), xml);
		date_time = mutt_parse_date(Date);
	}
	
	fprintf(file_fp, "</form>\n");

	if (!Content_Type) {
		while (fgets(buf, buf_len, box_fp)) {
			utf2xml(buf, strlen(buf), xml);
			fprintf(file_fp, "%s", xml);
		}
	} else if (strstr(Content_Type, "multipart/alternative;")) {
		print_multipart(box_fp, file_fp);
	} else if (strstr(Content_Type, "multipart/mixed;")) {
		print_multipart(box_fp, file_fp);
	} else if (strstr(Content_Type, "text/plain;") ||
		strstr(Content_Type, "TEXT/PLAIN;")) 
	{
		print_text_plain(box_fp, file_fp);
	} else if (strstr(Content_Type, "text/html;")) {

	} else {
		while (fgets(buf, buf_len, box_fp)) {
			utf2xml(buf, strlen(buf), xml);
			fprintf(file_fp, "%s", xml);
		}
	}
	fprintf(file_fp, "</body></html>\n");
	fclose(box_fp);
	fclose(file_fp);
	t = ctime(&date_time);
	t[strlen(t) - 1] = '\0';
	fprintf(index, "\vb%s\f\vi%s\f%s\f\vC7%s\n", Subject, From, 
		t, Message_ID);
	snprintf(buf, 1024, "%s/%s.html", p, f);
	snprintf(xml, 1024, "%s/%s", p, Message_ID);
	rename(buf, xml);
}

int MailParser::get_quoted_line(FILE *fp, char *b, int bl)
{
	int c;
	int i = 0;
	buf[0] = '\0';
	int bound = 1;

	while ((c = fgetc(fp)) != EOF) {
		if (buf_len < i + 10) {
			double_buffer();
		}
		
		if (b && bound && i < bl) {
			if (b[i] != c) bound = 0;
		}
		if (!bound && c == '=') {
			if ((c = fgetc(fp)) && c == '\n') {
				continue;
			} else {
				buf[i] = fromhex(c) << 4;
				c = fgetc(fp);
				buf[i] |= fromhex(c);
			}
		} else if (c == '\n') {
			buf[i++] = c;
			break;
		} else {
			buf[i] = (unsigned char) c;
		}
		i++;
	}
	buf[i] = '\0';
	if (bound && i >= bl) return 0;
	return i;
}

char *MailParser::get_boundary()
{
	char *boundary = NULL;
	char *ptr;
	char *c;
	char *b;

	c = strdup(Content_Type);
	ptr = strstr(c, "boundary");
	if (!ptr) {
		free(c);
		return NULL;
	}
	while (*ptr && *ptr != '"') ptr++;
	if (*ptr) ptr++;
	b = ptr;
	while (*ptr && *ptr != '"') ptr++;
	*ptr = '\0';
	boundary = (char*) malloc(strlen(b) + 10);
	sprintf(boundary, "--%s", b);
	free(c);
	return boundary;
}

void MailParser::print_multipart(FILE *in, FILE *out)
{
	char *b = get_boundary();
	int bl;
	int l;
	
	if (!b) return;
	bl = strlen(b);

	while (fgets(buf, buf_len, in)) {
		if (!strncmp(buf, b, bl)) {
			if (buf[bl] == '-' && buf[bl + 1] == '-') return;
			break;
		}
	}

	while (!feof(in)) {
		while ((l = get_header_line(in)) > 0) {
			char *ptr;
		
			ptr = buf;
			while (*ptr && *ptr != ':') ptr++;
			if (*ptr) {
				*ptr = '\0';
				ptr++;
				if (*ptr) ptr++;
			}
			if (!strcmp("Content-Type", buf)) {
				free(Content_Type);
				Content_Type = strdup(ptr);
			} else if (!strcmp("Content-Transfer-Encoding", buf)) {
				free(Content_Transfer_Encoding);
				Content_Transfer_Encoding = strdup(ptr);
			}
		}
		if (strstr(Content_Type, "text/plain;") ||
			strstr(Content_Type, "TEXT/PLAIN;")) 
		{
			print_text_plain(in, out, b);
		} else {
			print_application(in, out, b);
		}
		if (buf[bl] == '-' && buf[bl + 1] == '-') return;
	}
}

void MailParser::print_application(FILE *in, FILE *out, char *b = NULL)
{
	int bl = 0;
	char *ptr;
	char *name = "data.bin";

	if (b) {
		bl = strlen(b);
	}
	
	ptr = strstr(Content_Type, "name");
	if (ptr) {	
		while (*ptr && *ptr != '"') ptr++;
		if (*ptr) ptr++;
		if (*ptr) {
			name = ptr;
			while (*ptr && *ptr != '"') ptr++;
			*ptr = '\0';
		}
	}
	utf2xml(name, strlen(name), xml);
	fprintf(out, _("<br />Attachment: "));
	fprintf(out, " <a name=\"%s\" ", xml);
	fprintf(out, "\nhref=\"data:application/octet-stream;base64,\n");

	if (strstr(Content_Transfer_Encoding, "quoted-printable")) {
		while (fgets(buf, buf_len, in)) {
			if (b && !strncmp(buf, b, bl)) break;
		}
	} else if (strstr(Content_Transfer_Encoding, "base64")) {
		while (fgets(buf, buf_len, in)) {
			if (b && !strncmp(buf, b, bl)) break;
			fprintf(out, "%s", buf);
		}
	} else {
		while (fgets(buf, buf_len, in)) {
			if (b && !strncmp(buf, b, bl)) break;
		}

	}
	utf2xml(name, strlen(name), xml);
	fprintf(out, "\">\n %s</a> (%s)\n", xml, Content_Type);
}

void MailParser::print_text_plain(FILE *in, FILE *out, char *b = NULL)
{
	int bl = 0;
	
	if (b) {
		bl = strlen(b);
	}
	fprintf(out, "<pre\n>");
	if (strstr(Content_Type, "ISO-8859-") ||
		strstr(Content_Type, "iso-8859-"))
	{
		// we only support iso-8859-1 !
		charset = C_ISO;
	} else if (strstr(Content_Type, "utf-7") ||
		strstr(Content_Type, "UTF-7")) {
		charset = C_UTF7;
	} else {
		charset = C_UTF8;
	}
	if (strstr(Content_Transfer_Encoding, "quoted-printable")) {
		int l;
		if (charset == C_UTF8) {
			while ((l = get_quoted_line(in, b, bl))) {
				utf2xml(buf, strlen(buf), xml);
				fprintf(out, "%s", xml);
			}
		} else if (charset == C_ISO) {
			while ((l = get_quoted_line(in, b, bl))) {
				latin12xml(buf, strlen(buf), xml);
				fprintf(out, "%s", xml);
			}
		} else {
			while ((l = get_quoted_line(in, b, bl))) {
				utf72utf8(buf, buf_len);
				utf2xml(buf, strlen(buf), xml);
				fprintf(out, "%s", xml);
			}
		}
	} else if (strstr(Content_Transfer_Encoding, "base64")) {
			while (fgets(buf, buf_len, in)) {
				if (b && !strncmp(buf, b, bl)) break;
			}
	} else {
		if (charset == C_UTF8) {
			while (fgets(buf, buf_len, in)) {
				if (b && !strncmp(buf, b, bl)) break;
				utf2xml(buf, strlen(buf), xml);
				fprintf(out, "%s", xml);
			}
		} else if (charset == C_ISO) {
			while (fgets(buf, buf_len, in)) {
				if (b && !strncmp(buf, b, bl)) break;
				latin12xml(buf, strlen(buf), xml);
				fprintf(out, "%s", xml);
			}
		} else {
			while (fgets(buf, buf_len, in)) {
				if (b && !strncmp(buf, b, bl)) break;
				utf72utf8(buf, buf_len);
				utf2xml(buf, strlen(buf), xml);
				fprintf(out, "%s", xml);
			}
		}
	}
	fprintf(out, "</pre\n>");
		
}

void MailParser::mailbox2html(const char *path)
{
	char buf[1024];
	FILE *index_fp;
	int nbd;
	struct dirent **namelist = NULL;
	int i;
	
	nbd = scandir(path, &namelist, selfile, alphasort);
	snprintf(buf, 1024, "%s/index.txt", path);
	index_fp = fopen(buf, "a");

	for (i = 0; i < nbd; i++) {
		mail2html(namelist[i]->d_name, path, index_fp);
		free(namelist[i]);
	}
	free(namelist);
	fclose(index_fp);
}

/*
 * "$Id: $"
 */

