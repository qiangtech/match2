/*
 * Copyright 1995-2018 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef HEADER_CRYPTO_H
# define HEADER_CRYPTO_H

# include <stdlib.h>
# include <time.h>

# include <openssl/e_os2.h>

# ifndef OPENSSL_NO_STDIO
#  include <stdio.h>
# endif

# include <openssl/safestack.h>
# include <openssl/opensslv.h>
# include <openssl/ossl_typ.h>
# include <openssl/opensslconf.h>
# include <openssl/cryptoerr.h>

# ifdef CHARSET_EBCDIC
#  include <openssl/ebcdic.h>
# endif

/*
 * Resolve problems on some operating systems with symbol names that clash
 * one way or another
 */
# include <openssl/symhacks.h>

# if OPENSSL_API_COMPAT < 0x10100000L
#  include <openssl/opensslv.h>
# endif

#ifdef  __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------*/
/*  Nuvoton Cryptographic Accerlator                         */
/*-----------------------------------------------------------*/
#define USE_NUVOTON_AES
#define USE_NUVOTON_SHA
#define USE_NUVOTON_ECC
#define USE_NUVOTON_RSA

#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#define NVT_AES          "/dev/nuvoton-aes"
#define NVT_SHA          "/dev/nuvoton-sha"
#define NVT_ECC          "/dev/nuvoton-ecc"
#define NVT_RSA          "/dev/nuvoton-rsa"

extern int fd_aes;
extern int fd_sha;
extern int fd_ecc;
extern int fd_rsa;
extern sem_t  aes_semaphore;

extern unsigned char  *aes_map_inbuff, *aes_map_outbuff;
extern unsigned long  aes_buff_size;

extern int Nuvoton_Init_AES(void);
extern void Nuvoton_Deinit_AES(void);
extern int Nuvoton_Init_SHA(void);
extern void Nuvoton_Deinit_SHA(void);
extern int Nuvoton_Init_ECC(void);
extern void Nuvoton_Deinit_ECC(void);
extern int Nuvoton_Init_RSA(void);
extern void Nuvoton_Deinit_RSA(void);


#define NVT_AES_KEYSZ_128			(0x00000000)			/*!<AES 128 bits key */
#define NVT_AES_KEYSZ_192			(0x00000004)			/*!<AES 192 bits key */
#define NVT_AES_KEYSZ_256			(0x00000008)			/*!<AES 256 bits key */
#define NVT_AES_ECB_MODE			(0x00000000)			/*!<AES ECB mode */
#define NVT_AES_CBC_MODE			(0x00000100)			/*!<AES CBC mode */
#define NVT_AES_CFB_MODE			(0x00000200)			/*!<AES CFB mode */
#define NVT_AES_OFB_MODE			(0x00000300)			/*!<AES OFB mode */
#define NVT_AES_CTR_MODE			(0x00000400)			/*!<AES CTR mode */
#define NVT_AES_CBCCS1_MODE			(0x00001000)			/*!<AES CBC CS1 mode */
#define NVT_AES_CBCCS2_MODE			(0x00001100)			/*!<AES CBC CS2 mode */
#define NVT_AES_CBCCS3_MODE			(0x00001200)			/*!<AES CBC CS3 mode */
#define NVT_AES_ENCRYPT				(0x00010000)			/*!<AES engine execute encryption */
#define NVT_AES_DECRYPT				(0x00000000)			/*!<AES engine execute decryption */
#define NVT_AES_OUTSWAP				(0x00400000)			/*!<AES engine output data transform */
#define NVT_AES_INSWAP				(0x00800000)			/*!<AES engine input data transform */

#define NVT_SHA1					(0x00000000)			/*!<SHA1 */
#define NVT_SHA224					(0x00000500)			/*!<SHA224 */
#define NVT_SHA256					(0x00000400)			/*!<SHA256 */
#define NVT_SHA384					(0x00000700)			/*!<SHA384 */
#define NVT_SHA512					(0x00000600)			/*!<SHA512 */
#define NVT_SHA_OUTSWAP				(0x00400000)			/*!<HMAC engine output data transform */
#define NVT_SHA_INSWAP				(0x00800000)			/*!<HMAC engine input data transform */

#define CRYPTO_IOC_MAGIC		'C'

#define AES_IOC_SET_MODE		_IOW(CRYPTO_IOC_MAGIC,  1, unsigned long)
#define AES_IOC_SET_LEN			_IOW(CRYPTO_IOC_MAGIC,  2, unsigned long)
#define AES_IOC_GET_BUFSIZE     _IOW(CRYPTO_IOC_MAGIC,  3, unsigned long *)
#define AES_IOC_SET_IV			_IOW(CRYPTO_IOC_MAGIC,  5, unsigned long *)
#define AES_IOC_SET_KEY			_IOW(CRYPTO_IOC_MAGIC,  6, unsigned long *)
#define AES_IOC_START			_IOW(CRYPTO_IOC_MAGIC,  8, unsigned long)
#define AES_IOC_C_START			_IOW(CRYPTO_IOC_MAGIC,  9, unsigned long)
#define AES_IOC_UPDATE_IV		_IOW(CRYPTO_IOC_MAGIC, 11, unsigned long *)

extern void  *sha_current_context;

#define SHA_IOC_INIT	  		_IOW(CRYPTO_IOC_MAGIC, 21, unsigned long)
#define SHA_IOC_FINISH			_IOW(CRYPTO_IOC_MAGIC, 25, unsigned long)


#define ECC_CURVE_P_192      0x100192
#define ECC_CURVE_P_224      0x100224
#define ECC_CURVE_P_256      0x100256
#define ECC_CURVE_P_384      0x100384
#define ECC_CURVE_P_521      0x100521
#define ECC_CURVE_K_163      0x200163
#define ECC_CURVE_K_233      0x200233
#define ECC_CURVE_K_283      0x200283
#define ECC_CURVE_K_409      0x200409
#define ECC_CURVE_K_571      0x200571
#define ECC_CURVE_B_163      0x300163
#define ECC_CURVE_B_233      0x300233
#define ECC_CURVE_B_283      0x300283
#define ECC_CURVE_B_409      0x300409
#define ECC_CURVE_B_571      0x300571
#define ECC_CURVE_KO_192     0x400192
#define ECC_CURVE_KO_224     0x400224
#define ECC_CURVE_KO_256     0x400256
#define ECC_CURVE_BP_256     0x500256
#define ECC_CURVE_BP_384     0x500384
#define ECC_CURVE_BP_512     0x500512
#define ECC_CURVE_25519      0x025519

enum
{
    CURVE_GF_P,
    CURVE_GF_2M,
};

#define CRYPTO_IOC_MAGIC		'C'

#define AES_IOC_SET_MODE		_IOW(CRYPTO_IOC_MAGIC,  1, unsigned long)
#define AES_IOC_SET_LEN			_IOW(CRYPTO_IOC_MAGIC,  2, unsigned long)
#define AES_IOC_GET_BUFSIZE     _IOW(CRYPTO_IOC_MAGIC,  3, unsigned long *)
#define AES_IOC_SET_IV			_IOW(CRYPTO_IOC_MAGIC,  5, unsigned long *)
#define AES_IOC_SET_KEY			_IOW(CRYPTO_IOC_MAGIC,  6, unsigned long *)
#define AES_IOC_START			_IOW(CRYPTO_IOC_MAGIC,  8, unsigned long)
#define AES_IOC_C_START			_IOW(CRYPTO_IOC_MAGIC,  9, unsigned long)
#define AES_IOC_UPDATE_IV		_IOW(CRYPTO_IOC_MAGIC, 11, unsigned long *)

#define SHA_IOC_INIT	  		_IOW(CRYPTO_IOC_MAGIC, 21, unsigned long)
#define SHA_IOC_FINISH			_IOW(CRYPTO_IOC_MAGIC, 25, unsigned long)

#define ECC_IOC_SEL_CURVE	  	_IOW(CRYPTO_IOC_MAGIC, 51, unsigned long)
#define ECC_IOC_SET_PRI_KEY     _IOW(CRYPTO_IOC_MAGIC, 52, unsigned char *)
#define ECC_IOC_SET_PUB_K1      _IOW(CRYPTO_IOC_MAGIC, 53, unsigned char *)
#define ECC_IOC_SET_PUB_K2      _IOW(CRYPTO_IOC_MAGIC, 54, unsigned char *)
#define ECC_IOC_SET_SCALAR_K    _IOW(CRYPTO_IOC_MAGIC, 55, unsigned char *)
#define ECC_IOC_SET_MSG         _IOW(CRYPTO_IOC_MAGIC, 56, unsigned char *)
#define ECC_IOC_SET_SIG_R       _IOW(CRYPTO_IOC_MAGIC, 57, unsigned char *)
#define ECC_IOC_SET_SIG_S       _IOW(CRYPTO_IOC_MAGIC, 58, unsigned char *)
#define ECC_IOC_GET_PUB_K1      _IOW(CRYPTO_IOC_MAGIC, 61, unsigned char *)
#define ECC_IOC_GET_PUB_K2      _IOW(CRYPTO_IOC_MAGIC, 62, unsigned char *)
#define ECC_IOC_GET_SIG_R       _IOW(CRYPTO_IOC_MAGIC, 63, unsigned char *)
#define ECC_IOC_GET_SIG_S       _IOW(CRYPTO_IOC_MAGIC, 64, unsigned char *)
#define ECC_IOC_GEN_PUB_KEY     _IOW(CRYPTO_IOC_MAGIC, 71, unsigned long)
#define ECC_IOC_ECDSA_SIGN      _IOW(CRYPTO_IOC_MAGIC, 72, unsigned long)
#define ECC_IOC_ECDSA_VERIFY    _IOW(CRYPTO_IOC_MAGIC, 73, unsigned long)
#define ECC_IOC_POINT_MUL       _IOW(CRYPTO_IOC_MAGIC, 81, unsigned long)

#define RSA_IOC_SET_BIT_LEN     _IOW(CRYPTO_IOC_MAGIC, 90, unsigned long)
#define RSA_IOC_SET_N           _IOW(CRYPTO_IOC_MAGIC, 91, unsigned char *)
#define RSA_IOC_SET_D           _IOW(CRYPTO_IOC_MAGIC, 92, unsigned char *)
#define RSA_IOC_SET_E           _IOW(CRYPTO_IOC_MAGIC, 93, unsigned char *)
#define RSA_IOC_SET_C           _IOW(CRYPTO_IOC_MAGIC, 94, unsigned char *)
#define RSA_IOC_SET_MSG         _IOW(CRYPTO_IOC_MAGIC, 95, unsigned char *)
#define RSA_IOC_GET_MSG         _IOW(CRYPTO_IOC_MAGIC, 96, unsigned char *)
#define RSA_IOC_GET_SIG         _IOW(CRYPTO_IOC_MAGIC, 97, unsigned char *)
#define RSA_IOC_SET_SIG         _IOW(CRYPTO_IOC_MAGIC, 98, unsigned char *)
#define RSA_IOC_DO_SIGN         _IOW(CRYPTO_IOC_MAGIC, 101, unsigned long)
#define RSA_IOC_DO_VERIFY       _IOW(CRYPTO_IOC_MAGIC, 102, unsigned long)


/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/


# if OPENSSL_API_COMPAT < 0x10100000L
#  define SSLeay                  OpenSSL_version_num
#  define SSLeay_version          OpenSSL_version
#  define SSLEAY_VERSION_NUMBER   OPENSSL_VERSION_NUMBER
#  define SSLEAY_VERSION          OPENSSL_VERSION
#  define SSLEAY_CFLAGS           OPENSSL_CFLAGS
#  define SSLEAY_BUILT_ON         OPENSSL_BUILT_ON
#  define SSLEAY_PLATFORM         OPENSSL_PLATFORM
#  define SSLEAY_DIR              OPENSSL_DIR

/*
 * Old type for allocating dynamic locks. No longer used. Use the new thread
 * API instead.
 */
typedef struct {
    int dummy;
} CRYPTO_dynlock;

# endif /* OPENSSL_API_COMPAT */

typedef void CRYPTO_RWLOCK;

CRYPTO_RWLOCK *CRYPTO_THREAD_lock_new(void);
int CRYPTO_THREAD_read_lock(CRYPTO_RWLOCK *lock);
int CRYPTO_THREAD_write_lock(CRYPTO_RWLOCK *lock);
int CRYPTO_THREAD_unlock(CRYPTO_RWLOCK *lock);
void CRYPTO_THREAD_lock_free(CRYPTO_RWLOCK *lock);

int CRYPTO_atomic_add(int *val, int amount, int *ret, CRYPTO_RWLOCK *lock);

/*
 * The following can be used to detect memory leaks in the library. If
 * used, it turns on malloc checking
 */
# define CRYPTO_MEM_CHECK_OFF     0x0   /* Control only */
# define CRYPTO_MEM_CHECK_ON      0x1   /* Control and mode bit */
# define CRYPTO_MEM_CHECK_ENABLE  0x2   /* Control and mode bit */
# define CRYPTO_MEM_CHECK_DISABLE 0x3   /* Control only */

struct crypto_ex_data_st {
    STACK_OF(void) *sk;
};
DEFINE_STACK_OF(void)

/*
 * Per class, we have a STACK of function pointers.
 */
# define CRYPTO_EX_INDEX_SSL              0
# define CRYPTO_EX_INDEX_SSL_CTX          1
# define CRYPTO_EX_INDEX_SSL_SESSION      2
# define CRYPTO_EX_INDEX_X509             3
# define CRYPTO_EX_INDEX_X509_STORE       4
# define CRYPTO_EX_INDEX_X509_STORE_CTX   5
# define CRYPTO_EX_INDEX_DH               6
# define CRYPTO_EX_INDEX_DSA              7
# define CRYPTO_EX_INDEX_EC_KEY           8
# define CRYPTO_EX_INDEX_RSA              9
# define CRYPTO_EX_INDEX_ENGINE          10
# define CRYPTO_EX_INDEX_UI              11
# define CRYPTO_EX_INDEX_BIO             12
# define CRYPTO_EX_INDEX_APP             13
# define CRYPTO_EX_INDEX_UI_METHOD       14
# define CRYPTO_EX_INDEX_DRBG            15
# define CRYPTO_EX_INDEX__COUNT          16

/*
 * This is the default callbacks, but we can have others as well: this is
 * needed in Win32 where the application malloc and the library malloc may
 * not be the same.
 */
#define OPENSSL_malloc_init() \
    CRYPTO_set_mem_functions(CRYPTO_malloc, CRYPTO_realloc, CRYPTO_free)

int CRYPTO_mem_ctrl(int mode);

# define OPENSSL_malloc(num) \
        CRYPTO_malloc(num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_zalloc(num) \
        CRYPTO_zalloc(num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_realloc(addr, num) \
        CRYPTO_realloc(addr, num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_clear_realloc(addr, old_num, num) \
        CRYPTO_clear_realloc(addr, old_num, num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_clear_free(addr, num) \
        CRYPTO_clear_free(addr, num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_free(addr) \
        CRYPTO_free(addr, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_memdup(str, s) \
        CRYPTO_memdup((str), s, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_strdup(str) \
        CRYPTO_strdup(str, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_strndup(str, n) \
        CRYPTO_strndup(str, n, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_secure_malloc(num) \
        CRYPTO_secure_malloc(num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_secure_zalloc(num) \
        CRYPTO_secure_zalloc(num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_secure_free(addr) \
        CRYPTO_secure_free(addr, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_secure_clear_free(addr, num) \
        CRYPTO_secure_clear_free(addr, num, OPENSSL_FILE, OPENSSL_LINE)
# define OPENSSL_secure_actual_size(ptr) \
        CRYPTO_secure_actual_size(ptr)

size_t OPENSSL_strlcpy(char *dst, const char *src, size_t siz);
size_t OPENSSL_strlcat(char *dst, const char *src, size_t siz);
size_t OPENSSL_strnlen(const char *str, size_t maxlen);
char *OPENSSL_buf2hexstr(const unsigned char *buffer, long len);
unsigned char *OPENSSL_hexstr2buf(const char *str, long *len);
int OPENSSL_hexchar2int(unsigned char c);

# define OPENSSL_MALLOC_MAX_NELEMS(type)  (((1U<<(sizeof(int)*8-1))-1)/sizeof(type))

unsigned long OpenSSL_version_num(void);
const char *OpenSSL_version(int type);
# define OPENSSL_VERSION          0
# define OPENSSL_CFLAGS           1
# define OPENSSL_BUILT_ON         2
# define OPENSSL_PLATFORM         3
# define OPENSSL_DIR              4
# define OPENSSL_ENGINES_DIR      5

int OPENSSL_issetugid(void);

typedef void CRYPTO_EX_new (void *parent, void *ptr, CRYPTO_EX_DATA *ad,
                           int idx, long argl, void *argp);
typedef void CRYPTO_EX_free (void *parent, void *ptr, CRYPTO_EX_DATA *ad,
                             int idx, long argl, void *argp);
typedef int CRYPTO_EX_dup (CRYPTO_EX_DATA *to, const CRYPTO_EX_DATA *from,
                           void *from_d, int idx, long argl, void *argp);
__owur int CRYPTO_get_ex_new_index(int class_index, long argl, void *argp,
                            CRYPTO_EX_new *new_func, CRYPTO_EX_dup *dup_func,
                            CRYPTO_EX_free *free_func);
/* No longer use an index. */
int CRYPTO_free_ex_index(int class_index, int idx);

/*
 * Initialise/duplicate/free CRYPTO_EX_DATA variables corresponding to a
 * given class (invokes whatever per-class callbacks are applicable)
 */
int CRYPTO_new_ex_data(int class_index, void *obj, CRYPTO_EX_DATA *ad);
int CRYPTO_dup_ex_data(int class_index, CRYPTO_EX_DATA *to,
                       const CRYPTO_EX_DATA *from);

void CRYPTO_free_ex_data(int class_index, void *obj, CRYPTO_EX_DATA *ad);

/*
 * Get/set data in a CRYPTO_EX_DATA variable corresponding to a particular
 * index (relative to the class type involved)
 */
int CRYPTO_set_ex_data(CRYPTO_EX_DATA *ad, int idx, void *val);
void *CRYPTO_get_ex_data(const CRYPTO_EX_DATA *ad, int idx);

# if OPENSSL_API_COMPAT < 0x10100000L
/*
 * This function cleans up all "ex_data" state. It mustn't be called under
 * potential race-conditions.
 */
# define CRYPTO_cleanup_all_ex_data() while(0) continue

/*
 * The old locking functions have been removed completely without compatibility
 * macros. This is because the old functions either could not properly report
 * errors, or the returned error values were not clearly documented.
 * Replacing the locking functions with no-ops would cause race condition
 * issues in the affected applications. It is far better for them to fail at
 * compile time.
 * On the other hand, the locking callbacks are no longer used.  Consequently,
 * the callback management functions can be safely replaced with no-op macros.
 */
#  define CRYPTO_num_locks()            (1)
#  define CRYPTO_set_locking_callback(func)
#  define CRYPTO_get_locking_callback()         (NULL)
#  define CRYPTO_set_add_lock_callback(func)
#  define CRYPTO_get_add_lock_callback()        (NULL)

/*
 * These defines where used in combination with the old locking callbacks,
 * they are not called anymore, but old code that's not called might still
 * use them.
 */
#  define CRYPTO_LOCK             1
#  define CRYPTO_UNLOCK           2
#  define CRYPTO_READ             4
#  define CRYPTO_WRITE            8

/* This structure is no longer used */
typedef struct crypto_threadid_st {
    int dummy;
} CRYPTO_THREADID;
/* Only use CRYPTO_THREADID_set_[numeric|pointer]() within callbacks */
#  define CRYPTO_THREADID_set_numeric(id, val)
#  define CRYPTO_THREADID_set_pointer(id, ptr)
#  define CRYPTO_THREADID_set_callback(threadid_func)   (0)
#  define CRYPTO_THREADID_get_callback()                (NULL)
#  define CRYPTO_THREADID_current(id)
#  define CRYPTO_THREADID_cmp(a, b)                     (-1)
#  define CRYPTO_THREADID_cpy(dest, src)
#  define CRYPTO_THREADID_hash(id)                      (0UL)

#  if OPENSSL_API_COMPAT < 0x10000000L
#   define CRYPTO_set_id_callback(func)
#   define CRYPTO_get_id_callback()                     (NULL)
#   define CRYPTO_thread_id()                           (0UL)
#  endif /* OPENSSL_API_COMPAT < 0x10000000L */

#  define CRYPTO_set_dynlock_create_callback(dyn_create_function)
#  define CRYPTO_set_dynlock_lock_callback(dyn_lock_function)
#  define CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function)
#  define CRYPTO_get_dynlock_create_callback()          (NULL)
#  define CRYPTO_get_dynlock_lock_callback()            (NULL)
#  define CRYPTO_get_dynlock_destroy_callback()         (NULL)
# endif /* OPENSSL_API_COMPAT < 0x10100000L */

int CRYPTO_set_mem_functions(
        void *(*m) (size_t, const char *, int),
        void *(*r) (void *, size_t, const char *, int),
        void (*f) (void *, const char *, int));
int CRYPTO_set_mem_debug(int flag);
void CRYPTO_get_mem_functions(
        void *(**m) (size_t, const char *, int),
        void *(**r) (void *, size_t, const char *, int),
        void (**f) (void *, const char *, int));

void *CRYPTO_malloc(size_t num, const char *file, int line);
void *CRYPTO_zalloc(size_t num, const char *file, int line);
void *CRYPTO_memdup(const void *str, size_t siz, const char *file, int line);
char *CRYPTO_strdup(const char *str, const char *file, int line);
char *CRYPTO_strndup(const char *str, size_t s, const char *file, int line);
void CRYPTO_free(void *ptr, const char *file, int line);
void CRYPTO_clear_free(void *ptr, size_t num, const char *file, int line);
void *CRYPTO_realloc(void *addr, size_t num, const char *file, int line);
void *CRYPTO_clear_realloc(void *addr, size_t old_num, size_t num,
                           const char *file, int line);

int CRYPTO_secure_malloc_init(size_t sz, int minsize);
int CRYPTO_secure_malloc_done(void);
void *CRYPTO_secure_malloc(size_t num, const char *file, int line);
void *CRYPTO_secure_zalloc(size_t num, const char *file, int line);
void CRYPTO_secure_free(void *ptr, const char *file, int line);
void CRYPTO_secure_clear_free(void *ptr, size_t num,
                              const char *file, int line);
int CRYPTO_secure_allocated(const void *ptr);
int CRYPTO_secure_malloc_initialized(void);
size_t CRYPTO_secure_actual_size(void *ptr);
size_t CRYPTO_secure_used(void);

void OPENSSL_cleanse(void *ptr, size_t len);

# ifndef OPENSSL_NO_CRYPTO_MDEBUG
#  define OPENSSL_mem_debug_push(info) \
        CRYPTO_mem_debug_push(info, OPENSSL_FILE, OPENSSL_LINE)
#  define OPENSSL_mem_debug_pop() \
        CRYPTO_mem_debug_pop()
int CRYPTO_mem_debug_push(const char *info, const char *file, int line);
int CRYPTO_mem_debug_pop(void);
void CRYPTO_get_alloc_counts(int *mcount, int *rcount, int *fcount);

/*-
 * Debugging functions (enabled by CRYPTO_set_mem_debug(1))
 * The flag argument has the following significance:
 *   0:   called before the actual memory allocation has taken place
 *   1:   called after the actual memory allocation has taken place
 */
void CRYPTO_mem_debug_malloc(void *addr, size_t num, int flag,
        const char *file, int line);
void CRYPTO_mem_debug_realloc(void *addr1, void *addr2, size_t num, int flag,
        const char *file, int line);
void CRYPTO_mem_debug_free(void *addr, int flag,
        const char *file, int line);

int CRYPTO_mem_leaks_cb(int (*cb) (const char *str, size_t len, void *u),
                        void *u);
#  ifndef OPENSSL_NO_STDIO
int CRYPTO_mem_leaks_fp(FILE *);
#  endif
int CRYPTO_mem_leaks(BIO *bio);
# endif

/* die if we have to */
ossl_noreturn void OPENSSL_die(const char *assertion, const char *file, int line);
# if OPENSSL_API_COMPAT < 0x10100000L
#  define OpenSSLDie(f,l,a) OPENSSL_die((a),(f),(l))
# endif
# define OPENSSL_assert(e) \
    (void)((e) ? 0 : (OPENSSL_die("assertion failed: " #e, OPENSSL_FILE, OPENSSL_LINE), 1))

int OPENSSL_isservice(void);

int FIPS_mode(void);
int FIPS_mode_set(int r);

void OPENSSL_init(void);
# ifdef OPENSSL_SYS_UNIX
void OPENSSL_fork_prepare(void);
void OPENSSL_fork_parent(void);
void OPENSSL_fork_child(void);
# endif

struct tm *OPENSSL_gmtime(const time_t *timer, struct tm *result);
int OPENSSL_gmtime_adj(struct tm *tm, int offset_day, long offset_sec);
int OPENSSL_gmtime_diff(int *pday, int *psec,
                        const struct tm *from, const struct tm *to);

/*
 * CRYPTO_memcmp returns zero iff the |len| bytes at |a| and |b| are equal.
 * It takes an amount of time dependent on |len|, but independent of the
 * contents of |a| and |b|. Unlike memcmp, it cannot be used to put elements
 * into a defined order as the return value when a != b is undefined, other
 * than to be non-zero.
 */
int CRYPTO_memcmp(const void * in_a, const void * in_b, size_t len);

/* Standard initialisation options */
# define OPENSSL_INIT_NO_LOAD_CRYPTO_STRINGS 0x00000001L
# define OPENSSL_INIT_LOAD_CRYPTO_STRINGS    0x00000002L
# define OPENSSL_INIT_ADD_ALL_CIPHERS        0x00000004L
# define OPENSSL_INIT_ADD_ALL_DIGESTS        0x00000008L
# define OPENSSL_INIT_NO_ADD_ALL_CIPHERS     0x00000010L
# define OPENSSL_INIT_NO_ADD_ALL_DIGESTS     0x00000020L
# define OPENSSL_INIT_LOAD_CONFIG            0x00000040L
# define OPENSSL_INIT_NO_LOAD_CONFIG         0x00000080L
# define OPENSSL_INIT_ASYNC                  0x00000100L
# define OPENSSL_INIT_ENGINE_RDRAND          0x00000200L
# define OPENSSL_INIT_ENGINE_DYNAMIC         0x00000400L
# define OPENSSL_INIT_ENGINE_OPENSSL         0x00000800L
# define OPENSSL_INIT_ENGINE_CRYPTODEV       0x00001000L
# define OPENSSL_INIT_ENGINE_CAPI            0x00002000L
# define OPENSSL_INIT_ENGINE_PADLOCK         0x00004000L
# define OPENSSL_INIT_ENGINE_AFALG           0x00008000L
/* OPENSSL_INIT_ZLIB                         0x00010000L */
# define OPENSSL_INIT_ATFORK                 0x00020000L
/* OPENSSL_INIT_BASE_ONLY                    0x00040000L */
/* OPENSSL_INIT flag range 0xfff00000 reserved for OPENSSL_init_ssl() */
/* Max OPENSSL_INIT flag value is 0x80000000 */

/* openssl and dasync not counted as builtin */
# define OPENSSL_INIT_ENGINE_ALL_BUILTIN \
    (OPENSSL_INIT_ENGINE_RDRAND | OPENSSL_INIT_ENGINE_DYNAMIC \
    | OPENSSL_INIT_ENGINE_CRYPTODEV | OPENSSL_INIT_ENGINE_CAPI | \
    OPENSSL_INIT_ENGINE_PADLOCK)


/* Library initialisation functions */
void OPENSSL_cleanup(void);
int OPENSSL_init_crypto(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings);
int OPENSSL_atexit(void (*handler)(void));
void OPENSSL_thread_stop(void);

/* Low-level control of initialization */
OPENSSL_INIT_SETTINGS *OPENSSL_INIT_new(void);
# ifndef OPENSSL_NO_STDIO
int OPENSSL_INIT_set_config_appname(OPENSSL_INIT_SETTINGS *settings,
                                    const char *config_file);
# endif
void OPENSSL_INIT_free(OPENSSL_INIT_SETTINGS *settings);

# if defined(OPENSSL_THREADS) && !defined(CRYPTO_TDEBUG)
#  if defined(_WIN32)
#   if defined(BASETYPES) || defined(_WINDEF_H)
/* application has to include <windows.h> in order to use this */
typedef DWORD CRYPTO_THREAD_LOCAL;
typedef DWORD CRYPTO_THREAD_ID;

typedef LONG CRYPTO_ONCE;
#    define CRYPTO_ONCE_STATIC_INIT 0
#   endif
#  else
#   include <pthread.h>
typedef pthread_once_t CRYPTO_ONCE;
typedef pthread_key_t CRYPTO_THREAD_LOCAL;
typedef pthread_t CRYPTO_THREAD_ID;

#   define CRYPTO_ONCE_STATIC_INIT PTHREAD_ONCE_INIT
#  endif
# endif

# if !defined(CRYPTO_ONCE_STATIC_INIT)
typedef unsigned int CRYPTO_ONCE;
typedef unsigned int CRYPTO_THREAD_LOCAL;
typedef unsigned int CRYPTO_THREAD_ID;
#  define CRYPTO_ONCE_STATIC_INIT 0
# endif

int CRYPTO_THREAD_run_once(CRYPTO_ONCE *once, void (*init)(void));

int CRYPTO_THREAD_init_local(CRYPTO_THREAD_LOCAL *key, void (*cleanup)(void *));
void *CRYPTO_THREAD_get_local(CRYPTO_THREAD_LOCAL *key);
int CRYPTO_THREAD_set_local(CRYPTO_THREAD_LOCAL *key, void *val);
int CRYPTO_THREAD_cleanup_local(CRYPTO_THREAD_LOCAL *key);

CRYPTO_THREAD_ID CRYPTO_THREAD_get_current_id(void);
int CRYPTO_THREAD_compare_id(CRYPTO_THREAD_ID a, CRYPTO_THREAD_ID b);


# ifdef  __cplusplus
}
# endif
#endif
