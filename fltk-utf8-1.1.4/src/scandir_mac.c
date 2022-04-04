/*
 * "$Id:  Exp $"
 *
 *  CARBON scandir function for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2003 by Bill Spitzak and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "fltk-bugs@fltk.org".
 */

/* Emulation of posix scandir() call */
#include <stdlib.h>
#include <FL/filename.H>
#include <FL/fl_utf8.H>
#include "flstring.h"

extern "C" {

int fl_scandir(const char *dirname, struct dirent ***namelist,
               int (*select)(struct dirent *),
               int (*compar)(struct dirent **, struct dirent **)) ;

OSErr fl_get_fsref(const char *path, FSRef *ref);

static int match(unsigned short *s1, xchar *s2, int l)
{
  int i = 0;
  while (i < l) {
   if (s1[i] == '/') s1[i] = ':';
   if (s1[i] != s2[i]) return 0;
   i++;
  }
  return 1;
}

static OSErr find_volume(FSRef *&Directory, xchar *&ptr, int &l, 
	  xchar *&uniDir, int &len)
{
  OSErr ret;
  int index = 1;
  HFSUniStr255 volumeName;

  if (ptr[0] == '/') {
    ptr++;
    l = 0;
    while ((ptr[l] != '/' || l == 0)  && ptr + l < uniDir + len) {
      if (l == 0 && ptr[l] == '/') {
	ptr++;
      } else {
        l++;
      }
    }
    if (l == 0) {
      return -2; // / is not valid with MacOS
    }
    do {
      ret = FSGetVolumeInfo(kFSInvalidVolumeRefNum, index, NULL, 
              kFSVolInfoNone, NULL, &volumeName, Directory);
      if (ret == noErr) {
        int i = 0;
        if (volumeName.length == l && match(volumeName.unicode, ptr, l)) {
          break;
	}
      }
      index++;
    } while (ret == noErr);
  } else if (ptr[0] == '.' || ptr[0] != '~') {
    if (ptr + 1 ==  uniDir + len || ptr[1] == '/') {
      /* "./" */
      l = 1;
    } else {
      l = -1;
    }
  } else {
    FSSpec src;
    FSRefParam param;
    FSRef newRef;
  // kDesktopFolderType
  // kPreferencesFolderType
  // kTemporaryFolderType
    src.name[0] = 0;
    ret = FindFolder(kOnSystemDisk, kDesktopFolderType, kDontCreateFolder, 
		&src.vRefNum, &src.parID);
    ret = FSpMakeFSRef(&src, Directory);
    param.ref = Directory;
    param.whichInfo = kFSCatInfoNone;
    param.catInfo = NULL;
    param.spec = NULL;
    param.parentRef = Directory;
    param.outName = NULL;
    PBGetCatalogInfoSync(&param);
    if (ptr + 1 ==  uniDir + len || ptr[1] == '/') {
      /* "~/"  */
      l = 1;
    } else {
      /* "~user/" */
      param.parentRef = &newRef;
      ret = PBGetCatalogInfoSync(&param);
      if (ret == noErr) {
	/* MacOS X */
        param.parentRef = Directory;
        PBGetCatalogInfoSync(&param);
        l = 0;
      } else {
        /* MacOS 9 */
        ptr++;
        l = 0;
        while (ptr[l] != '/' && ptr + l < uniDir + len) {
          l++;
        }
      }
    }
  }
  if (ret != noErr) return -1;
  return ret;
}

OSErr fl_get_fsref(const char *path, FSRef *Directory)
{
  OSErr ret;
  FSIterator iterator;
  int len, l;
  xchar *uniDir, *ptr;
  FSRef Volume;
  FSSpec src;
  
  len = strlen(path);
  uniDir = (xchar*) malloc((len + 1) * sizeof(xchar));
  len = fl_utf2unicode((unsigned char*)path, len, uniDir);
  uniDir[len] = 0;

  src.name[0] = 0;
  ret = FindFolder(kOnSystemDisk, kDesktopFolderType, kDontCreateFolder, 
		&src.vRefNum, &src.parID);
  ret = FSpMakeFSRef(&src, Directory);

  ptr = uniDir;
  l = 0;
  ret = find_volume(Directory, ptr, l, uniDir, len);
  
  if (ret != noErr || ptr + l >= uniDir + len) {
    free(uniDir);
    return ret;
  }

  memcpy(&Volume, Directory, sizeof(FSRef)); 
 
  do {
    ptr += l + 1;
    l = 0;
    while ((ptr[l] != '/' || l == 0)  && ptr + l < uniDir + len) {
      if (l == 0 && ptr[l] == '/') {
	ptr++;
      } else {
        l++;
      }
    }
    if (ptr >= uniDir + len) {
      free(uniDir);
      return noErr; // found it !
    }
    if (ptr[0] == '.') {
      if (l == 1) {
        continue;
      } else if (l == 2 && ptr[1] == '.') {
	// go back
	FSRefParam param;
	if (noErr == FSCompareFSRefs(Directory, &Volume)) {
	  ptr += l;
	  ret = find_volume(Directory, ptr, l, uniDir, len);
	  if (ret != noErr) {
	    free(uniDir);
	    return ret;
	  }
  	  memcpy(&Volume, Directory, sizeof(FSRef)); 
	} else {
	  param.ref = Directory;
	  param.whichInfo = kFSCatInfoNone;
	  param.catInfo = NULL;
	  param.spec = NULL;
	  param.parentRef = Directory;
	  param.outName = NULL;
	  PBGetCatalogInfoSync(&param);
        }
	continue;
      }
    } 
    ret = FSOpenIterator(Directory, kFSIterateFlat, &iterator);
    do {
      ItemCount actualObjects;
      HFSUniStr255 names[1];
      FSRef refs[1];
      ret = FSGetCatalogInfoBulk(iterator, 1, &actualObjects,
	      NULL, kFSCatInfoNone, NULL, refs, NULL, names);
      if (ret == noErr && names[0].length == l && 
         match(names[0].unicode, ptr, l))
      {
        FSCloseIterator(iterator);
        memcpy(Directory, refs, sizeof(FSRef)); 
        break;
      }
    } while (ret == noErr);
  } while (ret == noErr);

  FSCloseIterator(iterator);

  free(uniDir);
  return -1;
}

int fl_scandir(const char *dirname, struct dirent ***namelist,
	       int (*select)(struct dirent *),
	       int (*compar)(struct dirent **, struct dirent **)) {

  OSErr ret;
  short index = 1;
  unsigned char buf[1024];
  xchar wbuf[1024];
  StringPtr volName = buf;
  SInt16 vRefNum = 0;
  SInt32 dirID = 0;
  FSRef newRef;
  FSIterator iterator;
  struct dirent **dir = 0, *selectDir;
  int nDir = 0, NDir = 0;
	
//	ret = FSMakeFSRefUnicode(&parentRef, nameLength, name, 
//		kTextEncodingUnknown, &newRef);
 
  buf[0] = 0;
  ret = fl_get_fsref(dirname, &newRef);

  if (ret == -2) {
    /* / !!! */
    int index = 1;
    HFSUniStr255 volumeName;
    *namelist = dir;
    do {
      ret = FSGetVolumeInfo(kFSInvalidVolumeRefNum, index, NULL, 
                      kFSVolInfoNone, NULL, &volumeName, NULL);
      if (ret == noErr) {
        int i, l;
	for (i = 0; i < volumeName.length; i++) {
 	  if (volumeName.unicode[i] == '/') {
	    wbuf[i] = ':';
	  } else {
	    wbuf[i] = volumeName.unicode[i];
	  }
	}
        selectDir=(struct dirent*)malloc(sizeof(struct dirent)+
		(volumeName.length + 2) * 5);
        l = fl_unicode2utf(wbuf, volumeName.length,
		selectDir->d_name);
	selectDir->d_name[l] = '/';
	l++;
	selectDir->d_name[l] = 0;
	if (!select || (*select)(selectDir)) {
          if (nDir==NDir) {
            struct dirent **tempDir = (struct dirent **) calloc(
					sizeof(struct dirent*), NDir+33);
            if (NDir) memcpy(tempDir, dir, sizeof(struct dirent*)*NDir);
            if (dir) free(dir);
            dir = tempDir;
            NDir += 32;
          }
          selectDir = (struct dirent*) realloc(selectDir, 
			sizeof(struct dirent) + l + 1);
          dir[nDir] = selectDir;
          nDir++;
          dir[nDir] = 0;
        } else {
            free(selectDir);
        }
      }
      index++;
    } while (ret == noErr);
    if (compar) qsort (dir, nDir, sizeof(*dir),
                       (int(*)(const void*, const void*))compar);
    *namelist = dir;
    return nDir;
  } else if (ret != noErr) {
    return -1;
  }

  *namelist = dir;
  ret = FSOpenIterator(&newRef, kFSIterateFlat, &iterator);
	
  do {
    ItemCount actualObjects;
    HFSUniStr255 names[1];
    FSCatalogInfo info;
    ret = FSGetCatalogInfoBulk(iterator, 1, &actualObjects,
	NULL, kFSCatInfoNodeFlags, &info, NULL, NULL, names);
    if (ret == noErr) {
      int i, l;
      for (i = 0; i < names[0].length; i++) {
        if (names[0].unicode[i] == '/') {
          wbuf[i] = ':';
        } else {
          wbuf[i] = names[0].unicode[i];
	}
      }
      selectDir=(struct dirent*)malloc(sizeof(struct dirent)+
		(names[0].length + 2) * 5);
      l = fl_unicode2utf(wbuf, names[0].length,
		selectDir->d_name);
      if (info.nodeFlags & kFSNodeIsDirectoryMask) {
        selectDir->d_name[l] = '/';
        l++;
      }
      selectDir->d_name[l] = 0;
      if (!select || (*select)(selectDir)) {
        if (nDir==NDir) {
          struct dirent **tempDir = (struct dirent **) calloc(
				sizeof(struct dirent*), NDir+33);
          if (NDir) memcpy(tempDir, dir, sizeof(struct dirent*)*NDir);
          if (dir) free(dir);
          dir = tempDir;
          NDir += 32;
        }
        selectDir = (struct dirent*) realloc(selectDir, 
			sizeof(struct dirent) + l + 1);
        dir[nDir] = selectDir;
        nDir++;
        dir[nDir] = 0;
      } else {
        free(selectDir);
      }
    }
  } while (ret == noErr);

  FSCloseIterator(iterator);

  if (compar) qsort (dir, nDir, sizeof(*dir),
                       (int(*)(const void*, const void*))compar);
  *namelist = dir;
  return nDir;
}

}
/*
 * End of "$Id:  Exp $".
 */
