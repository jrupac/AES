#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

// Fill in actual key here
static const uchar ckey[] = "\x12\x12";

void print(const char *str, uchar *in, uchar *out)
{
    // Print in hex because encrypted data is not always ASCII-friendly ;-)

    int i;

    printf("%s\n", str);
    printf("Input:\t");
    for (i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%x", in[i]);
    printf("\n");
    printf("Output:\t");
    for (i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%x", out[i]);
    printf("\n\n");
}

void Encrypt(uchar *in, uchar *out, int debug)
{
    static int firstRun = 1;
    static AES_KEY encryptKey;

    if (firstRun == 1)
    {
        AES_set_encrypt_key(ckey, 256, &encryptKey);
        firstRun = 0;
    }

    AES_ecb_encrypt(in, out, &encryptKey, AES_ENCRYPT);

    if (debug)
        print("Encryption:", in, out);
}

void Decrypt(uchar *in, uchar *out, int debug)
{
    static int firstRun = 1;
    static AES_KEY decryptKey;

    if (firstRun == 1)
    {
        AES_set_decrypt_key(ckey, 256, &decryptKey);
        firstRun = 0;
    }

    AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);

    if (debug)
        print("Decryption:", in, out);
}

// Test client
int main(int argc, char **argv) 
{
    int BLOCKS = 1e6, PRINT = 0, i;
  
	argc--; argv++;
	while (argc > 0) 
    {
        if ((*argv)[0] == '-') 
        {
            if (!strcmp(*argv, "-help")) { printf("Usage: AES [-trials N] [-print]"); exit(0); }
            else if (!strcmp(*argv, "-trials")) { argc--; argv++; BLOCKS = atoi(*argv); }
            else if (!strcmp(*argv, "-print")) { PRINT = 1; }
            else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(0); }
        }

        argv++; 
        argc--;
	}

    // Sample input
    uchar in[AES_BLOCK_SIZE] = "helloworld1234\n";
    uchar out[AES_BLOCK_SIZE];

    printf("Running %d trials...\n", BLOCKS);

    // Run trials
    for (i = 0; i < BLOCKS; i++)
    {
        Encrypt(in, out, PRINT);
        Decrypt(out, in, PRINT);
    }
    
    printf("Completed encryption and decryption of %d blocks.\n", BLOCKS);

    return 0;
}
