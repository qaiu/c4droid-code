/*
* Description:
* This file will exercise the SHA-1 code performing the three
* tests documented in FIPS PUB 180-1 plus one which calls
* SHA1Input with an exact multiple of 512 bits, plus a few
* error test checks.
*
* Portability Issues:
* None.
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEX_BASE 0x10

/*
* If you do not have the ISO standard stdint.h header file, then you
* must typdef the following:
* name meaning
* uint32_t unsigned 32 bit integer
* uint8_t unsigned 8 bit integer (i.e., unsigned char)
* int_least16_t integer of >= 16 bits
*
*/

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef short int int_least16_t;

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,         /* Null pointer parameter */
    shaInputTooLong, /* input data too long */
    shaStateError    /* called Input after Result */
};
#endif
#define SHA1HashSize 20
/*
* This structure will hold context information for the SHA-1
* hashing operation
*/
typedef struct SHA1Context
{
    uint32_t Intermediate_Hash[SHA1HashSize/4]; /* Message Digest */
    uint32_t Length_Low; /* Message length in bits */
    uint32_t Length_High; /* Message length in bits */
    /* Index into message block array */
    int_least16_t Message_Block_Index;
    uint8_t Message_Block[64]; /* 512-bit message blocks */
    int Computed; /* Is the digest computed? */
    int Corrupted; /* Is the message digest corrupted? */
} SHA1Context;


/*
* Function Prototypes
*/

int SHA1Reset( SHA1Context *);
int SHA1Input( SHA1Context *,const uint8_t *,unsigned int);
int SHA1Result( SHA1Context *,uint8_t Message_Digest[SHA1HashSize]);



/*
* sha1.c
*
* Description:
* This file implements the Secure Hashing Algorithm 1 as
* defined in FIPS PUB 180-1 published April 17, 1995.
*
* The SHA-1, produces a 160-bit message digest for a given
* data stream. It should take about 2**n steps to find a
* message with the same digest as a given message and
* 2**(n/2) to find any two messages with the same digest,
* when n is the digest size in bits. Therefore, this
* algorithm can serve as a means of providing a
* "fingerprint" for a message.
*
* Portability Issues:
* SHA-1 is defined in terms of 32-bit "words". This code
* uses <stdint.h> (included via "sha1.h" to define 32 and 8
* bit unsigned integer types. If your C compiler does not
* support 32 bit unsigned integers, this code is not
* appropriate.
*
* Caveats:
* SHA-1 is designed to work with messages less than 2^64 bits
* long. Although SHA-1 allows a message digest to be generated
* for messages of any number of bits less than 2^64, this
* implementation only works with messages with a length that is
* a multiple of the size of an 8-bit character.
*
*/


/*
* Define the SHA1 circular left shift macro
*/
#define SHA1CircularShift(bits,word) \
    (((word) << (bits)) | ((word) >> (32-(bits))))
/* Local Function Prototyptes */
void SHA1PadMessage(SHA1Context *);
void SHA1ProcessMessageBlock(SHA1Context *);
/*
* SHA1Reset
*
* Description:
* This function will initialize the SHA1Context in preparation
* for computing a new SHA1 message digest.
*
* Parameters:
* context: [in/out]
* The context to reset.
*
* Returns:
* sha Error Code.
*
*/
int SHA1Reset(SHA1Context *context)//初始化状态
{
    if (!context)
    {
        return shaNull;
    }
    context->Length_Low = 0;
    context->Length_High = 0;
    context->Message_Block_Index = 0;
    context->Intermediate_Hash[0] = 0x67452301;//取得的HASH结果（中间数据）
    context->Intermediate_Hash[1] = 0xEFCDAB89;
    context->Intermediate_Hash[2] = 0x98BADCFE;
    context->Intermediate_Hash[3] = 0x10325476;
    context->Intermediate_Hash[4] = 0xC3D2E1F0;
    context->Computed = 0;
    context->Corrupted = 0;
    return shaSuccess;
}


/*
* SHA1Result
*
* Description:
* This function will return the 160-bit message digest into the
* Message_Digest array provided by the caller.
* NOTE: The first octet of hash is stored in the 0th element,
* the last octet of hash in the 19th element.
*
* Parameters:
* context: [in/out]
* The context to use to calculate the SHA-1 hash.
* Message_Digest: [out]
* Where the digest is returned.
*
* Returns:
* sha Error Code.
*
*/
int SHA1Result( SHA1Context *context,uint8_t Message_Digest[SHA1HashSize])
{
    int i;
    if (!context || !Message_Digest)
    {
        return shaNull;
    }
    if (context->Corrupted)
    {
        return context->Corrupted;
    }
    if (!context->Computed)
    {
        SHA1PadMessage(context);
        for(i=0; i<64; ++i)
        {
            /* message may be sensitive, clear it out */
            context->Message_Block[i] = 0;
        }
        context->Length_Low = 0; /* and clear length */
        context->Length_High = 0;
        context->Computed = 1;
    }
    for(i = 0; i < SHA1HashSize; ++i)
    {
        Message_Digest[i] = context->Intermediate_Hash[i>>2]
        >> 8 * ( 3 - ( i & 0x03 ) );
    }
    return shaSuccess;
}


/*
* SHA1Input
*
* Description:
* This function accepts an array of octets as the next portion
* of the message.
*
* Parameters:
* context: [in/out]
* The SHA context to update
* message_array: [in]
* An array of characters representing the next portion of
* the message.
* length: [in]
* The length of the message in message_array
*
* Returns:
* sha Error Code.
*
*/

int SHA1Input( SHA1Context *context,const uint8_t *message_array,unsigned length)
{
    if (!length)
    {
        return shaSuccess;
    }
    if (!context || !message_array)
    {
        return shaNull;
    }
    if (context->Computed)
    {
        context->Corrupted = shaStateError;
        return shaStateError;
    }
    if (context->Corrupted)
    {
        return context->Corrupted;
    }
    while(length-- && !context->Corrupted)
    {
        context->Message_Block[context->Message_Block_Index++] =
            (*message_array & 0xFF);
        context->Length_Low += 8;
        if (context->Length_Low == 0)
        {
            context->Length_High++;
            if (context->Length_High == 0)
            {
                /* Message is too long */
                context->Corrupted = 1;
            }
        }
        if (context->Message_Block_Index == 64)
        {
            SHA1ProcessMessageBlock(context);
        }
        message_array++;
    }
    return shaSuccess;
}

/*
* SHA1ProcessMessageBlock
*
* Description:
* This function will process the next 512 bits of the message
* stored in the Message_Block array.
*
* Parameters:
* None.
*
* Returns:
* Nothing.
*
* Comments:
* Many of the variable names in this code, especially the
* single character names, were used because those were the
* names used in the publication.
*
*/

void SHA1ProcessMessageBlock(SHA1Context *context)
{
    const uint32_t K[] = { /* Constants defined in SHA-1 */
        0x5A827999,
        0x6ED9EBA1,
        0x8F1BBCDC,
        0xCA62C1D6
    };
    int t; /* Loop counter */
    uint32_t temp; /* Temporary word value */
    uint32_t W[80]; /* Word sequence */
    uint32_t A, B, C, D, E; /* Word buffers */
    /*
    * Initialize the first 16 words in the array W
    */
    for(t = 0; t < 16; t++)
    {
        W[t] = context->Message_Block[t * 4] << 24;
        W[t] |= context->Message_Block[t * 4 + 1] << 16;
        W[t] |= context->Message_Block[t * 4 + 2] << 8;
        W[t] |= context->Message_Block[t * 4 + 3];
    }
    for(t = 16; t < 80; t++)
    {
        W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }
    A = context->Intermediate_Hash[0];
    B = context->Intermediate_Hash[1];
    C = context->Intermediate_Hash[2];
    D = context->Intermediate_Hash[3];
    E = context->Intermediate_Hash[4];
    for(t = 0; t < 20; t++)
    {
                temp = SHA1CircularShift(5,A) +
                        ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }
    for(t = 20; t < 40; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }
    for(t = 40; t < 60; t++)
    {
        temp = SHA1CircularShift(5,A) +
            ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }
    for(t = 60; t < 80; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }
    context->Intermediate_Hash[0] += A;
    context->Intermediate_Hash[1] += B;
    context->Intermediate_Hash[2] += C;
    context->Intermediate_Hash[3] += D;
    context->Intermediate_Hash[4] += E;
    context->Message_Block_Index = 0;
}


/*
* SHA1PadMessage
*
* Description:
* According to the standard, the message must be padded to an even
* 512 bits. The first padding bit must be a ’1’. The last 64
* bits represent the length of the original message. All bits in
* between should be 0. This function will pad the message
* according to those rules by filling the Message_Block array
* accordingly. It will also call the ProcessMessageBlock function
* provided appropriately. When it returns, it can be assumed that
* the message digest has been computed.
*
* Parameters:
* context: [in/out]
* The context to pad
* ProcessMessageBlock: [in]
* The appropriate SHA*ProcessMessageBlock function
* Returns:
* Nothing.
*
*/

void SHA1PadMessage(SHA1Context *context)
{
    /*
    * Check to see if the current message block is too small to hold
    * the initial padding bits and length. If so, we will pad the
    * block, process it, and then continue padding into a second
    * block.
    */
    if (context->Message_Block_Index > 55)
    {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        while(context->Message_Block_Index < 64)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
        SHA1ProcessMessageBlock(context);
        while(context->Message_Block_Index < 56)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }
    else
    {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        while(context->Message_Block_Index < 56)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }

    /*
    * Store the message length as the last 8 octets
    */
    context->Message_Block[56] = context->Length_High >> 24;
    context->Message_Block[57] = context->Length_High >> 16;
    context->Message_Block[58] = context->Length_High >> 8;
    context->Message_Block[59] = context->Length_High;
    context->Message_Block[60] = context->Length_Low >> 24;
    context->Message_Block[61] = context->Length_Low >> 16;
    context->Message_Block[62] = context->Length_Low >> 8;
    context->Message_Block[63] = context->Length_Low;
    SHA1ProcessMessageBlock(context);
}


/*
* 进行sha1算法运算
* @input 输入的字符串
* @output 输出的字符串
* @size 输入字符串长度
* return  0 成功，-1 失败
*/
int sha1(const char *input, uint8_t output[20], unsigned size)
{
    int i, err;
    SHA1Context sha;

    err = SHA1Reset(&sha);
    err = SHA1Input(&sha,(const unsigned char *) input, size);
    if (err){
        printf("SHA1Input Error %d.\n", err);
        return -1;
    }
    err = SHA1Result(&sha, output);
    if (err){
        printf("SHA1Result Error %d, could not compute message digest.\n",err );
        return -1;
    } 
    return 0;
}

#ifdef HMAC_DEBUG
debug_out(unsigned char *label, unsigned char *data, int data_length)
{
	int i, j;
	int num_blocks;
	int block_remainder;
	num_blocks = data_length / 16;
	block_remainder = data_length % 16;

	printf("%s\n", label);

	for (i = 0; i < num_blocks; i++) {
		printf("\t");
		for (j = 0; j < 16; j++) {
			printf("%02x ", data[j + (i * 16)]);
		}
		printf("\n");
	}

	if (block_remainder > 0) {
		printf("\t");
		for (j = 0; j < block_remainder; j++) {
			printf("%02x ", data[j + (num_blocks * 16)]);
		}
		printf("\n");
	}
}
#endif

#define MAX_MESSAGE_LENGTH 4096
/******************************************************/
/* hmac-sha1()                                        */
/* Performs the hmac-sha1 keyed secure hash algorithm */
/******************************************************/
 
void hmac_sha1(const char *key, int key_length,const char *data, int data_length, uint8_t digest[20])
{
    int b = 64; /* blocksize */
    uint8_t ipad = 0x36;
 
    uint8_t opad = 0x5c;
 
    char k0[64];
    char k0xorIpad[64];
    char step7data[64];
    char step5data[MAX_MESSAGE_LENGTH+128];
    char step8data[64+20];
    int i;
 
    for (i=0; i<64; i++)
    {
        k0[i] = 0x00;
    }
 
    if (key_length != b)    /* Step 1 */
    {
        /* Step 2 */
        if (key_length > b)      
        {
            sha1(key, digest, key_length);
            for (i=0;i<20;i++)
            {
                k0[i]=digest[i];
            }
        }
        else if (key_length < b)  /* Step 3 */
        {
            for (i=0; i<key_length; i++)
            {
                k0[i] = key[i];
            }
        }
    }
    else
    {
        for (i=0;i<b;i++)
        {
            k0[i] = key[i];
        }
    }
#ifdef HMAC_DEBUG
    debug_out("k0",k0,64);
#endif
    /* Step 4 */
    for (i=0; i<64; i++)
    {
        k0xorIpad[i] = k0[i] ^ ipad;
    }
#ifdef HMAC_DEBUG
    debug_out("k0 xor ipad",k0xorIpad,64);
#endif
    /* Step 5 */
    for (i=0; i<64; i++)
    {
        step5data[i] = k0xorIpad[i];
    }
    for (i=0;i<data_length;i++)
    {
        step5data[i+64] = data[i];
    }
#ifdef HMAC_DEBUG
    debug_out("(k0 xor ipad) || text",step5data,data_length+64);
#endif
 
    /* Step 6 */
    sha1(step5data, digest, data_length+b);
 
#ifdef HMAC_DEBUG
    debug_out("Hash((k0 xor ipad) || text)",digest,20);
#endif
 
    /* Step 7 */
    for (i=0; i<64; i++)
    {
        step7data[i] = k0[i] ^ opad;
    }
 
#ifdef HMAC_DEBUG
    debug_out("(k0 xor opad)",step7data,64);
#endif
 
    /* Step 8 */
    for (i=0;i<64;i++)
    {
        step8data[i] = step7data[i];
    }
    for (i=0;i<20;i++)
    {
        step8data[i+64] = digest[i];
    }
 
#ifdef HMAC_DEBUG
    debug_out("(k0 xor opad) || Hash((k0 xor ipad) || text)",step8data,20+64);
#endif
 
    /* Step 9 */
    sha1(step8data, digest, b+20);
 
#ifdef HMAC_DEBUG
    debug_out("HASH((k0 xor opad) || Hash((k0 xor ipad) || text))",digest,20);
#endif
}

char *str_reverse(char *s)
{
	if (s==NULL) return NULL;
	for (int i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		char c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
	return s;
}

char *decimalToHex(int decimalNumber, char *hexadecimalNumber)
{
	if(decimalNumber == 0) {
		hexadecimalNumber[0]=hexadecimalNumber[1]='0';
		return hexadecimalNumber;
	} else if(decimalNumber < 0) { 
        decimalNumber+=0x100;
    }
	int remainder, quotient;
	int i = 0, j, temp;

	quotient = decimalNumber;
	
	while (quotient != 0)
	{
		temp = quotient % HEX_BASE;
		if (temp < 0xa)
			temp = temp + '0';
		else
			temp = temp + 'a' - 0xa;
		hexadecimalNumber[i++] = temp;
		quotient = quotient / HEX_BASE;
	}
	
	//个位补零
	if (decimalNumber < HEX_BASE)
		hexadecimalNumber[i++] = '0';
	
	return str_reverse(hexadecimalNumber);
}

char *binToHex(const char *bin, char* dest,uint32_t length){
	for (int i=0;i<length;i++) {
        char temp[3] = {};
		strcat(dest, decimalToHex(bin[i],temp));
	}
	return dest;
}

/*
* hmac-sha1 封装
* 字符串转16进制输入
*/
char *hmac_sha1_str(const char*text_str,const char*key_str,char *digest){
    hmac_sha1(key_str, strlen(key_str), text_str, strlen(text_str), (uint8_t*)digest);
    char temp[20]={};
    memcpy(temp,digest,20);
    memset(digest,0,20);
	return binToHex(temp,digest,20);
}

char *sha1_str(const char*text_str,char *digest){
    sha1(text_str,(uint8_t*)digest,strlen(text_str));
    char temp[20]={};
    memcpy(temp,digest,20);
    memset(digest,0,20);
	return binToHex(temp,digest,20);
}


/*
int main()
{
	// char p[256]={};
	// hmac_sha1_str("1617744925;1617744985","a4IFxfO4YllVzOVPWO0Q3SbXE6j62h7H",p);
	// // puts("");
	// puts(p);
    // char out[20] = {};
    // sha1("123456",(uint8_t*)out,6);

    // char out2[20] = {};
	// printf("%s\n",out);

    // hmac_sha1("123",3,"1234",4,(uint8_t*)out2);
	// printf("%s\n",out2);
	// char p2[64]={};
	// sha1_str("123456",p2);
	// puts("");
	// puts(p2);
	
	// char p3[64]={};
    // hmac_sha1_str("1234556666666","阿打算大所大所大所多撒大多所撒大所大所多",p3);

	// puts(p3);
    // printf("%d\n",~((int8_t)-127)+1);
    // printf("%d\n",'1');
    // char* aaa="啊";
    // printf("%d\n",aaa[0]+0x100);
    return 0;
}
//*/
