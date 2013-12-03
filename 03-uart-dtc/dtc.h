#ifndef DTC_H__
#define DTC_H__

typedef struct dtc_control_data {
    unsigned char DTCCR;
    unsigned char DTBLS;
    unsigned char DTCCT;
    unsigned char DTRLD;
    unsigned int  DTSAR;
    unsigned int  DTDAR;
} dtc_control_data;

typedef struct __attribute__((aligned(256)))
dtc_area {
    unsigned char    vector[40];
    unsigned char    reserved[24];
    dtc_control_data data[24];
} dtc_area;

void dtc_init (void);
void dtc_set_data (unsigned char n, const volatile void *src, volatile void *dst, unsigned char size, unsigned char length, unsigned char mode);
void dtc_set_vector (unsigned char vector_n, unsigned char data_n);
void dtc_enable_vector (unsigned char vector_n);
void dtc_disable_vector (unsigned char vector_n);
void dtc_wait (unsigned char vector_n);

#define DTC_MODE_8_BITS  (0U)
#define DTC_MODE_16_BITS (1U << 6)
#define DTC_MODE_FIX_DST (0U)
#define DTC_MODE_INC_DST (1U << 3)
#define DTC_MODE_FIX_SRC (0U)
#define DTC_MODE_INC_SRC (1U << 2)

#define DTC_MODE_MASK    (DTC_MODE_16_BITS | DTC_MODE_INC_DST | DTC_MODE_INC_SRC)

#define DTC_VECTOR_0         (0U)
#define DTC_VECTOR_INTP0     (1U)
#define DTC_VECTOR_INTP1     (2U)
#define DTC_VECTOR_INTP2     (3U)
#define DTC_VECTOR_INTP3     (4U)
#define DTC_VECTOR_INTP4     (5U)
#define DTC_VECTOR_INTP5     (6U)
#define DTC_VECTOR_INTP6     (7U)
#define DTC_VECTOR_INTP7     (8U)
#define DTC_VECTOR_KR        (9U)
#define DTC_VECTOR_AD        (10U)
#define DTC_VECTOR_SR0_CSI01 (11U)
#define DTC_VECTOR_ST0_CSI00 (12U)
#define DTC_VECTOR_SR1_CSI11 (13U)
#define DTC_VECTOR_ST1_CSI10 (14U)
#define DTC_VECTOR_SR2_CSI21 (15U)
#define DTC_VECTOR_ST2_CSI20 (16U)
#define DTC_VECTOR_SR3_CSI31 (17U)
#define DTC_VECTOR_ST3_CSI30 (18U)
#define DTC_VECTOR_TM00      (19U)
#define DTC_VECTOR_TM01      (20U)
#define DTC_VECTOR_TM02      (21U)
#define DTC_VECTOR_TM03      (22U)
#define DTC_VECTOR_TM10      (23U)
#define DTC_VECTOR_TM11      (24U)
#define DTC_VECTOR_TM12      (25U)
#define DTC_VECTOR_TM13      (26U)
#define DTC_VECTOR_TRDA0     (27U)
#define DTC_VECTOR_TRDB0     (28U)
#define DTC_VECTOR_TRDC0     (29U)
#define DTC_VECTOR_TRDD0     (30U)
#define DTC_VECTOR_TRDA1     (31U)
#define DTC_VECTOR_TRDB1     (32U)
#define DTC_VECTOR_TRDC1     (33U)
#define DTC_VECTOR_TRDD1     (34U)
#define DTC_VECTOR_TRGA      (35U)
#define DTC_VECTOR_TRGB      (36U)
#define DTC_VECTOR_TRJ0      (37U)
#define DTC_VECTOR_CMP0      (38U)
#define DTC_VECTOR_CMP1      (39U)

#endif  /* DTC_H__ */
