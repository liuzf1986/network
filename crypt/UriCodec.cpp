#include "UriCodec.hpp"
#include <string.h>

static const char HEX2DEC[256] = 
{
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,
    
    /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

// Only alphanum is safe.
static const char SAFE[256] =
{
    /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
    /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,
    
    /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
    /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
    
    /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    
    /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

std::string UriCodec::decode(const std::string& sSrc) {
  // Note from RFC1630:  "Sequences which start with a percent sign
  // but are not followed by two hexadecimal characters (0-9, A-F) are reserved
  // for future extension"
    
  const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
  const int SRC_LEN = sSrc.length();
  const unsigned char * const SRC_END = pSrc + SRC_LEN;
  const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%' 

  char * const pStart = new char[SRC_LEN];
  char * pEnd = pStart;

  while (pSrc < SRC_LAST_DEC)
  {
    if (*pSrc == '%')
    {
      char dec1, dec2;
      if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
          && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
      {
        *pEnd++ = (dec1 << 4) + dec2;
        pSrc += 3;
        continue;
      }
    }

    *pEnd++ = *pSrc++;
  }

    // the last 2- chars
    while (pSrc < SRC_END)
        *pEnd++ = *pSrc++;

    std::string sResult(pStart, pEnd);
    delete [] pStart;
	return sResult;
}

std::string UriCodec::encode(const std::string& sSrc) {
  const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
  const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
  const int SRC_LEN = sSrc.length();
  unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
  unsigned char * pEnd = pStart;
  const unsigned char * const SRC_END = pSrc + SRC_LEN;

  for (; pSrc < SRC_END; ++pSrc)
  {
    if (SAFE[*pSrc]) 
      *pEnd++ = *pSrc;
    else
    {
      // escape this char
      *pEnd++ = '%';
      *pEnd++ = DEC2HEX[*pSrc >> 4];
      *pEnd++ = DEC2HEX[*pSrc & 0x0F];
    }
  }

  std::string sResult((char *)pStart, (char *)pEnd);
  delete [] pStart;
  return sResult;
}

size_t UriCodec::getEncodeBufferSize(const char* srcptr, size_t strlen) {
  return (strlen * 3) + 1;
}

size_t UriCodec::encode(char* dstptr, size_t dstlen,const char* srcptr, size_t strlen) {
  const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
  const char* pSrcIndex = srcptr;
  char* pDstIndex = dstptr;
  const char* const pEndEnc = srcptr + strlen;

  if(dstlen < ((strlen * 3) + 1)) {
    return false;
  }

  while(pSrcIndex < pEndEnc) {
    if(SAFE[*pSrcIndex]) {
      *pDstIndex++ = *pSrcIndex;
    } else {
      // escape this char
      *pDstIndex++ = '%';
      *pDstIndex++ = DEC2HEX[*pSrcIndex >> 4];
      *pDstIndex++ = DEC2HEX[*pSrcIndex & 0x0F];
    }

    ++ pSrcIndex;
  }
  *pDstIndex++ = '\0';

  return pDstIndex - dstptr;
}

size_t UriCodec::getDecodeBufferSize(const char *srcptr, size_t strlen) {
  return strlen + 1;
}

size_t UriCodec::decode(char* dstptr, size_t dstlen, const char* srcptr, size_t strlen) {
  const char* pSrcIndex = srcptr;
  char* pDstIndex = dstptr;
  const char* const pSrcEnd = srcptr + strlen;
  const char* const pLastDec = pSrcEnd - 2;
  if(dstlen < (strlen + 1)) {
    return 0;
  }

  while(pSrcIndex < pLastDec) {
    if('%' == *pSrcIndex) {
      char dec1, dec2;
      if(-1 != (dec1 = HEX2DEC[*(pSrcIndex + 1)])
         && -1 != (dec2 = HEX2DEC[*(pSrcIndex + 2)])) {
        *pDstIndex++ = (dec1 << 4) + dec2;
        pSrcIndex += 3;
      }
    }

    *pDstIndex++ = *pSrcIndex++;
  }

  while(pSrcIndex < pSrcEnd) {
    *pDstIndex++ = *pSrcIndex++;
  }
  
  *pDstIndex++ = '\0';
  return pDstIndex - dstptr;
}

#include <iostream>
int main(int argc, char *argv[])
{
  char temp[200];
  char temp2[400];
  
  std::string orig = "/hello/coming=workd/";
  size_t enclen;


  const char* enced = "http://zh.wikipedia.org/zh-cn/%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81";
  UriCodec::decode(temp, sizeof(temp), enced, strlen(enced));

  std::cout << temp << std::endl;

#if 1
  for(int i = 0; i < 10000000; i ++) {
    enclen = UriCodec::encode(temp, sizeof(temp), orig.data(), orig.length());
    UriCodec::decode(temp2, sizeof(temp2), temp, enclen - 1);
  }

  std::cout << temp << "   " << enclen << std::endl;
  std::cout << temp2 << std::endl;
#else
  std::string enc;
  std::string dec;
  
  for(int i = 0; i < 10000000; i ++) {
    enc = UriCodec::encode(orig);
    dec = UriCodec::decode(enc);
  }
  
  std::cout << enc << "   " << enclen << std::endl;
  std::cout << dec << std::endl;
#endif
  

  return 0;
}

















