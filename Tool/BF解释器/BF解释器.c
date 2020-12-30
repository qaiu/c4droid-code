// BF解释器

#include<stdio.h>
#include<malloc.h>
#include<string.h>
char br[] = { };

int main()
{

	printf("Welcome brainfuck interpreter");
	while (1)
	{
		printf("\n> ");
		scanf("%s", br);
		char *eip = br;
		char *mem = (char *)malloc(1000);
		char *ptr = mem;
		int isright = 1;
		while (*eip != 0)
		{
			if (*eip == '[')
			{
				while (*eip != 0 && *eip != ']')
					++eip;
				if (*eip == 0)
					isright = 0;
			}
			else if (*eip == ']')
				isright = 0;

			if (!isright)
			{
				printf("The brainfuck code has some wrong!\n");
				*eip = 0;
			}
			++eip;
		}
		memset(ptr, 0, 100);
		eip = br;
		do
		{
			switch (*eip)
			{
			case '>':
				++ptr;
				break;
			case '<':
				--ptr;
				break;
			case '+':
				++*ptr;
				break;
			case '-':
				--*ptr;
				break;
			case '.':
				putchar(*ptr);
				break;
			case ',':
				*ptr = getchar();
				break;
			case '[':
				if (*ptr == 0)
				{
					do
					{
						++eip;
					}
					while (*eip != ']');
				}
				break;
			case ']':
				if (*ptr != 0)
				{
					do
					{
						--eip;
					}
					while (*eip != '[');
				}
				break;
			}
			if ((ptr - mem) > 100)
			{
				printf("Stack overflow!\n");
				break;
			}
			++eip;
		}
		while (*eip != 0);

		free(mem);
	}
	return 0;
}