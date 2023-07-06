#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <vector>
#include <map>

#include <set>
#include <queue>
#include "CustomVector.h"

using namespace std;

struct Person
{
    int age;
    string name;
};

void groupByAge(vector<Person> *person_array1)
{
    vector<Person> &person_array = *person_array1;
    unordered_map<int, int> age_freq;
    for (const Person &p : person_array)
    {
        ++age_freq[p.age];
    }

    unordered_map<int, int> age_indices;
    int idx = 0;
    for (const auto &p : age_freq)
    {
        age_indices[p.first] = idx;
        idx += p.second;
    }

    while (age_indices.size())
    {
        auto indices_map_itr = age_indices.begin();
        auto curr_index = indices_map_itr->second;
        Person curr_person = person_array[curr_index];
        auto to = age_indices.find(curr_person.age);
        swap(person_array[curr_index], person_array[to->second]);

        --age_freq[to->first];
        if (age_freq[to->first] > 0)
            ++to->second;
        else
            age_indices.erase(to);
    }
}


// II  Return true if subarray partial_assignmentfstart_row : end_row -
// II l][start_col : end_col - l] contains any duplicates in {l, 2,
// II partial_assignment.size()}; otherwise return false.
bool HasDuplicate(const vector<vector<int>> &partial_assignment,
                  int start_row, int end_row, int start_col, int end_col)
{
    deque<bool> is_present(partial_assignment.size() + 1, false);
    for (int i = start_row; i < end_row; ++i)
    {
        for (int j = start_col; j < end_col; ++j)
        {
            if (partial_assignment[i][j] != 0 && is_present[partial_assignment[i][j]])
            {
                return true;
            }
            is_present[partial_assignment[i][j]] = true;
        }
    }
    return false;
}

// II Check if a partially filled matrix has any conflicts.
bool IsValidSudoku(const vector<vector<int>> &partial_assignment)
{
    // II Check row constraints.
    for (int i = 0; i < partial_assignment.size(); ++i)
        if (HasDuplicate(partial_assignment, i, i + 1, 0,
                         partial_assignment.size()))
        {
            return false;
        }

    // II Check column constraints.
    for (int c = 0; c < partial_assignment.size(); ++c)
    {
        if (HasDuplicate(partial_assignment, 0, partial_assignment.size(), c,
                         c + 1))
        {
            return false;
        }
    }

    // II Check region constraints.
    int region_size = sqrt(partial_assignment.size());
    for (int I = 0; I < region_size; ++I)
    {
        for (int J = 0; J < region_size; ++J)
        {
            if (HasDuplicate(partial_assignment, region_size * I,
                             region_size * (I + 1), region_size * J, region_size * (J + 1)))
            {
                return false;
            }
        }
    }
    return true;

}




int solution(const vector<int> &nums, int distance) 
{
    const int n = nums.size();
    set<int> have;

    int r = numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) 
    {
        int curr = nums[i];
        if (i >= distance) 
        {
            have.insert(nums[i - distance]);
            auto upper_or_eq = have.lower_bound(curr);
            if (upper_or_eq != have.end()) {
                r = min(r, (*upper_or_eq) -curr);
            }
            if (upper_or_eq != have.begin()) {
                r = min(r, curr - *(--upper_or_eq));
            }
        } 
    }
    return r;
}





int main()
{
    multiset<int> vals{2,3,4,5,5};

    auto itr = vals.lower_bound(6);


   cout<< distance(vals.begin(), itr)<<endl;

}