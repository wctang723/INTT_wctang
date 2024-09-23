#include <vector>

using namespace std;

void generateSubsets (vector<int> &nums, int m, int start, vector<int> &currentSubset, vector<vector<int>> &allSubsets) {
    if ( m == 0 ) {
        allSubsets.push_back (currentSubset);
        return;
    }
    for ( long unsigned int i = start; i <= (nums.size() - m); ++i ) {
        currentSubset.push_back (nums[i]);
        generateSubsets (nums, m - 1, i + 1, currentSubset, allSubsets);
        currentSubset.pop_back();
    }
}

vector<vector<int>> generateAllSubsets (vector<int> &nums, int m) {
    vector<vector<int>> allSubsets;
    vector<int>         currentSubset;

    generateSubsets (nums, m, 0, currentSubset, allSubsets);

    return allSubsets;
}

void test () {
    vector<vector<int>> combinations;
    vector<int>         nums = {1, 2, 3, 4, 5, 6, 7};

    combinations = generateAllSubsets (nums, 4);
    cout << "combinations size: " << combinations.size() << endl;
    for ( int j = 0; j < combinations.size(); j++ ) {
        cout << "[";
        for ( int k = 0; k <= 3; k++ ) {
            if ( k == 3 ) cout << combinations[j][k];
            else cout << combinations[j][k] << ",";
        }
        cout << "]" << endl;
    }
}
