//
// Created by Thimot on 03/11/2020.
//

#include <stdio.h>

#include "main.h"
#include "other/Bitmap/bitmap.h"
#include "characters_detection/characters_detection.h"





void test1()
{
	printf("wesh la street du 92! damsdams bangbang");
}















int	isEqual(char* s1, char *s2)
{
	int i = 0;
	int res = 1;
	while (s1[i])
	{
		char c  = s1[i];
		if (s2[i])
		{
			if (c != s2[i])
				res = 0;

		}else
		{
			res = 0;
		}
		i++;
	}
	if (s2[i])
		res = 0;
	return res;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Error Syntax\nSyntax: %s {str}\n", argv[0]);
		return 1;
	}

	if (isEqual(argv[1], "test1"))
		test1();
	else if(isEqual(argv[1], "test2"))
		test1();
	else
	{
		printf("Test not Found: %s\n", argv[1]);
		return 1;
	}
	return 0;
}
