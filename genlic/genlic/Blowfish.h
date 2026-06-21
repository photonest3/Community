///////////////////////////////////////////////////////////////////////
/// @file Blowfish.h
/// @brief Blowfish 加密算法类头文件 (genlic 模块)
/// @details 定义 Blowfish 分组加密算法的类和接口,
///          用于软件授权码的加密和验证
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 genlic 模块使用的加密算法头文件
///////////////////////////////////////////////////////////////////////

/* Blowfish Encryption Class - Header File */
#define ___BLOWFISH_H___

#define NUM_SUBKEYS   18
#define NUM_S_BOXES   4
#define NUM_ENTRIES   256

#define MAX_STRING   256
#define MAX_PASSWD   56   /* 56 bytes = 448 bits */



/*   Define LITTLE_ENDIAN (Intel, DCBA) or BIG_ENDIAN (Motorola, ABCD) or VAX (BADC).
      If one of these is defined in another file, you can comment out the following line. */
#define LITTLE_ENDIAN


#ifdef BIG_ENDIAN
   struct BFWordByte
   {
      unsigned int zero:8;
      unsigned int one:8;
      unsigned int two:8;
      unsigned int three:8;
   };
#endif /* endif BIG_ENDIAN (Intel, DCBA) defined */

#ifdef LITTLE_ENDIAN
   struct BFWordByte
   {
      unsigned int three:8;
      unsigned int two:8;
      unsigned int one:8;
      unsigned int zero:8;
   };
#endif /* endif LITTLE_ENDIAN (Motorola, ABCD) defined */

#ifdef VAX
   struct BFWordByte
   {
      unsigned int one:8;
      unsigned int zero:8;
      unsigned int three:8;
      unsigned int two:8;
   };
#endif /* endif VAX (BADC) defined */

union BFWord
{
   unsigned int word;
   BFWordByte byte;
};

struct BFDWord
{
   BFWord word0;
   BFWord word1;
};


class CBlowfish
{
public:
   CBlowfish();
   ~CBlowfish();

   void Reset();

   void SetPassword(char *Passwd);

   void Encrypt(void *Ptr, unsigned long nBytes);
   void Decrypt(void *Ptr, unsigned long nBytes);

private:
   unsigned int PA[NUM_SUBKEYS];
   unsigned int SB[NUM_S_BOXES][NUM_ENTRIES];

   void GenerateSubkeys(char *Passwd);

   inline void BF_En(BFWord *x1, BFWord *x2);
   inline void BF_De(BFWord *x1, BFWord *x2);
};

#endif /* ___BLOWFISH_H___ */
