/***************************************************************************
 * 
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
/**
 * @file sliding_window/dev/sliding_bitmap.hpp
 * @author knzeus(zhou_jie@baidu.com)
 * @date 2015/03/31 23:38:02
 * @version 1.0.0.0
 * @brief The implementation of sliding bitmap
 *  
 **/

namespace anti {
namespace themis {
namespace common_lib {

template<int bit_size>
SlidingBitmap<bit_size>::SlidingBitmap() : 
        _start_pos(0),
        _valid_size(bit_size) {
    _bitmap[0] = _bitmap[1] = NULL;
}

template<int bit_size>
SlidingBitmap<bit_size>::~SlidingBitmap() {
    for (int i = 0; i < 2; i++) {
        if (_bitmap[i] != NULL) {
            delete _bitmap[i];
            _bitmap[i] = NULL;
        }
    }
    _valid_size = 0;
    _start_pos = 0;
}

template<int bit_size>
bool SlidingBitmap<bit_size>::init(int valid_size) {
    if (valid_size > bit_size) {
        return false;
    }
    _valid_size = valid_size;
    for (int idx = 0; idx < 2; idx++) {
        _bitmap[idx] = new (std::nothrow) Bitmap();
        if (_bitmap[idx] == NULL) {
            return false;
        }
        _bitmap[idx]->reset();
    }
    return true;
}

template<int bit_size>
bool SlidingBitmap<bit_size>::_find_pos(
        const int pos_in, 
        Bitmap** p_map, 
        int* pos_out) const {
    if (pos_in >= _valid_size) {
        return false;
    }
    if (p_map == NULL || pos_out == NULL) {
        return false;
    }
    Bitmap* which = _bitmap[0];
    int cur_pos = _start_pos + pos_in;
    if (cur_pos >= _valid_size) {
        which = _bitmap[1];
        cur_pos -= _valid_size;
    }
    *p_map = which;
    *pos_out = cur_pos;
    return true;
}

template<int bit_size>
bool SlidingBitmap<bit_size>::set(int pos) {
    Bitmap* which = NULL;
    int inner_pos = 0;
    if (!_find_pos(pos, &which, &inner_pos)) {
        return false;
    }
    which->set(inner_pos);
    return true;
}

// in order to be simple, reuse the retval
template<int bit_size>
bool SlidingBitmap<bit_size>::operator[] (int pos) {
    Bitmap* which = NULL;
    int inner_pos = 0;
    if (!_find_pos(pos, &which, &inner_pos)) {
        return false;
    }
    return which->test(inner_pos);
}

template<int bit_size>
void SlidingBitmap<bit_size>::slide_one_bit() {
    int last_pos = _valid_size - 1;
    if (_start_pos < last_pos) {
        _bitmap[0]->reset(_start_pos);
        _start_pos++;
    } else if (_start_pos == last_pos) {
        _start_pos = 0;
        _bitmap[0]->reset();
        _swap_bitmap();
    } else if (_start_pos > last_pos) {
        // nevel into this branch
        _start_pos = _start_pos % _valid_size;
        slide_one_bit();
    }
    return ;
}

template<int bit_size>
void SlidingBitmap<bit_size>::slide(int bits) {
    if (bits >= _valid_size) {
        _reset_all();
        _start_pos = 0;
        bits = 0;
        return ;
    }
    for (int cnt = 0; cnt < bits; cnt++) {
        slide_one_bit();
    }
    return ;
}

template<int bit_size>
void SlidingBitmap<bit_size>::_swap_bitmap() {
    Bitmap* tmp = _bitmap[0];
    _bitmap[0] = _bitmap[1];
    _bitmap[1] = tmp;
    return ;
}

template<int bit_size>
void SlidingBitmap<bit_size>::_reset_all() {
    _bitmap[0]->reset();
    _bitmap[1]->reset();
}

}  // namespace common_lib
}  // namespace themis
}  // namespace anti

/* vim: set ts=4 sw=4 sts=4 tw=100 */
