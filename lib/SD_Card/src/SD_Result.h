#ifndef SD_RESULT_H
#define SD_RESULT_H

// General success
#define SD_OK 0

// Low-level errors
#define SD_ERR_TIMEOUT            -1
#define SD_ERR_NO_RESPONSE        -2
#define SD_ERR_BAD_CRC            -3
#define SD_ERR_ILLEGAL_COMMAND    -4

// Command-specific errors
#define SD_ERR_CMD0               -10
#define SD_ERR_CMD8               -11
#define SD_ERR_CMD17              -12
#define SD_ERR_CMD55              -13
#define SD_ERR_CMD58              -14
#define SD_ERR_ACMD41             -15
#define SD_ERR_READ_TOKEN_TIMEOUT -16

// Data errors
#define SD_ERR_READ_TIMEOUT       -20
#define SD_ERR_WRITE_TIMEOUT      -21
#define SD_ERR_DATA_TOKEN         -22
#define SD_ERR_DATA_CRC           -23
#define SD_ERR_START_BIT          -24

// Logical issues
#define SD_ERR_NOT_INITIALIZED    -30
#define SD_ERR_INVALID_ARGUMENT   -31
#define SD_ERR_OUT_OF_RANGE       -32
#define SD_ERR_CARD_LOCKED        -33

// SPI or peripheral errors
#define SD_ERR_SPI_FAIL           -40
#define SD_ERR_CS_NOT_LOW         -41

// Other
#define SD_ERR_UNKNOWN            -100

#endif