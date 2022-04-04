/*
 * Copyright (C) 1996-2000 Michael R. Elkins <me@cs.hmc.edu>
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define mutt_strcasecmp strcasecmp
#define strfcpy(A,B,C) strncpy(A,B,C), *(A+(C)-1)=0
#define SHORT_STRING 256
#define SKIPWS(c) while (*(c) && isspace ((unsigned char) *(c))) c++;
#define mutt_strncasecmp strncasecmp

const char *Months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "ERR" };

static const char *uncomment_timezone (char *buf, size_t buflen, const char *tz)
{
  char *p;
  size_t len;

  if (*tz != '(')
    return tz; /* no need to do anything */
  tz++;
  SKIPWS (tz);
  if ((p = strpbrk (tz, " )")) == NULL)
    return tz;
  len = p - tz;
  if (len > buflen - 1)
    len = buflen - 1;
  memcpy (buf, tz, len);
  buf[len] = 0;
  return buf;
}

int mutt_check_month (const char *s)
{
  int i;

  for (i = 0; i < 12; i++)
    if (mutt_strncasecmp (s, Months[i], 3) == 0)
      return (i);
  return (-1); /* error */
}

/* returns the seconds east of UTC given `g' and its corresponding gmtime()
   representation */
static time_t compute_tz (time_t g, struct tm *utc)
{
  struct tm *lt = localtime (&g);
  time_t t;
  int yday;

  t = (((lt->tm_hour - utc->tm_hour) * 60) + (lt->tm_min - utc->tm_min)) * 60;

  if ((yday = (lt->tm_yday - utc->tm_yday)))
  {
    /* This code is optimized to negative timezones (West of Greenwich) */
    if (yday == -1 ||	/* UTC passed midnight before localtime */
	yday > 1)	/* UTC passed new year before localtime */
      t -= 24 * 60 * 60;
    else
      t += 24 * 60 * 60;
  }

  return t;
}

/* Returns the local timezone in seconds east of UTC for the time t,
 * or for the current time if t is zero.
 */
time_t mutt_local_tz (time_t t)
{
  struct tm *ptm;
  struct tm utc;

  if (!t)
    t = time (NULL);
  ptm = gmtime (&t);
  /* need to make a copy because gmtime/localtime return a pointer to
     static memory (grr!) */
  memcpy (&utc, ptm, sizeof (utc));
  return (compute_tz (t, &utc));
}

/* converts struct tm to time_t, but does not take the local timezone into
   account unless ``local'' is nonzero */
time_t mutt_mktime (struct tm *t, int local)
{
  time_t g;

  static int AccumDaysPerMonth[12] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
  };

  /* Compute the number of days since January 1 in the same year */
  g = AccumDaysPerMonth [t->tm_mon % 12];

  /* The leap years are 1972 and every 4. year until 2096,
   * but this algoritm will fail after year 2099 */
  g += t->tm_mday;
  if ((t->tm_year % 4) || t->tm_mon < 2)
    g--;
  t->tm_yday = g;

  /* Compute the number of days since January 1, 1970 */
  g += (t->tm_year - 70) * 365;
  g += (t->tm_year - 69) / 4;

  /* Compute the number of hours */
  g *= 24;
  g += t->tm_hour;

  /* Compute the number of minutes */
  g *= 60;
  g += t->tm_min;

  /* Compute the number of seconds */
  g *= 60;
  g += t->tm_sec;

  if (local)
    g -= compute_tz (g, t);

  return (g);
}

void mutt_normalize_time (struct tm *tm)
{
  static char DaysPerMonth[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

  while (tm->tm_sec < 0)
  {
    tm->tm_sec += 60;
    tm->tm_min--;
  }
  while (tm->tm_min < 0)
  {
    tm->tm_min += 60;
    tm->tm_hour--;
  }
  while (tm->tm_hour < 0)
  {
    tm->tm_hour += 24;
    tm->tm_mday--;
  }
  while (tm->tm_mon < 0)
  {
    tm->tm_mon += 12;
    tm->tm_year--;
  }
  while (tm->tm_mday < 0)
  {
    if (tm->tm_mon)
      tm->tm_mon--;
    else
    {
      tm->tm_mon = 11;
      tm->tm_year--;
    }
    tm->tm_mday += DaysPerMonth[tm->tm_mon];
  }
}

static struct tz_t
{
  char tzname[5];
  unsigned char zhours;
  unsigned char zminutes;
  unsigned char zoccident; /* west of UTC? */
}
TimeZones[] =
{
  { "aat",   1,  0, 1 }, /* Atlantic Africa Time */
  { "adt",   4,  0, 0 }, /* Arabia DST */
  { "ast",   3,  0, 0 }, /* Arabia */
/*{ "ast",   4,  0, 1 },*/ /* Atlantic */
  { "bst",   1,  0, 0 }, /* British DST */
  { "cat",   1,  0, 0 }, /* Central Africa */
  { "cdt",   5,  0, 1 },
  { "cest",  2,  0, 0 }, /* Central Europe DST */
  { "cet",   1,  0, 0 }, /* Central Europe */
  { "cst",   6,  0, 1 },
/*{ "cst",   8,  0, 0 },*/ /* China */
/*{ "cst",   9, 30, 0 },*/ /* Australian Central Standard Time */
  { "eat",   3,  0, 0 }, /* East Africa */
  { "edt",   4,  0, 1 },
  { "eest",  3,  0, 0 }, /* Eastern Europe DST */
  { "eet",   2,  0, 0 }, /* Eastern Europe */
  { "egst",  0,  0, 0 }, /* Eastern Greenland DST */
  { "egt",   1,  0, 1 }, /* Eastern Greenland */
  { "est",   5,  0, 1 },
  { "gmt",   0,  0, 0 },
  { "gst",   4,  0, 0 }, /* Presian Gulf */
  { "hkt",   8,  0, 0 }, /* Hong Kong */
  { "ict",   7,  0, 0 }, /* Indochina */
  { "idt",   3,  0, 0 }, /* Israel DST */
  { "ist",   2,  0, 0 }, /* Israel */
/*{ "ist",   5, 30, 0 },*/ /* India */
  { "jst",   9,  0, 0 }, /* Japan */
  { "kst",   9,  0, 0 }, /* Korea */
  { "mdt",   6,  0, 1 },
  { "met",   1,  0, 0 }, /* this is now officially CET */
  { "msd",   4,  0, 0 }, /* Moscow DST */
  { "msk",   3,  0, 0 }, /* Moscow */
  { "mst",   7,  0, 1 },
  { "nzdt", 13,  0, 0 }, /* New Zealand DST */
  { "nzst", 12,  0, 0 }, /* New Zealand */
  { "pdt",   7,  0, 1 },
  { "pst",   8,  0, 1 },
  { "sat",   2,  0, 0 }, /* South Africa */
  { "smt",   4,  0, 0 }, /* Seychelles */
  { "sst",  11,  0, 1 }, /* Samoa */
/*{ "sst",   8,  0, 0 },*/ /* Singapore */
  { "utc",   0,  0, 0 },
  { "wat",   0,  0, 0 }, /* West Africa */
  { "west",  1,  0, 0 }, /* Western Europe DST */
  { "wet",   0,  0, 0 }, /* Western Europe */
  { "wgst",  2,  0, 1 }, /* Western Greenland DST */
  { "wgt",   3,  0, 1 }, /* Western Greenland */
  { "wst",   8,  0, 0 }, /* Western Australia */
};

/* parses a date string in RFC822 format:
 *
 * Date: [ weekday , ] day-of-month month year hour:minute:second timezone
 *
 * This routine assumes that `h' has been initialized to 0.  the `timezone'
 * field is optional, defaulting to +0000 if missing.
 */
time_t mutt_parse_date (const char *s)
{
  int count = 0;
  char *t;
  int hour, min, sec;
  struct tm tm;
  int i;
  int tz_offset = 0;
  int zhours = 0;
  int zminutes = 0;
  int zoccident = 0;
  const char *ptz;
  char tzstr[SHORT_STRING];
  char scratch[SHORT_STRING];

  /* Don't modify our argument. Fixed-size buffer is ok here since
   * the date format imposes a natural limit. 
   */

  strfcpy (scratch, s, sizeof (scratch));

  /* kill the day of the week, if it exists. */
  if ((t = strchr (scratch, ',')))
    t++;
  else
    t = scratch;
  SKIPWS (t);

  memset (&tm, 0, sizeof (tm));

  while ((t = strtok (t, " \t")) != NULL)
  {
    switch (count)
    {
      case 0: /* day of the month */
        if (!isdigit ((unsigned char) *t))
          return (-1);
        tm.tm_mday = atoi (t);
        if (tm.tm_mday > 31)
          return (-1);
        break;

      case 1: /* month of the year */
        if ((i = mutt_check_month (t)) < 0)
          return (-1);
        tm.tm_mon = i;
        break;

      case 2: /* year */
        tm.tm_year = atoi (t);
        if (tm.tm_year < 70)
          tm.tm_year += 100;
        else if (tm.tm_year >= 1900)
          tm.tm_year -= 1900;
        break;

      case 3: /* time of day */
        if (sscanf (t, "%d:%d:%d", &hour, &min, &sec) == 3)
          ;
        else if (sscanf (t, "%d:%d", &hour, &min) == 2)
          sec = 0;
        else
        {
          return(-1);
        }
        tm.tm_hour = hour;
        tm.tm_min = min;
        tm.tm_sec = sec;
        break;

      case 4: /* timezone */
        /* sometimes we see things like (MST) or (-0700) so attempt to
         * compensate by uncommenting the string if non-RFC822 compliant
         */
        ptz = uncomment_timezone (tzstr, sizeof (tzstr), t);

        if (*ptz == '+' || *ptz == '-')
        {
          if (ptz[1] && ptz[2] && ptz[3] && ptz[4]
              && isdigit ((unsigned char) ptz[1]) && isdigit ((unsigned char) ptz[2])
              && isdigit ((unsigned char) ptz[3]) && isdigit ((unsigned char) ptz[4]))
          {
            zhours = (ptz[1] - '0') * 10 + (ptz[2] - '0');
            zminutes = (ptz[3] - '0') * 10 + (ptz[4] - '0');

            if (ptz[0] == '-')
              zoccident = 1;
          }
        }
        else
        {
          struct tz_t *tz;

          tz = (struct tz_t *) bsearch (ptz, TimeZones, 
			sizeof TimeZones/sizeof (struct tz_t),
                        sizeof (struct tz_t),
                        (int (*)(const void *, const void *)) strcasecmp
                        /* This is safe to do: A pointer to a struct equals
                         * a pointer to its first element*/);

          if (tz)
          {
            zhours = tz->zhours;
            zminutes = tz->zminutes;
            zoccident = tz->zoccident;
          }

          /* ad hoc support for the European MET (now officially CET) TZ */
          if (mutt_strcasecmp (t, "MET") == 0)
          {
            if ((t = strtok (NULL, " \t")) != NULL)
            {
              if (!mutt_strcasecmp (t, "DST"))
                zhours++;
            }
          }
        }
        tz_offset = zhours * 3600 + zminutes * 60;
        if (!zoccident)
          tz_offset = -tz_offset;
        break;
    }
    count++;
    t = 0;
  }

  if (count < 4) /* don't check for missing timezone */
  {
    return (-1);
  }

  return (mutt_mktime (&tm, 0) + tz_offset);
}

