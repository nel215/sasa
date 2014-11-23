#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>

bool is_lms(int index, const std::vector<bool> &sl_seq){
    if(index+1 == (int)sl_seq.size())return true;
    if(0 < index && index < (int)sl_seq.size()){
        return sl_seq[index] && !sl_seq[index-1];
    }
    return false;
}
// utils
template<class T> void output_vector(const std::vector<T> &vec){
    for(int i=0 ; i<(int)vec.size() ; i++){
        std::cerr << vec[i] << (i==(int)vec.size()-1 ? '\n' : ' ');
    }
}

void output_sq_sequence(const std::vector<bool> &sl_seq){
    for(int i=0 ; i<(int)sl_seq.size() ; i++){
        std::cerr << (sl_seq[i] ? 'S' : 'L') << (i==(int)sl_seq.size()-1 ? '\n' : ' ');
    }
    for(int i=0 ; i<(int)sl_seq.size() ; i++){
        std::cerr << (is_lms(i,sl_seq) ? '*' : ' ') << (i==(int)sl_seq.size()-1 ? '\n' : ' ');
    }
}

#endif
