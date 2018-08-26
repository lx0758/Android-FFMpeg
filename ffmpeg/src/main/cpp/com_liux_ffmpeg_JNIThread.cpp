/**
 * pthread_t
 * pthread_creat
 * pthread_exit
 *
 * pthread_mutex_t
 * pthread_mutex_init
 * pthread_mutex_lock
 * pthread_mutex_unlock
 * pthread_mutex_destroy
 *
 * pthread_cond_t
 * pthread_cond_init
 * pthread_cond_signal
 * pthread_cond_wait
 * pthread_cond_destroy
 */

#include <jni.h>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include "android_log.h"

// std 是命名空间,类似 java 中的包名
// using namespace std;

int count = 0;
bool finish = false;
std::queue<int> queue;
pthread_t producer_t;
pthread_t consumer_t;
pthread_mutex_t mutex_t;
pthread_cond_t cond_t;

void pushQueue(int count);

void popQueue();

/**
 * 模拟功能累加一定数量队列后通知别的线程
 * @param data
 * @return
 */
void *runProducerCallback(void *data) {
    while (count < 50) {
        pushQueue(5);
        pthread_cond_signal(&cond_t);
    }

    finish = true;

    LOGD("[thread]:%s", "producer_t exit.");
    pthread_exit(&producer_t);
}

/**
 * 模拟延时操作,操作队列过程中加锁
 * @param size
 */
void pushQueue(int size) {
    for (int i = 0; i < size; i ++) {
        usleep(500 * 1000);
        count ++;
        LOGD("[thread]:queue push %d", count);

        pthread_mutex_lock(&mutex_t);
        queue.push(count);
        pthread_mutex_unlock(&mutex_t);
    }
}

/**
 * 检测队列为空时等待信号
 * @param data
 * @return
 */
void *runConsumerCallback(void *data) {
    while (true) {
        pthread_mutex_lock(&mutex_t);
        if (queue.empty()) {
            LOGD("[thread]:consumer_t wait");
            // 在执行等待时会解锁对象,不会造成死锁
            // 传入的 mutex 必须是一个已经锁定的对象
            pthread_cond_wait(&cond_t, &mutex_t);
        }
        popQueue();
        pthread_mutex_unlock(&mutex_t);

        if (finish && queue.empty()) break;
    }

    LOGD("[thread]:%s", "consumer_t exit.");
    pthread_exit(&consumer_t);
}

/**
 * 打印队列内的数据,操作队列时需要加锁
 */
void popQueue() {
    while (!queue.empty()) {
        usleep(50 * 1000);
        int value = queue.front();queue.pop();
        LOGD("[thread]:queue front %d", value);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_JNIThread_runThread(JNIEnv *env, jclass type) {

    pthread_create(&producer_t, NULL, runProducerCallback, NULL);
    pthread_create(&consumer_t, NULL, runConsumerCallback, NULL);

    pthread_mutex_init(&mutex_t, NULL);
    pthread_cond_init(&cond_t, NULL);
}