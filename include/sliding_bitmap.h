/***************************************************************************
 * 
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
/**
 * @file sliding_window/dev/sliding_bitmap.h
 * @author knzeus(zhou_jie@baidu.com)
 * @date 2015/03/31 23:33:51
 * @version $Revision$ 
 * @brief: a bitmap but support sliding bit
 *         for SlidingWindow
 *  
 **/

#ifndef ANTI_THEMIS_SLIDING_WINDOW_DEV_SLIDING_BITMAP_H
#define ANTI_THEMIS_SLIDING_WINDOW_DEV_SLIDING_BITMAP_H

#include <bitset>

namespace anti {
namespace themis {
namespace common_lib {

// use two bitset to simulate the sliding bitset
template<int bit_size>
class SlidingBitmap {
    typedef std::bitset<bit_size> Bitmap;
public:
    SlidingBitmap();
    ~SlidingBitmap();

    bool init(int valid_size);

    // @param pos: start with zero
    bool set(int pos);

    // @param pos: start with zero
    bool operator[](int pos);

    int size() const {
        return _valid_size;
    }

    bool any() const {
        return _bitmap[0]->any() || _bitmap[1]->any();
    }

    void slide_one_bit();
    void slide(int bits);

    void clear() {
        _reset_all();
    }

private:
    bool _find_pos(const int pos_in, Bitmap** p_map, int* pos_out) const;
    void _swap_bitmap();
    void _reset_all();

private:
    Bitmap* _bitmap[2];
    // start with zero
    int _start_pos;
    int _valid_size;
};

}  // namespace common_lib
}  // namespace themis
}  // namespace anti

#include "sliding_bitmap.hpp"

#endif  // ANTI_THEMIS_SLIDING_WINDOW_DEV_SLIDING_BITMAP_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
