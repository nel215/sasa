#include <boost/test/minimal.hpp>
#include <ctime>

#include "./suffix_array.hpp"
#include "./utils.hpp"

using namespace std;

// SuffixArray O(nlogn^2)
#define REP(i,n) for(int i=0; i<(int)(n) ; i++)
struct SAComp{
	int h;
	const vector<int> &g;
	SAComp(int h, vector<int> &g):h(h),g(g){}
	bool operator()(int a,int b){
		return a == b ? false : g[a] != g[b] ? g[a] < g[b] : g[a + h] < g[b + h];
	}
};

vector<int> SuffixArray(const string &str){
	int n = str.size();
	vector<int> sa(n+1),g(n+1),b(n+1);
	REP(i,n+1)sa[i] = i,g[i] = str[i];
	b[0] = b[n] = 0;
	SAComp comp(0,g);
	sort(sa.begin(),sa.end(),comp);
	for(comp.h = 1; b[n] != n; comp.h *= 2) {
		sort(sa.begin(),sa.end(),comp);
		REP(i,n)b[i+1] = b[i] + comp(sa[i], sa[i+1]);
		REP(i,n+1)g[sa[i]] = b[i];
	}
	return sa;
}
// HeightArray O(n)
vector<int> HeightArray(const string &str, const vector<int> &sa){
	int n = str.size(),h = 0;
	vector<int> ha(n+1),b(n+1);
	REP(i,n+1)b[sa[i]] = i;
	REP(i,n+1){
		if (b[i]){
			for (int j=sa[b[i]-1]; j+h<n && i+h<n && str[j+h]==str[i+h]; ++h);
			ha[b[i]] = h;
		}else ha[b[i]] = -1;
		if(h>0)--h;
	}
	return ha;
}

vector<int> create_vector(int arr[], int len){
    return vector<int>(arr, arr+len);
}

void test_create_suffix_array_from_string(){
    string data[] = {"mmiissiissippi", "abracadabra"};
    vector<int> expected[] = {
          create_vector((int[]){14,13,2,6,10,3,7,1,0,12,11,5,9,4,8}, 15)
        , create_vector((int[]){11,10,7,0,3,5,8,1,4,6,9,2}, 12)
    };
    for(int i=0 ; i<2 ; i++){
        vector<int> actual = sasa::create_suffix_array_from_string(data[i]);
        BOOST_CHECK(actual.size() == expected[i].size());
        for(int j=0 ; j<(int)actual.size() ; j++){
            BOOST_CHECK(actual[j] == expected[i][j]);
        }
    }
}

void test_construction_speed(){
    int n = 5000000;
    string str = "";
    for(int i=0 ; i<n ; i++){
        str += rand() % 2 ? 'a' : 'b';
    }
    clock_t start,end;
    start = clock();
    sasa::create_suffix_array_from_string(str);
    end = clock();

    double sais_time = (double)(end - start)/CLOCKS_PER_SEC;

    start = clock();
    SuffixArray(str);
    end = clock();

    double ls_time = (double)(end - start)/CLOCKS_PER_SEC;

    cout << "sais time: " << sais_time << " [s]" << endl;
    cout << "ls time: " << ls_time << " [s]" << endl;
}

int test_main(int argc, char* argv[]){
    test_create_suffix_array_from_string();
    test_construction_speed();
    return 0;
}
