struct Table
{
    char domainName[200];
    char ip[10][16];
    int nip;
};

typedef struct Table Table;

int checkTable(Table lookup[50], char *hostname , int k)      //find is a string matches to domainName of list of Tables of size k
{
	for(int i = 0; i < k; i++)
	{
	    if(strcmp(hostname, lookup[i].domainName) == 0)
	    	  return i;
	}

	return -1;
}