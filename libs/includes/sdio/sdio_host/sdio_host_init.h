#ifndef _SDIO_HOST_INIT_H_
#define _SDIO_HOST_INIT_H_

//选择SDIO0? SDIO1? 作为输出,默认为SDIO1
#define SDIO_GRP_1  0
#define SDIO_GRP_0  (1 << 31)
#define SDIO_GRP_MASK  (1 << 31)

//选择SDIOx的哪个出口作为输出,默认为出口0
#define SDIO_PORT_0  0
#define SDIO_PORT_1  (1 << 30)
#define SDIO_PORT_2  (1 << 29)
#define SDIO_PORT_3  (1 << 30|1 << 29)
#define SDIO_PORT_MASK  (1 << 30|1 << 29)

//是否使用四线模式, 默认为单线模式
#define SDIO_1_BIT_DATA  0
#define SDIO_4_BIT_DATA  (1 << 28)
#define SDIO_4_BIT_DATA_MASK  (1 << 28)

//是否使用硬件中断检测外设事件,例如接收到数据. , 默认使用轮询方式查询事件
#define SDIO_POLLING  0
#define SDIO_DATA1_IRQ  (1 << 27)
#define SDIO_DATA1_IRQ_MASK  (1 << 27)

//配置SDIO时钟(HZ), 默认为40MHZ
#define SDIO_MAX_CLK_MASK ((1 << 27)-1)

/*
sdio_host_init(0);
sdio_host_init(SDIO_GRP_1|SDIO_4_BIT_DATA);
sdio_host_init(SDIO_GRP_1|SDIO_PORT_1|SDIO_DATA1_IRQ);
sdio_host_init(SDIO_GRP_0|SDIO_PORT_0|SDIO_4_BIT_DATA|SDIO_DATA1_IRQ|(10*1000000));
*/
#endif  //_SDIO_HOST_INIT_H_
