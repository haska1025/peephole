#ifndef _LIBPEEPHOLE_IOBUFFER_H_
#define _LIBPEEPHOLE_IOBUFFER_H_

#include <stdint.h>
#include "ph_decl.h"

PH_NAMESPACE_DECL_BEGIN
/**
 * The IOBuffer has two mode.
 * 1. The IOBuffer has memory ,so you need to call Initialize to init the IOBuffer
 * 2. The IOBuffer doesn't has memory. the user need to set the data by the SetBufferData
 */
class IOBuffer
{
public:
    IOBuffer();
    ~IOBuffer();

    /**
     * @brief Initialize the buffer, allocate the buffer's memory by buf_size
     * @param[in] buf_size the buffer's size
     * @param[in] chunk_minsize the chunk minimum size for published
     * @return 0 if success, -1 otherwise
     */
    int Initialize(uint32_t buf_size, uint32_t chunk_minsize);
    /**
     * @brief If you call Initialize,you must call Destroy to free the IOBuffer's memory
     * @return 0 if success, -1 otherwise
     */
    int Destroy();
    /**
     * @brief reserve a memory block
     * @param[in] size the needed size
     * @return 0 if success, -1 otherwise
     */
    int Reserve(uint32_t size);
    /**
     * @brief set the user data to the buffer
     * @param[in] data the user data
     * @param[in] data_size the user data size
     */
    void SetBufferData(void *data, uint32_t data_size);
    /**
     * @brief [1] adjust the consumed size.
     *        [2] when (size_ - published_) < chunk_minsize_, move the consumed_ and published_ to the begin of the buffer.
     *            let the consumed_ equals to 0 
     * @param[in] new_consumed the new consumed size
     */
    void AdjustConsumed(uint32_t new_consumed);
    /**
     * @brief adjust the published size 
     * @param[in] new_published the new published size
     */
    void AdjustPublished(uint32_t new_published);

    /** Adust unpublished capacity */
    int AdjustUnpublishedCapacity();
    /**
     * @brief Get the published pointer
     * @return the pubished pointer
     */
    uint8_t *published_pointer(){ return buffer_+published_;}
    uint32_t unpublished_size(){return size_ - published_;}

    uint8_t *buffer_pointer(){return buffer_;}
    uint32_t buffer_size(){return size_;}

    uint32_t consumed_size(){return consumed_;}

    uint8_t *consumed_pointer(){return buffer_ + consumed_;}
    uint32_t published_size(){return published_ - consumed_;}

    void print_bytes();
private:
    uint8_t *buffer_;/**< the chunk pointer */
    uint32_t size_;/**< the buffer total size */
    uint32_t consumed_;/**< it indicates the postion that the buffer have been consumed*/
    uint32_t published_;/**< it indicates the postion that the buffer can been used freely */
    uint32_t chunk_minsize_;/**< the minimum size for (size_ - published_) */
};
PH_NAMESPACE_DECL_END
#endif//_LIBPEEPHOLE_IOBUFFER_H_

