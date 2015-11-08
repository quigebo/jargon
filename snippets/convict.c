/*
 * convict.c
 *
 * Example chrooting program.
 *
 * Copyright 2002, James Lee and Bri Hatch
 *
 * Released under the GPL.  See COPYING file
 * for more information.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

/* User to run this as */
#define CONVICT "convict"

/* directory to which we'll chroot. */
#define CHROOT_DIR "/usr/local/convict"


/* No more changes needed. */
#define bail(x) { perror(x); exit(1); }

int main() {
	char filename[4096], buf[BUFSIZ];
	int fd, count;


	/* Get uid and gid for convict user */
	struct passwd *pw = getpwnam(CONVICT);
	if ( ! pw ) bail("getpwnam");

	/*
	 * chdir to our chroot area, chroot,
	 * and then setuid to convict user.
	 */
	if ( chdir(CHROOT_DIR) == -1) bail("chdir");
	if ( chroot(CHROOT_DIR) == -1) bail("chroot");
	if ( setgid(pw->pw_gid) == -1) bail("setgid");
	if ( setuid(pw->pw_uid) == -1) bail("setuid");

	/* Yeah, scanf sucks.  Sorry. */
	printf("Please enter a file name: ");
	scanf("%4095s", filename);
	printf("You entered %s\n", filename);

	/* print the file to stdout. */
	if ( (fd=open(filename, O_RDONLY)) >0) {
		printf("Contents of %s:\n", filename);
		while ( (count=read(fd, buf, BUFSIZ)) > 0 ) {
			write(1, buf, count);
		}
	} else {
		printf("Failed to open %s\n", filename);
	}
}
