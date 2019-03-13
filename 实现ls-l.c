//实现ls -l files功能
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

int main(int argc, char *argv[])
{
	struct stat buf;
	char type, ownrd, ownwt, ownet, grprd, grpwt, grpet, othrd, othwt, othet;
	struct passwd *pa;
	struct group *gp;
	struct tm *ct;

	if (argc < 2) {
		printf("./ls files\n");
		exit(1);
	}

	if (stat(argv[1], &buf) < 0) {
		perror("stat");
		exit(1);
	}
	printf("%s\t%ld\n", argv[1], buf.st_size);

	//file type & mode (permissions)
	if (S_ISDIR(buf.st_mode))
		type = 'd';
	else if (S_ISCHR(buf.st_mode))
		type = 'c';
	else if (S_ISBLK(buf.st_mode))
		type = 'b';
	else if (S_ISFIFO(buf.st_mode))
		type = 'p';
	else if (S_ISLNK(buf.st_mode))
		type = 'l';
	else if (S_ISSOCK(buf.st_mode))
		type = 's';
	else
		type = '-';
		
	if (buf.st_mode & S_IRUSR)
		ownrd = 'r';
	if (buf.st_mode & S_IWUSR)
		ownwt = 'w';
	if (buf.st_mode & S_IXUSR)
		ownet = 'x';
	if (buf.st_mode & S_IRGRP)
		grprd = 'r';
	if (buf.st_mode & S_IWGRP)
		grpwt = 'w';
	if (buf.st_mode & S_IXGRP)
		grpet = 'x';
	if (buf.st_mode & S_IROTH)
		othrd = 'r';
	if (buf.st_mode & S_IWOTH)
		othwt = 'w';
	if (buf.st_mode & S_IXOTH)
		othet = 'x';

	pa = getpwuid(buf.st_uid);
	gp = getgrgid(buf.st_gid);

	ct = localtime(&buf.st_ctime);

	printf("%c%c%c%c%c%c%c%c%c%c %lu %s %s %ld %2d %2d %d:%d %s\n",
		type, ownrd, ownwt, ownet, grprd, grpwt, grpet, othrd, othwt, othet,
		buf.st_nlink, pa->pw_name, gp->gr_name, buf.st_size,
		ct->tm_mon + 1, ct->tm_mday, ct->tm_hour, ct->tm_min, argv[1]);
	return 0;
}