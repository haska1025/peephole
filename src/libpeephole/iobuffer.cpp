#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iomanip>

#include "iobuffer.h"
#include "ph_log.h"

IOBuffer::IOBuffer()
{
    buffer_ = NULL;
    size_ = 0;
    consumed_ = 0;
    published_ = 0;
    chunk_minsize_ = 0;
}
IOBuffer::~IOBuffer()
{
}

int IOBuffer::Initialize(uint32_t buf_size, uint32_t chunk_minsize)
{
    if (buf_size < 2 * chunk_minsize)
        return -1;

    size_ = buf_size;
    chunk_minsize_ = chunk_minsize;
    buffer_ = (uint8_t*)malloc(size_);

    return 0;
}
int IOBuffer::Reserve(uint32_t size)
{
    uint32_t t, newsize;
    if (size <= size_){
        consumed_ = 0;
        published_ = 0;
        chunk_minsize_ = 0;
        return 0;
    }

    if ((size_ > (1<<16)) || (size > (1<<16))){
        LOG_WARNING("the size(%u) or newsize(%u) gt 64k", size_, size);
        return -1;
    }

    t = (size + size_ - 1) / size_;
    newsize = t * size_;
    if (newsize > (1<<16)){
        LOG_WARNING("the newsize(%u) gt 64k", newsize);
        return -1;
    }
    free(buffer_);
    buffer_ = (uint8_t*)malloc(newsize);
    size_ = newsize;
    consumed_ = 0;
    published_ = 0;
    chunk_minsize_ = 0;

    return 0;
}
void IOBuffer::SetBufferData(void *data, uint32_t data_size)
{
    size_ = data_size;
    buffer_ = (uint8_t*)data;
}
void IOBuffer::AdjustConsumed(uint32_t new_consumed)
{
    /* We need to check the new_consumed size */
    if (new_consumed > published_size()){
        LOG_ERR("Adjust consumed check new_consumed(%u) gt publisedsize(%u)",
                new_consumed,
                published_size());

        abort();
    }
    /* No consumed */
    if (new_consumed == 0)
        return;

    consumed_ += new_consumed;

    /* the published size is 0,the buffer is empty */
    if (consumed_ == published_){
        consumed_ = 0;
        published_ = 0;
    }
}

int IOBuffer::AdjustUnpublishedCapacity()
{
    /* the size of unpublised continuous memory less than the chunk_minsize_ */
    if (size_ - published_ < chunk_minsize_){
        if ((consumed_ + (size_-published_)) >= chunk_minsize_){
            LOG_DEBUG("adjust buffer(%p) size(%u) consumed(%u) published(%u)",buffer_, size_,consumed_,published_);
            memmove(buffer_, buffer_+consumed_, published_ - consumed_);
            published_ = published_ - consumed_;
            consumed_ = 0;
        }else{
            
            if (size_ >= (1<<16)){
                LOG_WARNING("the size of buffer ge 64k size(%u) consumed(%u) published(%u)",size_,consumed_,published_);
                consumed_ = 0;
                published_ = 0;
                /* The message to large */
                //abort();
                /* the size of stream is too long */
                return -118;
            }
            LOG_WARNING("the buffer will be enlarged size(%u) consumed(%u) published(%u)",size_,consumed_,published_);
            /* enlarge the buffer capacity to 2 times of the old buffer length */
            size_t newsize = size_<<1;
            uint8_t *newbuffer = (uint8_t*)malloc(newsize); 
            memcpy(newbuffer+consumed_, buffer_+consumed_, published_ - consumed_);
            free(buffer_);/* free the old buffer memory */
            buffer_ = newbuffer;
            size_ = (uint32_t)newsize;
        }
    }
    return 0;
}

void IOBuffer::AdjustPublished(uint32_t new_published)
{
    /* We need to check the new_published size */
    if (new_published > unpublished_size()){
        LOG_ERR("Adjust consumed check new_published(%u) gt unpublisedsize(%u)",
                new_published,
                unpublished_size());

        abort();
    }
    published_ += new_published;
}

int IOBuffer::Destroy()
{
    free(buffer_);
    buffer_ = NULL;

    return 0;
}

void IOBuffer::print_bytes()
{
    std::ostringstream oss;
    uint8_t *p = consumed_pointer();
    uint32_t len = published_size();
    oss << std::hex << std::setfill('0');
    for(int i = 0; i < (int)len; ++i){
        oss << std::setw(2) << int(*(p+i)) << " ";
    }
    LOG_INFO("iobufer(%p) len(%u) bytes[%s]",this, len, oss.str().c_str());
}


