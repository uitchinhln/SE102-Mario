#pragma once
#include <Windows.h>

#include <signal.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <d3dx9.h>
#include <map>
#include <xtree>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <limits>
#include <chrono>
#include "RectF.h"


#ifndef PI
#define PI 3.1416f
#endif // !PI

#ifndef VECTOR_0
#define VECTOR_0 Vec2(0, 0)
#endif // !VEC0

static DWORD64 __id = 0;

typedef D3DXVECTOR2 Vec2;

using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");

wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

template<class key_type, class value_type>
pair<key_type, value_type> floor_entry(const map<key_type, value_type> &input, const key_type& key);

template<class key_type, class value_type>
value_type floor_value(const map<key_type, value_type> &input, const key_type& key);

template<class key_type, class value_type>
inline pair<key_type, value_type> floor_entry(const map<key_type, value_type>& input, const key_type& key)
{
	if (input.size() < 2) 
		return pair<key_type, value_type>((*input.begin()).first, (*input.begin()).second);

	auto it = input.lower_bound(key);
	if (it != input.begin() && (*it).first != key) {
		--it;
	}
	return pair<key_type, value_type>((*it).first, (*it).second);
}

template<class key_type, class value_type>
inline value_type floor_value(const map<key_type, value_type>& input, const key_type& key)
{
	if (input.size() < 2)
		return (input.begin())->second;

	auto it = input.lower_bound(key);
	if (it != input.begin() && (*it).first != key) {
		--it;
	}
	return it->second;
}
