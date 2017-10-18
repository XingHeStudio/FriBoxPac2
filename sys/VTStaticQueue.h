#ifndef VT_STATIC_QUEUE_H
#define VT_STATIC_QUEUE_H
#include "HalCtype.h"

#define VTSQueue(name) (name)

#define VTSQueueDef(type, queue, size) \
struct queue##_st\
{ \
uint16_t front, back, count; \
type items[size]; \
}VTSQueue(queue)

#define VTSQueuePush(queue, item) \
do{ \
if(VTSQueueHasSpace(queue)) \
{ \
VTSQueue(queue).items[VTSQueue(queue).back] = item; \
VTSQueue(queue).back = (VTSQueue(queue).back + 1) % sizeof(VTSQueue(queue).items); \
VTSQueue(queue).count++; \
} \
}while(0)

#define VTSQueueHasSpace(queue) (VTSQueue(queue).count < sizeof(VTSQueue(queue).items))

#define VTSQueueIsEmpty(queue) (VTSQueue(queue).count == 0)

#define VTSQueueCount(queue) VTSQueue(queue).count

#define VTSQueueFront(queue) VTSQueue(queue).items[VTSQueue(queue).front]

#define VTSQueuePop(queue) \
do{ \
if(!VTSQueueIsEmpty(queue)) \
{ \
    VTSQueue(queue).front = (VTSQueue(queue).front + 1) % sizeof(VTSQueue(queue).items); \
    VTSQueue(queue).count--; \
} \
}while(0)


#endif // VT_STATIC_QUEUE_H
