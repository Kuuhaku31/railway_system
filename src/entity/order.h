//
// Created by sy200 on 24-12-17.
//

#ifndef RAILWAYSYSTEM_ORDER_H
#define RAILWAYSYSTEM_ORDER_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
typedef struct {
    uint32_t id; // 唯一 ID
    uint32_t train_id;
    uint32_t user_id;
    bool is_canceled;
} Order;
#ifdef __cplusplus
}
#endif
#endif //RAILWAYSYSTEM_ORDER_H
