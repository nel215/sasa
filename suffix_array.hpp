#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include <vector>
#include <string>
#include <set>
#include <algorithm>

namespace sasa{

bool is_lms(int index, const std::vector<bool> &sl_seq){
    if(index+1 == (int)sl_seq.size())return true;
    if(0 < index && index < (int)sl_seq.size()){
        return sl_seq[index] && !sl_seq[index-1];
    }
    return false;
}

std::vector<int> get_bucket(int* seq, const int n, const int alphabet_size){
    std::vector<int> bucket(alphabet_size);
    for(int i=0 ; i<n ; i++)bucket[seq[i]]++;
    for(int i=0 ; i<alphabet_size-1 ; i++)bucket[i+1] += bucket[i];
    return bucket;
}

void sort_type_l(int* seq, const int n, const std::vector<bool> &is_type_s, int* arr, const int alphabet_size){
    std::vector<int> bucket = get_bucket(seq, n, alphabet_size);
    for(int i=0 ; i<n ; i++){
        if(arr[i] != -1 && arr[i]>0 && !is_type_s[arr[i]-1]){
            arr[bucket[seq[arr[i]-1]-1]++] = arr[i]-1;
        }
    }
}

void sort_type_s(int* seq, const int n, const std::vector<bool> &is_type_s, int* arr, const int alphabet_size){
    std::vector<int> bucket = get_bucket(seq, n, alphabet_size);
    for(int i=n-1 ; i>=0 ; i--){
        if(arr[i] != -1 && arr[i]>0 && is_type_s[arr[i]-1]){
            arr[--bucket[seq[arr[i]-1]]] = arr[i]-1;
        }
    }
}

void create_suffix_array(int* arr, int* seq, const int n, const int alphabet_size){
    for(int i=0 ; i<n ; i++)arr[i] = -1;
    std::vector<bool> is_type_s(n);
    is_type_s[n-1] = true;
    for(int i=n-2 ; i>=0 ; i--){
        if(seq[i] == seq[i+1])is_type_s[i] = is_type_s[i+1];
        else is_type_s[i] = seq[i] < seq[i+1];
    }
    std::vector<int> bucket = get_bucket(seq, n, alphabet_size);
    for(int i=0 ; i < n ; i++){
        if(is_lms(i, is_type_s))arr[--bucket[seq[i]]] = i;
    }
    sort_type_l(seq, n, is_type_s, arr, alphabet_size);
    sort_type_s(seq, n, is_type_s, arr, alphabet_size);

    // LMS-substringの元の数値列における位置を辞書順に左詰めで格納する
    int num_lms = 0;
    for(int i=0 ; i<n ; i++){
        if(is_lms(arr[i], is_type_s))arr[num_lms++] = arr[i];
    }
    // 辞書順以外を一旦クリア
    for(int i=num_lms ; i<n ; i++)arr[i] = -1;

    // LMS-substringのidを元の数値列における出現順で空いているところに格納する
    arr[num_lms + arr[0]/2] = 0;
    for(int i=1 ; i<num_lms ; i++){
        int p = arr[i-1], q = arr[i];

        bool same = i != 1; // 番兵回避
        for(int w=1 ; same ; w++){
            if(seq[p+w]!=seq[q+w])same = false;
            bool p_is_lms  = is_lms(p+w, is_type_s);
            bool q_is_lms  = is_lms(q+w, is_type_s);
            if(!p_is_lms && !q_is_lms)continue;
            if(!(p_is_lms && q_is_lms))same = false;
            break;
        }
        if(same){
            arr[num_lms + q/2] = arr[num_lms + p/2];
        }else{
            arr[num_lms + q/2] = arr[num_lms + p/2] + 1;
        }
    }

    // LMS-substringのidを元の数値列における出現順で右詰で格納する
    int next_alphabet_size = arr[num_lms + arr[num_lms-1]/2] + 1;
    for(int i=n-1, p=n-1 ; i >= num_lms ; i--){
        if(arr[i] != -1)arr[p--] = arr[i];
    }

    // 同じLMS-substringがある場合は再帰的に順序を求めるa
    if(next_alphabet_size != num_lms){
        create_suffix_array(arr, arr+n-num_lms, num_lms, next_alphabet_size);
        for(int i=0 ; i < num_lms ; i++)arr[n-num_lms+arr[i]] = i;
        for(int i=0, p=0 ; i < n ; i++){
            if(is_lms(i, is_type_s)){
                arr[arr[n-num_lms+p]] = i;
                p++;
            }
        }
    }

    // 辞書順以外を一旦クリア
    for(int i=num_lms ; i<n ; i++)arr[i] = -1;
    // 配列の末尾から正規の位置に詰めなおす
    bucket = get_bucket(seq, n, alphabet_size);
    for(int i=num_lms-1 ; i >= 0 ; i--){
        bucket[seq[arr[i]]]--;
        std::swap(arr[bucket[seq[arr[i]]]], arr[i]);
    }

    sort_type_l(seq, n, is_type_s, arr, alphabet_size);
    sort_type_s(seq, n, is_type_s, arr, alphabet_size);
}

std::vector<int> create_suffix_array_from_string(const std::string &str){
    std::set<char> _alphabet(str.begin(), str.end());
    std::vector<char> alphabet(_alphabet.begin(), _alphabet.end());
    int n = str.size()+1;
    int* seq = new int[n];
    int* arr = new int[n];
    for(int i=0 ; i < n-1 ; i++)seq[i] = lower_bound(alphabet.begin(), alphabet.end(), str[i]) - alphabet.begin() + 1;
    create_suffix_array(arr, seq, n, alphabet.size());
    return std::vector<int>(arr, arr+n);
}

};

#endif
