#include <vector>
#include <iostream> 

using namespace std;
void merge(vector<int>& nums, int l,int m,int r)
{
	vector<int>tmp(r - l + 1);
	int i = l, j = m + 1, k = 0;
	while (i <= m && j <= r)
	{
		if (nums[i] <= nums[j])
		{
			tmp[k++] = nums[i++];
		}
		else
		{
			tmp[k++] = nums[j++];
		}
	}
	while (i <= m)
	{
		tmp[k++] = nums[i++];
	}
	while (j <= r)
	{
		tmp[k++] = nums[j++];
	}
	for (i = 0;i < k;i++)
	{
		nums[l + i] = tmp[i];
	}
}
void mergeSort(vector<int>& nums,int l,int r)
{
	if (l >= r)return;
	int m = l + (r - l) / 2;
	mergeSort(nums, l, m);
	mergeSort(nums, m + 1, r);
	merge(nums, l, m, r);
}
int main()
{
	vector<int>nums = { 1,5,3,68,3,2,55,6,3,24,34,3,22,1,3243,2,1 };
	mergeSort(nums, 0, nums.size()-1);
	for (int i = 0;i < nums.size();i++)
	{
		cout << nums[i] << ",";
	}
	return 0;
}