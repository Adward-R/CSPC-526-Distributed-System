#define BLK_SIZE 4096ll // block size
#define N_LOG_BLK 0x80000ll // number of blocks for the log, which is first 2GB
#define N_CKP_BLK 0x200000ll // number of blocks for the checkpoint, which is next 8GB
#define N_BLK (N_LOG_BLK + N_CKP_BLK) // number of blocks in total
#define DISK_SIZE (BLK_SIZE * N_BLK) // total size of the disk

#define MAGIC_NUMBER 0x3632352043535043ll // translates to string "CPSC 526"
