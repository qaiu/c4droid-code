

#ifndef AES_H
#define AES_H

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

char __str16[16];
uint8_t __b64_table[128] = {};
char _0_key[128]="+QQ736226400";
char *tempkey;
char __key[33];


void getMD5(char *source, char *out);


void initEncoder()
{
	int k=0;
	for (int i = 0x41; i >= 0x21; i--)
		__b64_table[k++] = i;
		
	for (int i = 0x44; i <= 0x60; i++)
		__b64_table[k++] = i;
	__b64_table[k++] = 0x7b;
	__b64_table[k++] = 0x7d;

	__b64_table[57] = 0x64;
	__b64_table[31] = 0x69;
	
	k=0;
	for (int i = 0x7a; i >= 0x6b; i--)
		__str16[k++] = i;
		
	memset(__key,0,18);
	char temp[33];
	getMD5(_0_key,temp);
	strncpy(__key,temp,15);
}


char* b64_encode (const unsigned char *src, size_t len) {
  int i = 0;
  int j = 0;
  char *enc = NULL;
  size_t size = 0;
  unsigned char buf[4];
  unsigned char tmp[3];

  
  enc = (char *) malloc(0);
  if (NULL == enc) { return NULL; }

  
  while (len--) {
    
    tmp[i++] = *(src++);

    
    if (3 == i) {
      buf[0] = (tmp[0] & 0xfc) >> 2;
      buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
      buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
      buf[3] = tmp[2] & 0x3f;

      enc = (char *) realloc(enc, size + 4);
      for (i = 0; i < 4; ++i) {
        enc[size++] = __b64_table[buf[i]];
      }

      i = 0;
    }
  }

  
  if (i > 0) {
    
    for (j = i; j < 3; ++j) {
      tmp[j] = '\0';
    }

    
    buf[0] = (tmp[0] & 0xfc) >> 2;
    buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
    buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
    buf[3] = tmp[2] & 0x3f;

    
    for (j = 0; (j < i + 1); ++j) {
      enc = (char *) realloc(enc, size + 1);
      enc[size++] = __b64_table[buf[j]];
    }

    
    
    while ((i++ < 3)) {
      enc = (char *) realloc(enc, size + 1);
      enc[size++] = 0x20;
    }
  }

  
  enc = (char *) realloc(enc, size + 1);
  enc[size] = '\0';

  return enc;
}


unsigned char* b64_decode_ex(const char *src, size_t len, size_t *decsize) {
  int i = 0;
  int j = 0;
  int l = 0;
  size_t size = 0;
  unsigned char *dec = NULL;
  unsigned char buf[3];
  unsigned char tmp[4];

  
  dec = (unsigned char *) malloc(0);
  if (NULL == dec) { return NULL; }

  
  while (len--) {
    
    if (0x20 == src[j]) { break; }
    

    
    tmp[i++] = src[j++];

    
    if (4 == i) {
      
      for (i = 0; i < 4; ++i) {
        
        for (l = 0; l < 64; ++l) {
          if (tmp[i] == __b64_table[l]) {
            tmp[i] = l;
            break;
          }
        }
      }

      
      buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
      buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
      buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

      
      dec = (unsigned char *) realloc(dec, size + 3);
      for (i = 0; i < 3; ++i) {
        dec[size++] = buf[i];
      }

      
      i = 0;
    }
  }

  
  if (i > 0) {
    
    for (j = i; j < 4; ++j) {
      tmp[j] = '\0';
    }

    
    for (j = 0; j < 4; ++j) {
      
      for (l = 0; l < 64; ++l) {
        if (tmp[j] == __b64_table[l]) {
          tmp[j] = l;
          break;
        }
      }
    }

    
    buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
    buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
    buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

    
    dec = (unsigned char *) realloc(dec, size + (i - 1));
    for (j = 0; (j < i - 1); ++j) {
      dec[size++] = buf[j];
    }
  }

  
  dec = (unsigned char *) realloc(dec, size + 1);
  dec[size] = '\0';

  
  if (decsize != NULL) *decsize = size;

  return dec;
}

unsigned char *
b64_decode(const char *src, size_t len) {
  return b64_decode_ex(src, len, NULL);
}




#define Nk 4
#define Nb 4
#define Nr 10

#define KEYLEN 16

#ifndef MULTIPLY_AS_A_FUNCTION
#define MULTIPLY_AS_A_FUNCTION 0
#endif

typedef uint8_t state_t[4][4];
static state_t* state;


static uint8_t RoundKey[176];


static const uint8_t* Key;
static const unsigned  char HEX[16]={0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
static const uint8_t sbox[256] =   {
        
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t rsbox[256] ={ 
          0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
          0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
          0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
          0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
          0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
          0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
          0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
          0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
          0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
          0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
          0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
          0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
          0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
          0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
          0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
          0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };


static const uint8_t Rcon[255] = {
        0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
        0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
        0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
        0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
        0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
        0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
        0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
        0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
        0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
        0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
        0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
        0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
        0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
        0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
        0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
        0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };





static uint8_t getSBoxValue(uint8_t num)
{
  return sbox[num];
}

static uint8_t getSBoxInvert(uint8_t num)
{
  return rsbox[num];
}


static void KeyExpansion(void)
{
  uint32_t i, j, k;
  uint8_t tempa[4]; 

  
  for(i = 0; i < Nk; ++i)
  {
    RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
    RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
    RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
    RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
  }

  
  for(; (i < (Nb * (Nr + 1))); ++i)
  {
    for(j = 0; j < 4; ++j)
    {
      tempa[j]=RoundKey[(i-1) * 4 + j];
    }
    if (i % Nk == 0)
    {
        k = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = k;
        
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
        
      tempa[0] =  tempa[0] ^ Rcon[i/Nk];
    }
    else if (Nk > 6 && i % Nk == 4)
    {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
    }
    RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
    RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
    RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
    RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
  }
}

static void AddRoundKey(uint8_t round)
{
  uint8_t i,j;
  for(i=0;i<4;++i)
  {
    for(j = 0; j < 4; ++j)
    {
      (*state)[i][j] ^= RoundKey[round * Nb * 4 + i * Nb + j];
    }
  }
}

static void SubBytes(void)
{
  uint8_t i, j;
  for(i = 0; i < 4; ++i)
  {
    for(j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

static void ShiftRows(void)
{
  uint8_t temp;

  
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp       = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  
  temp       = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}


static void MixColumns(void)
{
  uint8_t i;
  uint8_t Tmp,Tm,t;
  for(i = 0; i < 4; ++i)
  {
    t   = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
    Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][3] ^ t ;        Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
  }
}

#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \


static void InvMixColumns(void)
{
  int i;
  uint8_t a,b,c,d;
  for(i=0;i<4;++i)
  {
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
  }
}

static void InvSubBytes(void)
{
  uint8_t i,j;
  for(i=0;i<4;++i)
  {
    for(j=0;j<4;++j)
    {
      (*state)[j][i] = getSBoxInvert((*state)[j][i]);
    }
  }
}

static void InvShiftRows(void)
{
  uint8_t temp;

  
  temp=(*state)[3][1];
  (*state)[3][1]=(*state)[2][1];
  (*state)[2][1]=(*state)[1][1];
  (*state)[1][1]=(*state)[0][1];
  (*state)[0][1]=temp;

  
  temp=(*state)[0][2];
  (*state)[0][2]=(*state)[2][2];
  (*state)[2][2]=temp;

  temp=(*state)[1][2];
  (*state)[1][2]=(*state)[3][2];
  (*state)[3][2]=temp;

  
  temp=(*state)[0][3];
  (*state)[0][3]=(*state)[1][3];
  (*state)[1][3]=(*state)[2][3];
  (*state)[2][3]=(*state)[3][3];
  (*state)[3][3]=temp;
}



static void Cipher(void)
{
  uint8_t round = 0;
  
  AddRoundKey(0);

  for(round = 1; round < Nr; ++round)
  {
    SubBytes();
    ShiftRows();
    MixColumns();
    AddRoundKey(round);
  }
  SubBytes();
  ShiftRows();
  AddRoundKey(Nr);
}

static void InvCipher(void)
{
  uint8_t round=0;

  
  AddRoundKey(Nr);

  for(round=Nr-1;round>0;round--)
  {
    InvShiftRows();
    InvSubBytes();
    AddRoundKey(round);
    InvMixColumns();
  }

  InvShiftRows();
  InvSubBytes();
  AddRoundKey(0);
}

static void BlockCopy(uint8_t* output, uint8_t* input)
{
  uint8_t i;
  for (i=0;i<KEYLEN;++i)
  {
    output[i] = input[i];
  }
}


void AES128_ECB_encrypt(uint8_t* input, const uint8_t* key, uint8_t* output)
{
  
  BlockCopy(output, input);
  state = (state_t*)output;

  Key = key;
  KeyExpansion();

  
  Cipher();
}

void AES128_ECB_decrypt(uint8_t* input, const uint8_t* key, uint8_t *output)
{
  
  BlockCopy(output, input);
  state = (state_t*)output;

  
  Key = key;
  KeyExpansion();

  InvCipher();
}



char* AES_128_ECB_PKCS5Padding_Encrypt(const char *in, const uint8_t *key,size_t inLength)
{
  int remainder = inLength % 16;
  uint8_t *paddingInput;
  int paddingInputLengt=0;
  int group = inLength / 16;
  int size = 16 * (group + 1);
  paddingInput=(uint8_t*)malloc(size);
  paddingInputLengt=size;

  int dif = size - inLength;
  int i;
  for (i = 0; i < size; i++) {
    if (i < inLength) {
      paddingInput[i] = in[i];
    } else {
      if (remainder == 0) {
        
        paddingInput[i] = HEX[0];
      } else {	
        paddingInput[i] = HEX[dif];
      }
    }
  }

  int count=paddingInputLengt / 16;
  
  uint8_t * out=(uint8_t*)malloc(paddingInputLengt);
  for ( i = 0; i < count; ++i) {
    AES128_ECB_encrypt(paddingInput+i*16, key, out+i*16);
  }
  char * base64En=b64_encode(out,paddingInputLengt);
  
  free(paddingInput);
  free(out);
  return base64En;
}



 
char* AES_128_ECB_PKCS5Padding_Decrypt(const char *in, const uint8_t* key,size_t *outSize)
{
  size_t inputLength = 0;
  uint8_t *inputDesBase64 = b64_decode_ex(in,strlen(in),&inputLength);
  inputLength+=1;
  
  uint8_t *out = (uint8_t*)calloc(1,inputLength);
  size_t count=inputLength/16;
  if (count<=0)
  {
    count=1;
  }
  size_t i;
  for ( i = 0; i < count; ++i) {
    AES128_ECB_decrypt(inputDesBase64+i*16,key,out+i*16);
  }
  
  
  int index = inputLength - out[inputLength - 2]-1;
  
  free(inputDesBase64);
  if (outSize != NULL)
  {
 	 *outSize = index;
  }
  
  if (index == 0) 
  {
  	free(out);
      return (char*) calloc(0,0);
  }
  else
  {
  	uint8_t *temp = out;
  	out = (uint8_t*)calloc(1,index);
  	memcpy(out,temp,index);
  	free(out);
  	return (char*)out;
  }
}



int getEncryptedDataSize(int originalDataSize)
{
	return ((originalDataSize/16+1)*16/3+1)*4;
}




 
#define __SHIFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define __FUNF(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define __FUNG(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define __FUNH(x, y, z) ((x) ^ (y) ^ (z))
#define __FUNI(x, y, z) ((y) ^ ((x) | (~z)))
#define __NUMA 0x67452301
#define __NUMB 0xefcdab89
#define __NUMC 0x98badcfe
#define __NUMD 0x10325476

unsigned int strlength;

unsigned int atemp;
unsigned int btemp;
unsigned int ctemp;
unsigned int dtemp;

const unsigned int md5_k[]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
        0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
        0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
        0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
        0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
        0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
        0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
        0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
        0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
        0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
        0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
        0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};

const unsigned int md5_s[]={
	    7,12,17,22,7,12,17,22,7,12,17,22,7,
        12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
        15,21,6,10,15,21,6,10,15,21,6,10,15,21};
void mainLoop(unsigned int M[])
{
    unsigned int f,g;
    unsigned int a=atemp;
    unsigned int b=btemp;
    unsigned int c=ctemp;
    unsigned int d=dtemp;
    for (unsigned int i = 0; i < 64; i++)
    {
        if(i<16){
            f=__FUNF(b,c,d);
            g=i;
        }else if (i<32)
        {
            f=__FUNG(b,c,d);
            g=(5*i+1)%16;
        }else if(i<48){
            f=__FUNH(b,c,d);
            g=(3*i+5)%16;
        }else{
            f=__FUNI(b,c,d);
            g=(7*i)%16;
        }
        unsigned int tmp=d;
        d=c;
        c=b;
        b=b+__SHIFT((a+f+md5_k[i]+M[g]),md5_s[i]);
        a=tmp;
    }
    atemp=a+atemp;
    btemp=b+btemp;
    ctemp=c+ctemp;
    dtemp=d+dtemp;
}


unsigned int* md5_add(char* str)
{
	size_t len = strlen(str);
    unsigned int num = ((len+8)/64)+1;
    unsigned int *strByte = (unsigned int*)calloc(1,num*16);    
    strlength=num*16;
    for (unsigned int i = 0; i < num*16; i++)
        strByte[i]=0;
    for (unsigned int i=0; i <len; i++)
    {
        strByte[i>>2]|=(str[i])<<((i%4)*8);
    }
    strByte[len>>2]|=0x80<<((len%4)*8);
    

    strByte[num*16-2]=len*8;
    return strByte;
}

void changeHex(int a, char* str)
{
    int b;
    memset(str,0,9);
    for(int i=0;i<4;i++)
    {
    	char str1[3] = {0};
        b=((a>>i*8)%(1<<8))&0xff;   
        str1[1]=__str16[b%16];
        b=b/16;
        str1[0]=__str16[b%16];
        strcat(str,str1);
    }
}

void getMD5(char *source, char *out)
{
    atemp=__NUMA;    
    btemp=__NUMB;
    ctemp=__NUMC;
    dtemp=__NUMD;
    unsigned int *strByte=md5_add(source);
    
    for (unsigned int i=0;i<strlength/16;i++)
    {
        unsigned int num[16];
        for(unsigned int j=0;j<16;j++)
            num[j]=strByte[i*16+j];
        mainLoop(num);
    }
  
    char temp[9] = {0};
    changeHex(atemp,temp);    
    
    strcpy(out,temp);
    changeHex(btemp,temp);
    strcat(out,temp);
    changeHex(ctemp,temp);
    strcat(out,temp);   
    changeHex(dtemp,temp);
    strcat(out,temp);    
    free(strByte);
}

#include <ctype.h>
#define ISOCT(x) ((x)>=0x30&&(x)<=0x37)
char* escapeChar(char* str)
{
	if (str==NULL) return NULL;	
    int len = strlen(str);
    const char *key[] = {"\\a","\\b","\\f","\\n","\\r","\\t","\\v","\\\\","\\'","\\\"","\\?","\\0","\\e","\\033","\\x1b"};
            int value[] = {0x07, 0x08, 0x0c, 0x0a, 0x0d, 0x09, 0x0b, 0x5c,  0x27, 0x22,  0x3f, 0x00, 0x1b,  0x1b,  0x1b };
    FILE *fp = fopen("002","r");
    char buff[len+3];
    memset(buff,0,len+3);
    for (size_t i = 0, j = 0; i < len; i++)
    {
        if (str[i] != '\\')
        {
            buff[j++] = str[i];
        }
        else
        {
            if (strncmp(str+i,"\\x",2) == 0)
            {
                char hex[3] = {*(str+i+2), *(str+i+3)};
                if (isxdigit(hex[0])&&isxdigit(hex[1]))
                {
                    buff[j++] = (char)strtol(hex,NULL,16);
                    i += 3;
                    continue;
                }
                else  if (isxdigit(hex[0]))
                {
                    buff[j++] = (char)strtol(hex,NULL,16);
                    i += 2;
                    continue;
                }
                else
                    continue;
            }
            char oct[4] = {*(str+i+1), *(str+i+2), *(str+i+3)};
            if (ISOCT(oct[0])&&ISOCT(oct[1])&&ISOCT(oct[2]))
            {
                buff[j++] = (char)strtol(oct,NULL,8);
                i += 3;
                continue;
            }

            for (size_t k = 0; k < sizeof(key)/sizeof(char *); k++)
            {
                if (strncmp(str+i, key[k], strlen(key[k])) == 0)
                {
                    buff[j++] = value[k];
                    i += strlen(key[k]) - 1;
                    break;
                }
            }
        }
               
    }
    strcpy(str,buff);
    return str;
}


char* encrypt(char* str0)
{
	return AES_128_ECB_PKCS5Padding_Encrypt(str0,(const uint8_t*)__key,strlen(str0));
}

char* decrypt(char* str)
{
	return AES_128_ECB_PKCS5Padding_Decrypt(str,(const uint8_t*)__key,NULL);
}

void _____(char *_)
{
	char *__ = escapeChar(decrypt(_));
	strcpy(_,__);
}

#endif

/*请不要修改此文件*/